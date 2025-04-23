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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /** Keep the artwork letter‑boxed when the user resizes the window. */
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onStepButtonClicked();         //!< Execute exactly one controller->step()
    void onSortButtonClicked();         //!< Toggle continuous sorting
    void onResetButtonClicked();        //!< Drop old blocks and start over
    void onCustomizeButtonClicked();    //!< Prompt user for custom list

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;
    QTimer* simTimer;                   //!< 60 FPS physics loop
    QTimer* sortTimer;                  //!< Drives SortingController
    b2World* world;

    std::vector<PhysicsBlock*> blocks;  //!< Active tiles
    SortingController sortController;   //!< Algorithm driver

    /* Helper routines */
    void spawnInitialBlocks(const std::vector<int>& values);
    void updateStatistics();

    QLabel* sortedLabel;
};

#endif // MAINWINDOW_H
