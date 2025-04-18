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

private:
    b2Body* body;
    QGraphicsSimpleTextItem* label;
};

#endif // PHYSICSBLOCK_H
