/**
 * physicsblock.h
 *
 * This file defines the PhysicsBlock class which combines Box2D physics
 * with Qt graphics to create interactive blocks for the visualization.
 *
 * Checked by: Siddharth Dhumal
 */
#ifndef PHYSICSBLOCK_H
#define PHYSICSBLOCK_H

#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <Box2D/Box2D.h>

/**
 * PhysicsBlock
 *
 * Represents a physical block in the simulation that follows Box2D physics
 * and is rendered using Qt graphics. Used to visualize values being sorted.
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

    /**
     * Updates the graphical position to match the physics position
     */
    void syncWithPhysics();

    /**
     * Moves the block to the specified index position
     *
     * @param index The target position index
     */
    void moveToPosition(size_t index);

    /**
     * Checks if the block is currently moving
     *
     * @return True if the block is moving, false otherwise
     */
    bool isMoving() const { return m_isMoving; }

    /**
     * Highlights the block for visualization purposes
     *
     * @param isActive Whether the block is being compared/active
     * @param isSorted Whether the block is in its sorted position
     */
    void highlight(bool isActive, bool isSorted = false);

    /**
     * Gets the Box2D body of this block
     *
     * @return The Box2D body
     */
    b2Body* getBody() const { return body; }

    /**
     * Gets the value of this block
     *
     * @return The block's value
     */
    int getValue() const { return m_value; }

private:
    b2Body* body;                       // Box2D representation
    QGraphicsSimpleTextItem* label;     // Numeric label
    int m_value;                        // Stored integer
    bool m_isMoving;                    // Animation flag
    b2Vec2 m_targetPosition;            // Destination for animation
};

#endif // PHYSICSBLOCK_H
