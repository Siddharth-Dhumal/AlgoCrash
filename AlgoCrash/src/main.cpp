#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

// Box2D
#include <Box2D/Box2D.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    // Simple Box2D test
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);

    // Define ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define falling body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    b2Body* body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f; // ← bounce!
    body->CreateFixture(&fixtureDef);

    // Simulate 60 steps
    for (int32 i = 0; i < 60; ++i) {
        world.Step(1.0f / 60.0f, 6, 2);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        qDebug() << "Step" << i << "Pos:" << position.y << "Angle:" << angle;
    }

    return app.exec();
}
