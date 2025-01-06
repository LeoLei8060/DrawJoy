#include "mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建画布
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    // 创建工具栏
    createToolBar();

    // 设置窗口属性
    resize(800, 600);
    setWindowTitle(tr("DrawJoy"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar(tr("工具栏"));
    toolBar->setMovable(false);

    // 创建按钮组以实现互斥
    QButtonGroup *drawButtonGroup = new QButtonGroup(this);
    drawButtonGroup->setExclusive(true);  // 设置为互斥

    // 创建绘图工具按钮
    auto createToolButton = [this, toolBar, drawButtonGroup](const QString &iconPath, const QString &tip, int drawMode) {
        auto *button = new QToolButton;  // 先创建按钮
        button->setParent(toolBar);      // 然后设置父对象
        button->setIcon(QIcon(iconPath));
        button->setToolTip(tip);
        button->setCheckable(true);  // 设置为可选中状态
        toolBar->addWidget(button);
        drawButtonGroup->addButton(button);  // 添加到按钮组

        // 连接信号槽
        connect(button, &QToolButton::clicked, this, [this, drawMode]() {
            canvas->setDrawMode(drawMode);
        });

        return button;
    };

    // 创建各种绘图工具按钮
    auto polylineButton = createToolButton(":/icons/polyline.svg", tr("折线"), Canvas::DrawPolyline);
    auto ellipseButton = createToolButton(":/icons/ellipse.svg", tr("椭圆"), Canvas::DrawEllipse);
    auto rectangleButton = createToolButton(":/icons/rectangle.svg", tr("矩形"), Canvas::DrawRectangle);
    auto arrowButton = createToolButton(":/icons/arrow.svg", tr("箭头"), Canvas::DrawArrow);
    auto freehandButton = createToolButton(":/icons/freehand.svg", tr("自由绘制"), Canvas::DrawFreehand);
    auto markerButton = createToolButton(":/icons/marker.svg", tr("马克笔"), Canvas::DrawMarker);
    auto mosaicButton = createToolButton(":/icons/mosaic.svg", tr("马赛克"), Canvas::DrawMosaic);
    auto textButton = createToolButton(":/icons/text.svg", tr("文本"), Canvas::DrawText);

    // 默认选中折线按钮
    polylineButton->setChecked(true);

    // 添加分隔符
    toolBar->addSeparator();

    // 添加撤销和重做按钮（这些不需要互斥）
    auto *undoButton = new QToolButton;
    undoButton->setParent(toolBar);
    undoButton->setIcon(QIcon(":/icons/undo.svg"));
    undoButton->setToolTip(tr("撤销"));
    toolBar->addWidget(undoButton);
    connect(undoButton, &QToolButton::clicked, canvas, &Canvas::undo);

    auto *redoButton = new QToolButton;
    redoButton->setParent(toolBar);
    redoButton->setIcon(QIcon(":/icons/redo.svg"));
    redoButton->setToolTip(tr("重做"));
    toolBar->addWidget(redoButton);
    connect(redoButton, &QToolButton::clicked, canvas, &Canvas::redo);
}
