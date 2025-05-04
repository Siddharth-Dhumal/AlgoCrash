/**
 * sortingcontroller.cpp
 *
 * This file implements the SortingController class which manages the execution
 * of sorting algorithms (bubble sort, insertion sort, selection sort) and
 * tracks performance statistics.
 *
 * Checked by: [Devin Gupta]
 */

#include "sortingcontroller.h"

SortingController::SortingController()
    : m_phase(HIGHLIGHT)
    , m_currentIndex(0)
    , m_lastSortedIndex(0)
    , m_isComplete(false)
    , m_isSwapping(false)
    , m_comparisonCount(0)
    , m_swapCount(0)
    , m_outerIdx(1)
    , m_innerIdx(1)
    , m_minIndex(0)
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

    case SELECTION:
        return selectionSortStep();

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

    if (!m_isComplete && !m_isSwapping && m_phase == HIGHLIGHT) {
        saveState();
    }

    if (m_phase == HIGHLIGHT) {
        for (auto block : m_blocks)
            block->highlight(false);

        if (m_currentIndex + 1 >= m_blocks.size() - m_lastSortedIndex) {
            m_lastSortedIndex++;
            m_currentIndex = 0;
            if (m_lastSortedIndex >= m_blocks.size() - 1) {

                m_isComplete = true;
                if (statusCallback)
                    statusCallback("Bubble Sort complete!");
                for (auto* block : m_blocks)
                    block->highlight(true, true); // ← green = sorted
                for (size_t i = 0; i < m_blocks.size(); ++i)
                    m_blocks[i]->moveToPosition(i);
                return false;
            }
        }

        PhysicsBlock* b1 = m_blocks[m_currentIndex];
        PhysicsBlock* b2 = m_blocks[m_currentIndex + 1];
        b1->highlight(true);
        b2->highlight(true);
        if (statusCallback)
            statusCallback(QString("Comparing elements at index %1 and %2")
                               .arg(m_currentIndex)
                               .arg(m_currentIndex + 1));

        m_comparisonCount++;
        m_phase = ACTION;
        return true;
    }

    else {
        PhysicsBlock* b1 = m_blocks[m_currentIndex];
        PhysicsBlock* b2 = m_blocks[m_currentIndex + 1];
        if (b1->getValue() > b2->getValue()) {
            int val1 = b1->getValue();
            int val2 = b2->getValue();
            performSwap(m_currentIndex, m_currentIndex + 1);
            if (statusCallback)
                statusCallback(QString("Swapping %1 and %2").arg(val1).arg(val2));
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

    if (!m_isComplete && !m_isSwapping && m_phase == HIGHLIGHT) {
        saveState();
    }

    if (m_phase == HIGHLIGHT) {
        /* end of inner scan? */
        if (m_innerIdx == 0 ||
            m_blocks[m_innerIdx - 1]->getValue() <= m_blocks[m_innerIdx]->getValue())
        {
            ++m_outerIdx;
            if (m_outerIdx >= m_blocks.size()) {
                m_isComplete = true;
                if (statusCallback)
                    statusCallback("Insertion Sort complete!");

                // 1) Highlight sorted
                for (auto* block : m_blocks)
                    block->highlight(true, true);

                // 2) Reposition all blocks
                for (size_t i = 0; i < m_blocks.size(); ++i)
                    m_blocks[i]->moveToPosition(i);

                // 3) End
                return false;
            }
            m_innerIdx = m_outerIdx;
        }

        m_blocks[m_innerIdx]->highlight(true);
        m_blocks[m_innerIdx - 1]->highlight(true);

        if (statusCallback)
            statusCallback(QString("Comparing index %1 with index %2")
                               .arg(m_innerIdx).arg(m_innerIdx - 1));

        ++m_comparisonCount;
        m_phase = ACTION;
        return true;
    }
    else {                                  /* ACTION */
        if (m_blocks[m_innerIdx - 1]->getValue() > m_blocks[m_innerIdx]->getValue()) {
            int val1 = m_blocks[m_innerIdx - 1]->getValue();
            int val2 = m_blocks[m_innerIdx]->getValue();
            performSwap(m_innerIdx - 1, m_innerIdx);

            if (statusCallback)
                statusCallback(QString("Swapping %1 and %2").arg(val1).arg(val2));
            ++m_swapCount;
            m_isSwapping = true;
        }
        if (m_innerIdx > 0) --m_innerIdx;
        m_phase = HIGHLIGHT;
        return true;
    }
}

bool SortingController::selectionSortStep()
{
    // Terminate if sorting is complete or array size is less than 2
    if (m_isComplete || m_blocks.size() < 2) return false;

    // Wait if a swap animation is in progress
    if (m_isSwapping) {
        for (auto *b : m_blocks)
            if (b->isMoving()) return true;
        m_isSwapping = false;
        for (auto *b : m_blocks) b->highlight(false);
    }

    if (!m_isComplete && !m_isSwapping && m_phase == HIGHLIGHT) {
        saveState();
    }

    if (m_phase == HIGHLIGHT) {
        // Clear highlights from all blocks
        for (auto *b : m_blocks) b->highlight(false);

        // Check if there are more elements to compare in the current pass
        if (m_currentIndex < m_blocks.size()) {
            // Highlight the current element and the current minimum
            m_blocks[m_currentIndex]->highlight(true);
            m_blocks[m_minIndex]->highlight(true);

            if (statusCallback)
                statusCallback(QString("Comparing index %1 with current min at index %2")
                                   .arg(m_currentIndex).arg(m_minIndex));

            m_comparisonCount++;
            m_phase = ACTION;
            return true;
        } else {
            // Pass complete: swap the minimum with m_lastSortedIndex
            if (m_minIndex != m_lastSortedIndex) {

                int val1 = m_blocks[m_minIndex]->getValue();
                int val2 = m_blocks[m_lastSortedIndex]->getValue();

                performSwap(m_minIndex, m_lastSortedIndex);

                if (statusCallback)
                    statusCallback(QString("Swapping %1 and %2").arg(val1).arg(val2));

                m_swapCount++;
                m_isSwapping = true;
            }
            // Move to the next pass
            m_lastSortedIndex++;
            if (m_lastSortedIndex >= m_blocks.size() - 1) {
                // Sorting complete
                m_isComplete = true;
                if (statusCallback)
                    statusCallback("Selection Sort complete!");
                for (auto* block : m_blocks)
                    block->highlight(true, true); // green
                for (size_t i = 0; i < m_blocks.size(); ++i)
                    m_blocks[i]->moveToPosition(i);
                return false;
            }
            // Start a new pass: reset indices
            m_currentIndex = m_lastSortedIndex + 1;
            m_minIndex = m_lastSortedIndex;
            m_phase = HIGHLIGHT;
            return true;
        }
    } else { // ACTION
        // Update the minimum value
        if (m_blocks[m_currentIndex]->getValue() < m_blocks[m_minIndex]->getValue()) {
            m_minIndex = m_currentIndex;

            if (statusCallback)
                statusCallback(QString("New minimum found at index %1").arg(m_minIndex));
        }
        // Move to the next element
        m_currentIndex++;
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
    m_lastSortedIndex = 0;
    m_isComplete      = false;
    m_isSwapping      = false;
    m_comparisonCount = 0;
    m_swapCount       = 0;

    // For bubble and insertion, start comparing at index 0.
    // For selection, start at index 1 vs index 0.
    if (m_algorithm == SELECTION) {
        m_minIndex     = m_lastSortedIndex;       // = 0
        m_currentIndex = m_lastSortedIndex + 1;   // = 1
    } else {
        m_minIndex     = 0;
        m_currentIndex = 0;
    }

    // Insertion-sort uses these; bubble ignores them.
    m_outerIdx = 1;
    m_innerIdx = 1;

    // Reset highlights on all blocks
    for (PhysicsBlock* block : m_blocks) {
        block->highlight(false);
    }

    // Clear undo history
    history.clear();
}

void SortingController::saveState() {
    SortState snapshot;
    snapshot.phase = m_phase;
    snapshot.currentIndex = m_currentIndex;
    snapshot.lastSortedIndex = m_lastSortedIndex;
    snapshot.isComplete = m_isComplete;
    snapshot.isSwapping = m_isSwapping;
    snapshot.comparisonCount = m_comparisonCount;
    snapshot.swapCount = m_swapCount;
    snapshot.outerIdx = m_outerIdx;
    snapshot.innerIdx = m_innerIdx;
    snapshot.minIndex = m_minIndex;
    snapshot.algorithm = m_algorithm;

    // Save block order
    snapshot.blocks = m_blocks;

    history.push_back(snapshot);
}

bool SortingController::restoreState() {
    if (history.empty())
        return false;

    // Pop the last snapshot
    SortState prev = history.back();
    history.pop_back();

    // Restore all controller fields
    m_phase           = prev.phase;
    m_currentIndex    = prev.currentIndex;
    m_lastSortedIndex = prev.lastSortedIndex;
    m_isComplete      = prev.isComplete;
    m_isSwapping      = prev.isSwapping;
    m_comparisonCount = prev.comparisonCount;
    m_swapCount       = prev.swapCount;
    m_outerIdx        = prev.outerIdx;
    m_innerIdx        = prev.innerIdx;
    m_minIndex        = prev.minIndex;
    m_algorithm       = prev.algorithm;

    // Restore the block order
    m_blocks = prev.blocks;

    // Animate blocks back to their stored positions,
    // and clear any existing highlight on each:
    for (size_t i = 0; i < m_blocks.size(); ++i) {
        m_blocks[i]->moveToPosition(i);
        m_blocks[i]->highlight(false);
    }

    // If we've just popped the *last* snapshot (history is now empty),
    // that means we're back at the very beginning—no highlights at all.
    if (history.empty()) {
        // Nothing more to do; all blocks remain un-highlighted.
        return true;
    }

    // Otherwise, re-apply the correct two-block (or full-green) highlights
    reapplyHighlights();
    return true;
}

void SortingController::reapplyHighlights()
{
    // 1) Clear all highlights first
    for (auto *b : m_blocks)
        b->highlight(false);

    // 2) If we’re fully sorted, color them all green
    if (m_isComplete) {
        for (auto *b : m_blocks)
            b->highlight(true, true);
        return;
    }

    // 3) Otherwise, re-highlight based on algorithm & phase
    switch (m_algorithm) {
    case BUBBLE:
        if (m_phase == HIGHLIGHT && m_currentIndex + 1 < m_blocks.size()) {
            m_blocks[m_currentIndex]    ->highlight(true);
            m_blocks[m_currentIndex + 1]->highlight(true);
        }
        break;

    case INSERTION:
        if (m_phase == HIGHLIGHT && m_innerIdx < m_blocks.size()) {
            m_blocks[m_innerIdx]          ->highlight(true);
            if (m_innerIdx > 0)
                m_blocks[m_innerIdx - 1]->highlight(true);
        }
        break;

    case SELECTION:
        if (m_phase == HIGHLIGHT && m_currentIndex < m_blocks.size()) {
            m_blocks[m_currentIndex]->highlight(true);
            m_blocks[m_minIndex]   ->highlight(true);
        }
        break;
    }
}
