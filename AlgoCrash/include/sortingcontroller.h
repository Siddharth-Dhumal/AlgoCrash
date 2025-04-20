#ifndef SORTINGCONTROLLER_H
#define SORTINGCONTROLLER_H

#include <vector>
#include "physicsblock.h"

class SortingController {
public:
    SortingController();

    /* ── Algorithm handling ───────────────────────── */
    enum Algorithm { BUBBLE, INSERTION, SELECTION };
    void setAlgorithm(Algorithm a) { m_algorithm = a; reset(); }
    bool step();                 // generic dispatcher

    void setBlocks(std::vector<PhysicsBlock*>& blocks);
    bool bubbleSortStep(); // Performs one step of bubble sort, returns false when complete
    bool isSortingComplete() const;
    void reset();

    int getComparisonCount() const { return m_comparisonCount; }
    int getSwapCount() const { return m_swapCount; }

    bool insertionSortStep();
    bool selectionSortStep();

private:
    enum Phase { HIGHLIGHT, ACTION };
    Phase m_phase;
    std::vector<PhysicsBlock*> m_blocks;
    size_t m_currentIndex;
    size_t m_lastSortedIndex;
    bool m_isComplete;
    bool m_isSwapping;
    int m_comparisonCount;
    int m_swapCount;

    /* insertion‑sort bookkeeping */
    size_t m_outerIdx;
    size_t m_innerIdx;

    /* selection-sort bookkeeping */
    size_t m_minIndex;

    Algorithm m_algorithm = BUBBLE;

    void performSwap(size_t index1, size_t index2);
};

#endif // SORTINGCONTROLLER_H
