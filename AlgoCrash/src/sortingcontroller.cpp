#include "sortingcontroller.h"

SortingController::SortingController()
    : m_phase(HIGHLIGHT)
    , m_currentIndex(0)
    , m_lastSortedIndex(0)
    , m_isComplete(false)
    , m_isSwapping(false)
    , m_comparisonCount(0)
    , m_swapCount(0)
{
}

bool SortingController::step()
{
    switch (m_algorithm)
    {
    case BUBBLE:
        return bubbleSortStep();

    case INSERTION:
        return insertionSortStep();

    // More algorithm adding

    default:
        // should never happen, but safe fallback
        return false;
    }
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

    if (m_isSwapping) {
        bool stillMoving = false;
        for (auto block : m_blocks) {
            if (block->isMoving()) {
                stillMoving = true;
                break;
            }
        }
        if (stillMoving)
            return true;

        m_isSwapping = false;
        for (auto block : m_blocks)
            block->highlight(false);
    }

    if (m_phase == HIGHLIGHT) {
        for (auto block : m_blocks)
            block->highlight(false);

        if (m_currentIndex + 1 >= m_blocks.size() - m_lastSortedIndex) {
            m_lastSortedIndex++;
            m_currentIndex = 0;
            if (m_lastSortedIndex >= m_blocks.size() - 1) {
                m_isComplete = true;
                for (size_t i = 0; i < m_blocks.size(); ++i)
                    m_blocks[i]->moveToPosition(i);
                return false;
            }
        }

        PhysicsBlock* b1 = m_blocks[m_currentIndex];
        PhysicsBlock* b2 = m_blocks[m_currentIndex + 1];
        b1->highlight(true);
        b2->highlight(true);

        m_comparisonCount++;
        m_phase = ACTION;
        return true;
    }

    else {
        PhysicsBlock* b1 = m_blocks[m_currentIndex];
        PhysicsBlock* b2 = m_blocks[m_currentIndex + 1];
        if (b1->getValue() > b2->getValue()) {
            performSwap(m_currentIndex, m_currentIndex + 1);
            m_swapCount++;
            m_isSwapping = true;
        }
        m_currentIndex++;
        m_phase = HIGHLIGHT;
        return true;
    }
}

bool SortingController::insertionSortStep()
{
    if (m_isComplete || m_blocks.size() < 2) return false;

    /* wait for any current swap animation to finish */
    if (m_isSwapping) {
        for (auto *b : m_blocks)
            if (b->isMoving()) return true;
        m_isSwapping = false;
        for (auto *b : m_blocks) b->highlight(false);
    }

    if (m_phase == HIGHLIGHT) {
        /* end of inner scan? */
        if (m_innerIdx == 0 ||
            m_blocks[m_innerIdx - 1]->getValue() <= m_blocks[m_innerIdx]->getValue())
        {
            ++m_outerIdx;
            if (m_outerIdx >= m_blocks.size()) {
                m_isComplete = true;
                return false;
            }
            m_innerIdx = m_outerIdx;
        }

        m_blocks[m_innerIdx]->highlight(true);
        m_blocks[m_innerIdx - 1]->highlight(true);
        ++m_comparisonCount;
        m_phase = ACTION;
        return true;
    }
    else {                                  /* ACTION */
        if (m_blocks[m_innerIdx - 1]->getValue() > m_blocks[m_innerIdx]->getValue()) {
            performSwap(m_innerIdx - 1, m_innerIdx);
            ++m_swapCount;
            m_isSwapping = true;
        }
        if (m_innerIdx > 0) --m_innerIdx;
        m_phase = HIGHLIGHT;
        return true;
    }
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
    m_phase = HIGHLIGHT;
    m_currentIndex      = 0;
    m_lastSortedIndex   = 0;
    m_isComplete        = false;
    m_isSwapping        = false;
    m_comparisonCount   = 0;
    m_swapCount         = 0;

    m_outerIdx          = 1;
    m_innerIdx          = 1;

    // Reset highlights
    for (PhysicsBlock* block : m_blocks) {
        block->highlight(false);
    }
}
