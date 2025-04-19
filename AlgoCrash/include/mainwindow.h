#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include <Box2D/Box2D.h>
#include "physicsblock.h"
#include "sortingcontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onStepButtonClicked();
    void onSortButtonClicked();
    void onResetButtonClicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;
    QTimer* simTimer;
    QTimer* sortTimer;
    b2World* world;

    std::vector<PhysicsBlock*> blocks;
    SortingController sortController;

    void spawnInitialBlocks(const std::vector<int>& values);
    void updateStatistics();
};

#endif // MAINWINDOW_H
