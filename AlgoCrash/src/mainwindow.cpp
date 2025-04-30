/**
 * mainwindow.cpp
 *
 * This file implements the MainWindow class which serves as the application's
 * main interface, handling UI interaction, physics simulation, and sorting visualization.
 *
 * Checked by: [Yen yee tan]
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include <QPushButton>
#include <QLabel>
#include <cmath>
#include <cstdlib>  // for rand
#include <ctime>    // for time
#include <QInputDialog>
#include <QMessageBox>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(new b2World(b2Vec2(0.0f, -10.0f))) // Gravity
{
    ui->setupUi(this);

    // Set initial interval to match slider value
    sortTimer = new QTimer(this);
    int sliderValue = ui->speedSlider->value();
    int adjustedInterval = std::max(50, 1000 - sliderValue);  // 1000 or a max value depending on your slider
    sortTimer->setInterval(adjustedInterval);

    connect(ui->speedSlider, &QSlider::valueChanged, this, [this](int newValue) {
        int invertedInterval = std::max(50, 1000 - newValue); // Invert logic
        sortTimer->setInterval(invertedInterval);
        ui->speedSlider->setToolTip(QString("Speed: %1 ms per step").arg(invertedInterval));
    });

    /* ── Algorithm selector ─────────────────────────── */
    connect(ui->algorithmComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [this](int idx)
            {
                // Stop any running sort
                sortTimer->stop();
                ui->sortButton->setText("Start Sort");

                // Set algorithm
                sortController.setAlgorithm(
                    idx == 0 ? SortingController::BUBBLE :
                        idx == 1 ? SortingController::SELECTION :
                        SortingController::INSERTION);

                // Hide sorted label and update stats
                sortedLabel->setVisible(false);
                updateStatistics();

                // 🔽 Update explanation panel
                switch (idx) {
                case 0:
                    ui->algorithmExplanationBrowser->setText(
                        "🔁 <b>Bubble Sort</b>\n\n"
                        "• Compares each pair of adjacent elements\n"
                        "• Swaps them if they’re in the wrong order\n"
                        "• Repeats until no more swaps needed\n\n"
                        "<b>Complexity:</b> O(n²)\n<b>Stable:</b> Yes\n<b>Intuition:</b> Heavy values bubble to the top."
                        );
                    break;
                case 1:
                    ui->algorithmExplanationBrowser->setText(
                        "📌 <b>Selection Sort</b>\n\n"
                        "• Repeatedly selects the smallest element\n"
                        "• Swaps it into its correct position\n\n"
                        "<b>Complexity:</b> O(n²)\n<b>Stable:</b> No\n<b>Intuition:</b> Picks the smallest and places it correctly."
                        );
                    break;
                case 2:
                    ui->algorithmExplanationBrowser->setText(
                        "🪛 <b>Insertion Sort</b>\n\n"
                        "• Builds the sorted array one item at a time\n"
                        "• Inserts current element into the correct place\n\n"
                        "<b>Complexity:</b> O(n²)\n<b>Stable:</b> Yes\n<b>Intuition:</b> Like sorting playing cards."
                        );
                    break;
                }
            });


    sortedLabel = new QLabel("✔ Sorting Complete!", this);
    sortedLabel->setAlignment(Qt::AlignCenter);
    sortedLabel->setStyleSheet("color: green; font-weight: bold;");
    sortedLabel->setVisible(false);

    ui->verticalLayout->insertWidget(3, sortedLabel);
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Set up graphics scene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(-400, -600, 800, 1200);

    // Make view resize properly
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Add invisible ground
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, -6.0f);
    b2Body* ground = world->CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(50.0f, 1.0f);

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    groundFixtureDef.friction = 0.8f; // Higher friction to slow down blocks
    ground->CreateFixture(&groundFixtureDef);

    // Add initial blocks
    spawnInitialBlocks({5, 3, 8, 1, 4});

    // Set up the sorting controller
    sortController.setBlocks(blocks);
    sortController.statusCallback = [this](const QString& msg) {
        ui->explanationLabel->setText(msg);
    };
    // Timer to simulate Box2D world
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, [=]() {
        world->Step(1.0f / 60.0f, 6, 2);
        for (PhysicsBlock* block : blocks)
            block->syncWithPhysics();
        updateButtonStates();
    });
    simTimer->start(16); // ~60 FPS

    // Timer for automatic sorting (disabled by default)
    sortTimer = new QTimer(this);
    connect(sortTimer, &QTimer::timeout, this, [=]() {

        if (!sortController.step()) {
            sortTimer->stop();
            ui->sortButton->setText("Start Sort");
        }
        updateStatistics();
    });

    // Connect UI buttons
    connect(ui->stepButton, &QPushButton::clicked, this, &MainWindow::onStepButtonClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    connect(ui->customizeButton, &QPushButton::clicked,
            this, &MainWindow::onCustomizeButtonClicked);

    // Trigger algorithm description update on startup
    int currentIdx = ui->algorithmComboBox->currentIndex();
    ui->algorithmComboBox->setCurrentIndex((currentIdx + 1) % 3);  // Change to a different one temporarily
    ui->algorithmComboBox->setCurrentIndex(currentIdx);           // Then back to original


    // Initial view setup
    QTimer::singleShot(0, this, [this]() {
        if (ui->graphicsView && scene) {
            ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete world;
}

void MainWindow::spawnInitialBlocks(const std::vector<int>& values)
{
    float startX = -3.0f;
    float spacing = 1.5f;

    for (size_t i = 0; i < values.size(); ++i)
    {
        // Add slight random offset to x position (±0.1)
        float randomXOffset = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 0.2f;
        float x = startX + i * spacing + randomXOffset;

        // Add random variation to drop height (between 4.0 and 5.0)
        float randomHeight = 4.0f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 1.0f;

        PhysicsBlock* block = new PhysicsBlock(world, x, randomHeight, values[i]);
        scene->addItem(block);
        blocks.push_back(block);
    }
}

void MainWindow::onStepButtonClicked()
{
    // sortController.step();
    // updateStatistics();

    world->SetGravity(b2Vec2(0.0f, 0.0f));
    for (PhysicsBlock* block : blocks) {
        b2Body* b = block->getBody();
        b2Vec2 p = b->GetPosition();
        b->SetTransform(p, 0.0f);
        b->SetType(b2_staticBody);
        b->SetLinearVelocity(b2Vec2_zero);
        b->SetAngularVelocity(0.0f);
    }

    // Then start the next step.
    if (!sortController.step()) {
        ui->sortButton->setText("Start Sort");
    }
    updateStatistics();
}

void MainWindow::onSortButtonClicked()
{
    if (sortTimer->isActive()) {
        sortTimer->stop();
        ui->sortButton->setText(
            "Continue Sort");
    } else {
        world->SetGravity(b2Vec2(0.0f, 0.0f));

        for (PhysicsBlock* block : blocks) {
            b2Body* b = block->getBody();
            b2Vec2 p = b->GetPosition();
            b->SetTransform(p, 0.0f);
            b->SetType(b2_staticBody);
            b->SetLinearVelocity(b2Vec2_zero);
            b->SetAngularVelocity(0.0f);
        }

        sortTimer->start(std::max(50, 1000 - ui->speedSlider->value()));  // 1 s per step
               ui->sortButton->setText("Pause Sort");
    }
}

void MainWindow::onResetButtonClicked()
{
    world->SetGravity(b2Vec2(0.0f, -10.0f));

    // Stop sorting
    sortTimer->stop();
    ui->sortButton->setText("Start Sort");

    // Clear existing blocks
    for (PhysicsBlock* block : blocks) {
        scene->removeItem(block);
        world->DestroyBody(block->getBody());
        delete block;
    }
    blocks.clear();

    // Create new blocks
    spawnInitialBlocks({5, 3, 8, 1, 4});

    // Reset the sorting controller
    sortController.setBlocks(blocks);
    sortController.statusCallback = [this](const QString& msg) {
        ui->explanationLabel->setText(msg);
    };
    ui->explanationLabel->setText("Sorting has been reset! Click 'Start Sort' to begin.");

    // Update statistics
    updateStatistics();
}

void MainWindow::onCustomizeButtonClicked()
{
    world->SetGravity(b2Vec2(0.0f, -10.0f));

    bool ok;
    QString text = QInputDialog::getText(
        this,
        "Customize Data",
        "Enter comma-separated integers:",
        QLineEdit::Normal,
        QString(),
        &ok
        );
    if (!ok || text.isEmpty())
        return;

    QStringList parts = text.split(",", Qt::SkipEmptyParts);
    std::vector<int> values;
    for (const QString& part : parts) {
        bool numOk = false;
        int val = part.trimmed().toInt(&numOk);
        if (numOk) values.push_back(val);
    }
    if (values.empty()) {
        QMessageBox::warning(
            this,
            "Invalid Input",
            "Please enter valid integers separated by commas."
            );
        return;
    }

    sortTimer->stop();
    ui->sortButton->setText("Start Sort");
    sortedLabel->setVisible(false);

    for (PhysicsBlock* block : blocks) {
        scene->removeItem(block);
        world->DestroyBody(block->getBody());
        delete block;
    }
    blocks.clear();

    spawnInitialBlocks(values);
    sortController.setBlocks(blocks);
    sortController.statusCallback = [this](const QString& msg) {
        ui->explanationLabel->setText(msg);
    };

    ui->explanationLabel->setText("Custom data loaded! Click 'Start Sort' to begin.");

    updateStatistics();
}

void MainWindow::updateStatistics()
{
    int comps = sortController.getComparisonCount();
    int swaps = sortController.getSwapCount();

    ui->comparisonLabel->setText(QString("Comparisons: %1").arg(comps));
    ui->swapLabel->setText(QString("Swaps: %1").arg(swaps));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (ui->graphicsView) {
        // Keep the same scene coordinates but fit view to window
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}

bool MainWindow::areBlocksSettled() const
{
    for (PhysicsBlock* block : blocks) {
        b2Body* body = block->getBody();
        if (body->GetType() != b2_staticBody) {
            return false;
        }
    }
    return true;
}

void MainWindow::updateButtonStates()
{
    bool ready = areBlocksSettled();
    ui->sortButton->setEnabled(ready);
    ui->stepButton->setEnabled(ready);
}
