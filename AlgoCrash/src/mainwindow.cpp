#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include <QPushButton>
#include <QLabel>
#include <cmath>
#include <cstdlib>  // for rand
#include <ctime>    // for time

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(new b2World(b2Vec2(0.0f, -10.0f))) // Gravity
{
    ui->setupUi(this);

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

    // Timer to simulate Box2D world
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, [=]() {
        world->Step(1.0f / 60.0f, 6, 2);
        for (PhysicsBlock* block : blocks)
            block->syncWithPhysics();
    });
    simTimer->start(16); // ~60 FPS

    // Timer for automatic sorting (disabled by default)
    sortTimer = new QTimer(this);
    connect(sortTimer, &QTimer::timeout, this, [=]() {
        if (!sortController.bubbleSortStep()) {
            // Sorting is complete, stop the timer
            sortTimer->stop();
            ui->sortButton->setText("Start Sort");
        }
        updateStatistics();
    });

    // Connect UI buttons
    connect(ui->stepButton, &QPushButton::clicked, this, &MainWindow::onStepButtonClicked);
    connect(ui->sortButton, &QPushButton::clicked, this, &MainWindow::onSortButtonClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);

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
    sortController.bubbleSortStep();
    updateStatistics();
}

void MainWindow::onSortButtonClicked()
{
    if (sortTimer->isActive()) {
        sortTimer->stop();
        ui->sortButton->setText("Continue Sort");
    } else {
        sortTimer->start(500); // Adjust timing as needed
        ui->sortButton->setText("Pause Sort");
    }
}

void MainWindow::onResetButtonClicked()
{
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

    // Update statistics
    updateStatistics();
}

void MainWindow::updateStatistics()
{
    // If we had labels for statistics, we would update them here
    // For now, just print to the console for debugging
    qDebug() << "Comparisons:" << sortController.getComparisonCount()
             << "Swaps:" << sortController.getSwapCount();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (ui->graphicsView) {
        // Keep the same scene coordinates but fit view to window
        ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}
