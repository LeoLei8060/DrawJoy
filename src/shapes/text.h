#ifndef TEXT_H
#define TEXT_H

#include "shape.h"
#include <QString>
#include <QRect>
#include <QFont>
#include <QColor>
#include <QPixmap>

class Canvas;  // 前向声明

class Text : public Shape
{
    friend class Canvas;  // 允许Canvas访问私有成员
public:
    Text(const QPoint& pos, const QColor& color = Qt::black);
    ~Text() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

    void setText(const QString& text);
    void appendText(const QString& text);
    void removeLastChar();
    void finishEdit();
    const QString& getText() const { return text; }
    bool isEmpty() const { return text.isEmpty(); }
    void startMove(const QPoint& pos);
    void updateBackgroundColor(const QColor& color);
    void setBackground(const QPixmap& bg) { background = bg; }
    void setFont(const QFont& font) { this->font = font; }

private:
    QPoint position;
    QString text;
    bool editing = true;
    QRect textRect;
    QFont font{"Microsoft YaHei", 12};  // 使用微软雅黑字体
    const int padding = 5;  // 输入框内边距
    QPoint moveOffset;  // 移动时的偏移量
    QColor backgroundColor;  // 背景颜色
    QPixmap background;     // 背景图像
    
    void updateTextRect();
    QColor getContrastColor(const QColor& color) const;  // 获取与指定颜色的对比色
    void drawDashedRect(QPainter& painter, const QRect& rect) const;  // 绘制虚线边框
};

#endif // TEXT_H
