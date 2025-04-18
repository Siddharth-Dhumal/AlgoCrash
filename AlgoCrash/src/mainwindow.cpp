#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    world(new b2World(b2Vec2(0.0f, -10.0f))) // Gravity
{
    ui->setupUi(this);

    // Set up graphics scene
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(-400, -600, 800, 1200); // in pixels


    // Add invisible ground
    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, -6.0f); // in meters
    b2Body* ground = world->CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(50.0f, 1.0f);
    ground->CreateFixture(&groundShape, 0.0f);

    // Add initial blocks
    spawnInitialBlocks({5, 3, 8, 1, 4});

    // Timer to simulate Box2D world
    simTimer = new QTimer(this);
    connect(simTimer, &QTimer::timeout, this, [=]() {
        world->Step(1.0f / 60.0f, 6, 2);
        for (PhysicsBlock* block : blocks)
            block->syncWithPhysics();
    });
    simTimer->start(16); // ~60 FPS


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
        float x = startX + i * spacing;
        PhysicsBlock* block = new PhysicsBlock(world, x, 4.0f, values[i]); // Drop from y = 4m
        scene->addItem(block);
        blocks.push_back(block);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    if (scene && ui->graphicsView)
    {
        QRectF bounds = ui->graphicsView->viewport()->rect();
        scene->setSceneRect(bounds);
    }
}
