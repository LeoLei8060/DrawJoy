#include "rectangle.h"

Rectangle::Rectangle(const QPoint& point)
    : startPoint(point)
    , endPoint(point)
{
}

void Rectangle::draw(QPainter& painter) const
{
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(boundingRect());
}

void Rectangle::updateShape(const QPoint& pos)
{
    if (!complete) {
        endPoint = pos;
    }
}

bool Rectangle::isComplete() const
{
    return complete;
}

Shape* Rectangle::clone() const
{
    Rectangle* newRect = new Rectangle(startPoint);
    newRect->endPoint = endPoint;
    newRect->complete = complete;
    return newRect;
}

QRect Rectangle::boundingRect() const
{
    return QRect(startPoint, endPoint).normalized();
}
