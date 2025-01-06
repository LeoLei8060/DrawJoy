#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QColorDialog>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectColor();

private:
    void createToolBar();
    void updateColorButton();

    Canvas *canvas;
    QToolButton *colorButton;
    QColor currentColor;
};

#endif // MAINWINDOW_H
