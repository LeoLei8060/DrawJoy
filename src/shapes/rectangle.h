#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <QPoint>
#include <QColor>

class Rectangle : public Shape
{
public:
    Rectangle(const QPoint& startPoint, const QColor& color = Qt::black);
    ~Rectangle() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    QColor color;
    bool complete = false;
    QRect boundingRect() const;
};

#endif // RECTANGLE_H
