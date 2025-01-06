#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QPoint>
#include <QColor>

class Shape
{
public:
    Shape(const QPoint& startPoint, const QColor& color = Qt::black)
        : start(startPoint)
        , penColor(color)
    {}
    virtual ~Shape() = default;

    virtual void draw(QPainter& painter) const = 0;
    virtual void updateShape(const QPoint& pos) = 0;
    virtual bool isComplete() const { return true; }
    virtual Shape* clone() const = 0;

    void setPenColor(const QColor& color) { penColor = color; }
    QColor getPenColor() const { return penColor; }

protected:
    QPoint start;
    QColor penColor;
};

#endif // SHAPE_H
