#include "physicsblock.h"
#include <cmath>

PhysicsBlock::PhysicsBlock(b2World* world, float x, float y, int value)
{
    // Draw a centered 50x50 square
    setRect(-25, -25, 50, 50);

    // Add a number label
    label = new QGraphicsSimpleTextItem(QString::number(value), this);
    label->setPos(-10, -10); // Center the label

    // Define the Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(0.5f, 0.5f); // 1m x 1m box

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;

    body->CreateFixture(&fixtureDef);

}

void PhysicsBlock::syncWithPhysics()
{
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle();

    setPos(pos.x * 100, -pos.y * 100); // Convert Box2D coords to pixels
    setRotation(-angle * 180.0 / M_PI);
}
