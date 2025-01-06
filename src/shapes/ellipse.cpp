#include "ellipse.h"

Ellipse::Ellipse(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
{
    endPoint = startPoint;
}

void Ellipse::draw(QPainter& painter) const
{
    painter.setPen(QPen(penColor, 2));
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
    auto* newShape = new Ellipse(start, penColor);
    newShape->endPoint = endPoint;
    newShape->complete = complete;
    return newShape;
}

QRect Ellipse::boundingRect() const
{
    return QRect(start, endPoint).normalized();
}
