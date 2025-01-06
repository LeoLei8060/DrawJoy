#include "mosaic.h"
#include <QtMath>

Mosaic::Mosaic(const QPoint& startPoint, MosaicType type, const QColor& color)
    : Shape(startPoint, color)
    , type(type)
{
    points.append(startPoint);
}

void Mosaic::draw(QPainter& painter) const
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(penColor);

    for (const QPoint& point : points) {
        QRect rect(point.x() - MOSAIC_SIZE/2, 
                  point.y() - MOSAIC_SIZE/2,
                  MOSAIC_SIZE, MOSAIC_SIZE);

        if (type == MosaicType::Rectangle) {
            painter.drawRect(rect);
        } else {
            painter.drawEllipse(rect);
        }
    }
}

void Mosaic::updateShape(const QPoint& pos)
{
    if (!complete && !isTooClose(pos)) {
        points.append(pos);
    }
}

bool Mosaic::isComplete() const
{
    return complete;
}

Shape* Mosaic::clone() const
{
    auto* newShape = new Mosaic(start, type, penColor);
    newShape->points = points;
    newShape->complete = complete;
    return newShape;
}

void Mosaic::drawMosaicUnit(QPainter& painter, const QPoint& center) const
{
    if (type == MosaicType::Circle) {
        painter.drawEllipse(center, MOSAIC_SIZE/2, MOSAIC_SIZE/2);
    } else {  // Rectangle
        QRect rect(
            center.x() - MOSAIC_SIZE/2,
            center.y() - MOSAIC_SIZE/2,
            MOSAIC_SIZE,
            MOSAIC_SIZE
        );
        painter.drawRect(rect);
    }
}

bool Mosaic::isTooClose(const QPoint& newPoint) const
{
    // 检查新点是否离已有的点太近
    for (const QPoint& point : points) {
        int dx = newPoint.x() - point.x();
        int dy = newPoint.y() - point.y();
        if (qSqrt(dx*dx + dy*dy) < MIN_DISTANCE) {
            return true;
        }
    }
    return false;
}
