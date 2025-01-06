#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createToolBar();
    Canvas *canvas;
};

#endif // MAINWINDOW_H
