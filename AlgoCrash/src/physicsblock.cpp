/**
 * physicsblock.cpp
 *
 * This file implements the PhysicsBlock class which combines Box2D physics with
 * Qt graphics to create interactive blocks for the sorting visualization.
 *
 * Checked by: [Sidharth Dhumal]
 */
#include "physicsblock.h"
#include <cmath>
#include <QBrush>
#include <QFont>

PhysicsBlock::PhysicsBlock(b2World* world, float x, float y, int value)
    : m_value(value)
    , m_isMoving(false)
{
    setRect(-40, -40, 80, 80);
    setBrush(QBrush(Qt::white));

    label = new QGraphicsSimpleTextItem(QString::number(value), this);
    QFont f = label->font();
    f.setPointSize(35);
    f.setBold(true);
    label->setFont(f);
    label->setPos(-20, -20);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);

    body->SetLinearDamping(0.5f);
    body->SetAngularDamping(0.5f);

    b2PolygonShape shape;
    shape.SetAsBox(0.5f, 0.5f); // 1m x 1m box

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.7f; // Less bouncing

    body->CreateFixture(&fixtureDef);
}

namespace {
    // World (meters) → Scene (pixels), including Y flip
    QPointF worldToScene(const b2Vec2& w) {
        return QPointF(w.x * kPixelsPerMeter,
                       -w.y * kPixelsPerMeter);
    }
    // Radians → Qt degrees (with sign flip)
    float radToDeg(float rad) {
        return -rad * kRadiansToDegrees;
    }
}

void PhysicsBlock::syncWithPhysics()
{
    b2Vec2 pos = body->GetPosition();

    // 1) Handle kinematic “moveToPosition” animation
    if (m_isMoving) {
        float dx = m_targetPosition.x - pos.x;
        if (std::abs(dx) < 0.05f) {
            // Close enough: snap to target and become static
            m_isMoving = false;
            body->SetTransform(m_targetPosition, 0.0f);
            body->SetType(b2_staticBody);
            body->SetLinearVelocity(b2Vec2_zero);
            body->SetAngularVelocity(0.0f);
        } else {
            // Slide toward target at kMoveSpeed
            float vx = (dx > 0 ? 1.0f : -1.0f) * kMoveSpeed;
            body->SetType(b2_kinematicBody);
            body->SetLinearVelocity(b2Vec2(vx, 0.0f));
        }
        pos = body->GetPosition();  // update after kinematic move
    }

    // 2) Detect landing: nearly still and below threshold
    if ((body->GetType() == b2_dynamicBody || body->GetType() == b2_kinematicBody) &&
        body->GetLinearVelocity().Length() < 0.01f &&
        pos.y <= kLandingYThreshold)
    {
        // Snap to uniform landing level
        body->SetType(b2_staticBody);
        body->SetLinearVelocity(b2Vec2_zero);
        body->SetAngularVelocity(0.0f);

        b2Vec2 landingPos = body->GetPosition();
        landingPos.y = kLandingYSnap;
        body->SetTransform(landingPos, 0.0f);
    }

    // 3) Finally, update the Qt item’s position & rotation
    QPointF scenePos = worldToScene(pos);
    setPos(scenePos);
    setRotation(radToDeg(body->GetAngle()));
}

void PhysicsBlock::moveToPosition(size_t index)
{
    float startX = -3.0f;
    float spacing = 1.5f;
    float targetX = startX + index * spacing;

    float currentY = body->GetPosition().y;
    m_targetPosition = b2Vec2(targetX, currentY);
    m_isMoving = true;
}

void PhysicsBlock::highlight(bool isActive, bool isSorted)
{
    m_activeHighlight = isActive;
    m_sortedHighlight = isSorted;
    if (isSorted) {
        setBrush(QBrush(Qt::green)); // Green for sorted
    }
    else if (isActive) {
        setBrush(QBrush(Qt::yellow)); // Yellow for active comparison
    }
    else {
        setBrush(QBrush(Qt::white)); // White = default
    }
}

void PhysicsBlock::setValue(int newValue)
{
    m_value = newValue;
    label->setText(QString::number(newValue));
}
