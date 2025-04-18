#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <vector>
#include <Box2D/Box2D.h>
#include "physicsblock.h"

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

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene;
    QTimer* simTimer;
    b2World* world;

    std::vector<PhysicsBlock*> blocks;

    void spawnInitialBlocks(const std::vector<int>& values);
};

#endif // MAINWINDOW_H
