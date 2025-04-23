#ifndef SORTINGCONTROLLER_H
#define SORTINGCONTROLLER_H

#include <vector>
#include "physicsblock.h"

/*!
 * \class SortingController
 * \brief A controoler that drives step-by-step execution of several sorting algorithms.
 *
 * The controller is intentionally stateless w.r.t. the UI.
 * This helps to manipulates the PhysicsBlock objects directly
 * and also reports status via a std::function callback so that
 * the MainWindow can keep mirroring the progress in Qt widgets.
 */
class SortingController {
public:
    SortingController();

    /*! An optional callback for the UI status updates. */
    std::function<void(const QString&)> statusCallback;

    /*! ── Algorithm handling ───────────────────────── */
    enum Algorithm { BUBBLE, INSERTION, SELECTION };

    /*! Set up Algorithm user needed and rewind its internal state. */
    void setAlgorithm(Algorithm a) { m_algorithm = a; reset(); }
    bool step();                 // generic dispatcher

    /*! Provide the blocks to be sorted (does **not** take ownership). */
    void setBlocks(std::vector<PhysicsBlock*>& blocks);

    /*! True when the current run has completed. */
    bool isSortingComplete() const;

    /*! Clear all the counters and internal indices (called from MainWindow). */
    void reset();

    /*！ ---------- Metrics ---------- */
    int getComparisonCount() const { return m_comparisonCount; }
    int getSwapCount() const { return m_swapCount; }

    /*！ Per‑algorithm helpers */
    bool bubbleSortStep();
    bool insertionSortStep();
    bool selectionSortStep();

private:
    /* ---------- Internal state ---------- */
    enum Phase { HIGHLIGHT, ACTION };
    Phase m_phase;
    std::vector<PhysicsBlock*> m_blocks;
    size_t m_currentIndex;
    size_t m_lastSortedIndex;
    bool m_isComplete;
    bool m_isSwapping;
    int m_comparisonCount;
    int m_swapCount;

    /* Insertion‑sort bookkeeping */
    size_t m_outerIdx;
    size_t m_innerIdx;

    /* Selection-sort bookkeeping */
    size_t m_minIndex;

    Algorithm m_algorithm = BUBBLE;

    /*！ Per‑algorithm helpers */
    void performSwap(size_t index1, size_t index2);
};

#endif // SORTINGCONTROLLER_H
