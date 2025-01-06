#include "freehand.h"

Freehand::Freehand(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
{
    points.append(startPoint);
}

void Freehand::draw(QPainter& painter) const
{
    painter.setPen(QPen(penColor, 2));
    if (points.size() > 1) {
        for (int i = 1; i < points.size(); ++i) {
            painter.drawLine(points[i-1], points[i]);
        }
    }
}

void Freehand::updateShape(const QPoint& pos)
{
    if (!complete) {
        points.append(pos);
    }
}

bool Freehand::isComplete() const
{
    return complete;
}

Shape* Freehand::clone() const
{
    auto* newShape = new Freehand(start, penColor);
    newShape->points = points;
    newShape->complete = complete;
    return newShape;
}
