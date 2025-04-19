#include "sortingcontroller.h"

SortingController::SortingController()
    : m_currentIndex(0)
    , m_lastSortedIndex(0)
    , m_isComplete(false)
    , m_isSwapping(false)
    , m_comparisonCount(0)
    , m_swapCount(0)
{
}

void SortingController::setBlocks(std::vector<PhysicsBlock*>& blocks)
{
    m_blocks = blocks;
    reset();
}

bool SortingController::bubbleSortStep()
{
    if (m_isComplete || m_blocks.size() < 2)
        return false;

    // Reset any previous highlights
    for (PhysicsBlock* block : m_blocks) {
        block->highlight(false);
    }

    // If currently in the middle of a swap, wait until it's complete
    if (m_isSwapping) {
        // Check if blocks are close enough to their target positions
        // For simplicity, we'll just set it to false immediately
        m_isSwapping = false;
    }

    if (!m_isSwapping) {
        // We can proceed with the next comparison
        if (m_currentIndex + 1 >= m_blocks.size() - m_lastSortedIndex) {
            // Reached the end of this pass
            m_lastSortedIndex++;
            m_currentIndex = 0;

            if (m_lastSortedIndex >= m_blocks.size() - 1) {
                m_isComplete = true;
                return false; // Sorting complete
            }
        }

        // Get blocks to compare
        PhysicsBlock* block1 = m_blocks[m_currentIndex];
        PhysicsBlock* block2 = m_blocks[m_currentIndex + 1];

        // Highlight blocks being compared
        block1->highlight(true);
        block2->highlight(true);

        // Increment comparison count
        m_comparisonCount++;

        // Compare and swap if needed
        int value1 = block1->getValue();
        int value2 = block2->getValue();

        if (value1 > value2) {
            // Need to swap
            performSwap(m_currentIndex, m_currentIndex + 1);
            m_isSwapping = true;
            m_swapCount++;
        }

        m_currentIndex++;
    }

    return true; // Sorting still in progress
}

void SortingController::performSwap(size_t index1, size_t index2)
{
    // Swap the blocks in our array
    std::swap(m_blocks[index1], m_blocks[index2]);

    // Get their positions
    PhysicsBlock* block1 = m_blocks[index1];
    PhysicsBlock* block2 = m_blocks[index2];

    // Tell the blocks to animate to their new positions
    block1->moveToPosition(index1);
    block2->moveToPosition(index2);
}

bool SortingController::isSortingComplete() const
{
    return m_isComplete;
}

void SortingController::reset()
{
    m_currentIndex = 0;
    m_lastSortedIndex = 0;
    m_isComplete = false;
    m_isSwapping = false;
    m_comparisonCount = 0;
    m_swapCount = 0;

    // Reset highlights
    for (PhysicsBlock* block : m_blocks) {
        block->highlight(false);
    }
}
