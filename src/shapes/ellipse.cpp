#include "ellipse.h"

Ellipse::Ellipse(const QPoint& point)
    : startPoint(point)
    , endPoint(point)
{
}

void Ellipse::draw(QPainter& painter) const
{
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(boundingRect());
}

void Ellipse::updateShape(const QPoint& pos)
{
    if (!complete) {
        endPoint = pos;
    }
}

bool Ellipse::isComplete() const
{
    return complete;
}

Shape* Ellipse::clone() const
{
    Ellipse* newEllipse = new Ellipse(startPoint);
    newEllipse->endPoint = endPoint;
    newEllipse->complete = complete;
    return newEllipse;
}

QRect Ellipse::boundingRect() const
{
    return QRect(startPoint, endPoint).normalized();
}
