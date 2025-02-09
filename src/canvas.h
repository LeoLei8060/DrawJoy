#ifndef CANVAS_H
#define CANVAS_H

#include <memory>
#include <stack>
#include <vector>
#include <QColor> // 添加QColor头文件
#include <QInputDialog>
#include <QKeyEvent>
#include <QWidget>

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
        DrawHighlighter,
        DrawMosaic,
        DrawText
    };

    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void setDrawMode(int mode);
    void setPenColor(const QColor &color); // 添加设置画笔颜色的方法
    void undo();
    void redo();

protected:
    void     paintEvent(QPaintEvent *event) override;
    void     mousePressEvent(QMouseEvent *event) override;
    void     mouseMoveEvent(QMouseEvent *event) override;
    void     mouseReleaseEvent(QMouseEvent *event) override;
    void     keyPressEvent(QKeyEvent *event) override;
    void     inputMethodEvent(QInputMethodEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private:
    void createShape(const QPoint &pos);
    void handleTextInput(const QPoint &pos);
    void finishCurrentShape();
    void removeCurrentShape();

    DrawMode                            m_currentMode;
    std::unique_ptr<Shape>              m_currentShape;
    std::vector<std::unique_ptr<Shape>> m_shapes;
    std::vector<std::unique_ptr<Shape>> m_redoStack;
    int                                 m_mosaicType = 0; // 0 for Rectangle, 1 for Circle
    QString                             m_preeditString;  // 用于存储输入法的预编辑文本
    QColor                              m_penColor;       // 添加画笔颜色成员变量
};

#endif // CANVAS_H
