#ifndef PHYSICSBLOCK_H
#define PHYSICSBLOCK_H

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <Box2D/Box2D.h>

class PhysicsBlock : public QGraphicsRectItem
{
public:
    PhysicsBlock(b2World* world, float x, float y, int value);
    void syncWithPhysics();

    int getValue() const { return m_value; }
    void moveToPosition(size_t index);
    bool isMoving() const { return m_isMoving; }
    b2Body* getBody() const { return body; }
    void highlight(bool isActive);

private:
    b2Body* body;
    QGraphicsSimpleTextItem* label;
    int m_value;
    bool m_isMoving;
    b2Vec2 m_targetPosition;
};

#endif // PHYSICSBLOCK_H
