#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <QPoint>
#include <QRect>

class Rectangle : public Shape
{
public:
    Rectangle(const QPoint& startPoint);
    ~Rectangle() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    QRect boundingRect() const;
};

#endif // RECTANGLE_H
