#include "mosaic.h"
#include <QtMath>

Mosaic::Mosaic(const QPoint& startPoint, MosaicType type)
    : type(type)
{
    points.append(startPoint);
}

void Mosaic::draw(QPainter& painter) const
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(128, 128, 128));  // 使用灰色填充

    for (const QPoint& point : points) {
        drawMosaicUnit(painter, point);
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
    Mosaic* newMosaic = new Mosaic(points.first(), type);
    newMosaic->points = points;
    newMosaic->complete = complete;
    return newMosaic;
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
