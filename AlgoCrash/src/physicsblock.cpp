#include "physicsblock.h"
#include <cmath>
#include <QBrush>

PhysicsBlock::PhysicsBlock(b2World* world, float x, float y, int value)
    : m_value(value)
    , m_isMoving(false)
{
    // Draw a centered 50x50 square
    setRect(-25, -25, 50, 50);

    // Set default brush
    setBrush(QBrush(Qt::white));

    // Add a number label
    label = new QGraphicsSimpleTextItem(QString::number(value), this);
    label->setPos(-10, -10); // Center the label

    // Define the Box2D body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);

    // Add damping to reduce oscillation
    body->SetLinearDamping(0.5f);
    body->SetAngularDamping(0.5f);

    b2PolygonShape shape;
    shape.SetAsBox(0.5f, 0.5f); // 1m x 1m box

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    // Reduced restitution for less bouncing
    float randomRestitution = 0.2f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.3f;
    fixtureDef.restitution = randomRestitution;

    body->CreateFixture(&fixtureDef);
}

void PhysicsBlock::syncWithPhysics()
{
    if (m_isMoving) {
        // Get current position
        b2Vec2 currentPos = body->GetPosition();

        // Calculate direction to target
        b2Vec2 direction = m_targetPosition - currentPos;

        // If we're close enough, stop moving
        if (direction.Length() < 0.1f) {
            m_isMoving = false;

            // Set position precisely to target
            body->SetTransform(m_targetPosition, body->GetAngle());

            // Reset velocity
            body->SetLinearVelocity(b2Vec2(0, 0));
        }
        else {
            // Apply force towards target
            float force = 10.0f; // Adjust as needed
            body->ApplyForceToCenter(b2Vec2(direction.x * force, direction.y * force), true);

            // Dampen velocity to prevent oscillation
            b2Vec2 velocity = body->GetLinearVelocity();
            body->SetLinearVelocity(b2Vec2(velocity.x * 0.95f, velocity.y * 0.95f));
        }
    }

    // Update visual position
    b2Vec2 pos = body->GetPosition();
    float angle = body->GetAngle();

    setPos(pos.x * 100, -pos.y * 100); // Convert Box2D coords to pixels
    setRotation(-angle * 180.0 / M_PI);
}

void PhysicsBlock::moveToPosition(size_t index)
{
    // Calculate the target position based on index
    float startX = -3.0f;
    float spacing = 1.5f;
    float targetX = startX + index * spacing;
    float targetY = 0.0f; // All blocks should align at the same height

    m_targetPosition = b2Vec2(targetX, targetY);
    m_isMoving = true;

    // Give an initial push toward the target
    b2Vec2 currentPos = body->GetPosition();
    b2Vec2 direction = m_targetPosition - currentPos;
    direction.Normalize();
    float impulse = 5.0f; // Adjust as needed
    body->ApplyLinearImpulse(b2Vec2(direction.x * impulse, direction.y * impulse), body->GetWorldCenter(), true);
}

void PhysicsBlock::highlight(bool isActive)
{
    if (isActive) {
        // Use a bright color for blocks being compared
        setBrush(QBrush(Qt::yellow));
    } else {
        // Reset to default color
        setBrush(QBrush(Qt::white));
    }
}
