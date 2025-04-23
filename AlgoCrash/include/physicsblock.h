#ifndef PHYSICSBLOCK_H
#define PHYSICSBLOCK_H

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <Box2D/Box2D.h>

/*!
 * \class PhysicsBlock
 * \brief Single sortable “tile” with a Box2D body and a text label.
 *
 * Each PhysicsBlock has both a QGraphicsRectItem and
 * a Box2D b2Body. This helps to highlight itself in
 * yellow during comparisons or green once the sorting started.
 */
class PhysicsBlock : public QGraphicsRectItem
{
public:

    /*!
     * \brief Constructs a block at world coordinates (\a x,\a y) with a displayed integer \a value.
     * \param world  Box2D world which creates the body.
     * \param x      Horizontal position in Box2D.
     * \param y      Vertical position in Box2D.
     * \param value  A displayed integer shown on the block
     */
    PhysicsBlock(b2World* world, float x, float y, int value);

    /*! Sync Box2D state shows in the  Qt graphics (It is called on each physics step). */
    void syncWithPhysics();

    /*! Numeric value carried by the block. */
    int getValue() const { return m_value; }

    /*! Animate horizontally so that the block ends up at slot \a index. */
    void moveToPosition(size_t index);

    /*! Returns true during a running animation. */
    bool isMoving() const { return m_isMoving; }

    /*! A raw Box2D body(Useful when the controller needs direct access). */
    b2Body* getBody() const { return body; }

    /*!
     * \brief Change the fill colour to indicate comparison or completion.
     * \param isActive   Return true when the block is under examination.
     * \param isSorted   Return true once the final position is fixed.
     */
    void highlight(bool isActive, bool isSorted = false);

private:
    b2Body* body;                       // Box2D representation
    QGraphicsSimpleTextItem* label;     // Numeric label
    int m_value;                        // Stored integer
    bool m_isMoving;                    // Animation flag
    b2Vec2 m_targetPosition;            // Destination for animation
};

#endif // PHYSICSBLOCK_H
