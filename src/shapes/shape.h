#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QPoint>

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw(QPainter& painter) const = 0;
    virtual void updateShape(const QPoint& pos) = 0;
    virtual bool isComplete() const = 0;
    virtual Shape* clone() const = 0;

protected:
    bool complete = false;
};

#endif // SHAPE_H
