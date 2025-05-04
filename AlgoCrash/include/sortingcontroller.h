/**
 * sortingcontroller.h
 *
 * This file defines the SortingController class which manages the execution
 * of sorting algorithms and tracks statistics about their performance.
 *
 * Checked by: Devin Gupta
 */
#ifndef SORTINGCONTROLLER_H
#define SORTINGCONTROLLER_H

#include <vector>
#include "physicsblock.h"

/**
 * SortingController
 *
 * Manages the execution of different sorting algorithms on a collection
 * of PhysicsBlock objects, tracking statistics and providing step-by-step
 * visualization.
 */
class SortingController {
public:
    /**
     * Constructor for the SortingController
     */
    SortingController();

    /**
     * Callback function to report status updates during sorting
     */
    std::function<void(const QString&)> statusCallback;

    /**
     * Enumeration of the supported sorting algorithms
     */
    enum Algorithm { BUBBLE, INSERTION, SELECTION };

    /**
     * Sets the current algorithm and resets the controller
     *
     * @param a The algorithm to use
     */
    void setAlgorithm(Algorithm a) { m_algorithm = a; reset(); }

    /**
     * Advances the current algorithm by one step
     *
     * @return False when sorting is complete, true otherwise
     */
    bool step();

    /**
     * Sets the blocks to be sorted
     *
     * @param blocks The vector of PhysicsBlock pointers
     */
    void setBlocks(std::vector<PhysicsBlock*>& blocks);

    /**
     * Performs one step of the bubble sort algorithm
     *
     * @return False when sorting is complete, true otherwise
     */
    bool bubbleSortStep();

    /**
     * Checks if sorting is complete
     *
     * @return True if sorting is complete, false otherwise
     */
    bool isSortingComplete() const;

    /**
     * Resets the controller to its initial state
     */
    void reset();

    /**
     * Gets the current comparison count
     *
     * @return The number of comparisons performed
     */
    int getComparisonCount() const { return m_comparisonCount; }

    /**
     * Gets the current swap count
     *
     * @return The number of swaps performed
     */
    int getSwapCount() const { return m_swapCount; }

    /**
     * Performs one step of the insertion sort algorithm
     *
     * @return False when sorting is complete, true otherwise
     */
    bool insertionSortStep();

    /**
     * Performs one step of the selection sort algorithm
     *
     * @return False when sorting is complete, true otherwise
     */
    bool selectionSortStep();
    bool restoreState();

    /**
    * Checks if the undo history is empty
    *
    * @return True if history is empty, false otherwise
    */
    bool isHistoryEmpty() const { return history.empty(); }

    /**
     * After a restore, re-highlight the two blocks that would
     * next be compared (or the current “min” in selection),
     * or show the sorted state if complete.
     */
    void reapplyHighlights();


private:
    /**
     * Phases of the sorting algorithms
     */
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

    /**
     * Performs a swap between two blocks and animates it
     *
     * @param index1 The index of the first block
     * @param index2 The index of the second block
     */
    void performSwap(size_t index1, size_t index2);

    /**
     * Saves the current state into history
     */
    void saveState();

    /**
     * Struct to represent one complete snapshot of the sort state
     */
    struct SortState {
        Phase phase;
        size_t currentIndex;
        size_t lastSortedIndex;
        bool isComplete;
        bool isSwapping;
        int comparisonCount;
        int swapCount;
        size_t outerIdx;
        size_t innerIdx;
        size_t minIndex;
        Algorithm algorithm;
        std::vector<PhysicsBlock*> blocks;
        std::vector<bool> wasActiveHighlight;
        std::vector<bool> wasSortedHighlight;
    };

    std::vector<SortState> history;
};

#endif // SORTINGCONTROLLER_H
