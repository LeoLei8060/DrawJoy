#include "rectangle.h"

Rectangle::Rectangle(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
{
    endPoint = startPoint;
}

void Rectangle::draw(QPainter& painter) const
{
    painter.setPen(QPen(penColor, 2));
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
    auto* newShape = new Rectangle(start, penColor);
    newShape->endPoint = endPoint;
    newShape->complete = complete;
    return newShape;
}

QRect Rectangle::boundingRect() const
{
    return QRect(startPoint, endPoint).normalized();
}
