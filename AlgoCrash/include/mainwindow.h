/**
 * mainwindow.h
 *
 * This file defines the MainWindow class which serves as the application's
 * main interface, managing the visualization and user interaction.
 *
 * Checked by: Jungbin Moon
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include <Box2D/Box2D.h>
#include "physicsblock.h"
#include "sortingcontroller.h"
#include <QLabel>
#include <random>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Qt top‑level window tying together UI widgets, physics, and sorting.
 *
 *  Responsibilities:
 *  * Hosts a QGraphicsScene containing @ref PhysicsBlock items.
 *  * Runs a 60 FPS timer to step the Box2D world and update graphics.
 *  * Owns a second timer that drives the @ref SortingController at a user‑set
 *    pace (via a speed slider).
 *  * Responds to UI buttons: *Step*, *Start/Pause*, *Reset*, *Customize*.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor for the MainWindow
     *
     * @param parent The parent widget, defaults to nullptr
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /**
     * Destructor for the MainWindow
     */
    ~MainWindow();

protected:
    /**
     * Handles window resize events to maintain proper visualization
     *
     * @param event The resize event information
     */
    void resizeEvent(QResizeEvent* event) override;

private slots:
    /**
     * Handles Step button clicks to advance the sorting algorithm one step
     */
    void onStepForwardButtonClicked();

    /**
     * Handles Sort button clicks to start/pause the automatic sorting
     */
    void onSortButtonClicked();

    /**
     * Handles Reset button clicks to reset the simulation
     */
    void onResetButtonClicked();

    /**
     * Handles Customize button clicks to allow custom data input
     */
    void onCustomizeButtonClicked();
    void onStepBackwardButtonClicked();


private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;
    QTimer* simTimer;                   //!< 60 FPS physics loop
    QTimer* sortTimer;                  //!< Drives SortingController
    b2World* world;

    std::vector<PhysicsBlock*> blocks;  //!< Active tiles
    SortingController sortController;   //!< Algorithm driver

    /**
     * Creates the initial blocks with given values
     *
     * @param values The values for the blocks to be created
     */
    void spawnInitialBlocks(const std::vector<int>& values);

    /**
     * Updates the statistics display (comparisons and swaps)
     */
    void updateStatistics();

    QLabel* sortedLabel;
    bool areBlocksSettled() const;
    void updateButtonStates();
    std::mt19937                        m_rng;
    std::uniform_real_distribution<float> m_offsetDist;
    std::uniform_real_distribution<float> m_heightDist;
};

#endif // MAINWINDOW_H
