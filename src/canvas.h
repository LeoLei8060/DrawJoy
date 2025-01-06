#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <stack>
#include <vector>
#include <memory>
#include <QInputDialog>
#include <QKeyEvent>

// 前向声明
class Shape;
class Polyline;
class Ellipse;
class Rectangle;
class Arrow;
class Freehand;
class Marker;
class Mosaic;
class Text;

class Canvas : public QWidget
{
    Q_OBJECT

public:
    enum DrawMode {
        DrawPolyline,
        DrawEllipse,
        DrawRectangle,
        DrawArrow,
        DrawFreehand,
        DrawMarker,
        DrawMosaic,
        DrawText
    };

    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void setDrawMode(int mode);
    void undo();
    void redo();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private:
    void createShape(const QPoint& pos);
    void finishCurrentShape();
    void handleTextInput(const QPoint& pos);

    DrawMode currentMode;
    std::unique_ptr<Shape> currentShape;
    std::vector<std::unique_ptr<Shape>> shapes;
    std::vector<std::unique_ptr<Shape>> redoStack;
    int mosaicType = 0; // 0 for Rectangle, 1 for Circle
    QString preeditString;  // 用于存储输入法的预编辑文本
};

#endif // CANVAS_H
