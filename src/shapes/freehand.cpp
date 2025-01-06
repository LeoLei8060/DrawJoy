#include "freehand.h"

Freehand::Freehand(const QPoint& startPoint)
{
    points.append(startPoint);
    path.moveTo(startPoint);
}

void Freehand::draw(QPainter& painter) const
{
    QPen pen(Qt::black, PEN_WIDTH);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(path);
}

void Freehand::updateShape(const QPoint& pos)
{
    if (!complete) {
        points.append(pos);
        if (points.size() > 1) {
            // 使用二次贝塞尔曲线使线条更平滑
            const QPoint& p1 = points[points.size() - 2];
            const QPoint& p2 = points[points.size() - 1];
            QPoint mid = (p1 + p2) / 2;
            
            if (points.size() == 2) {
                path.moveTo(points.first());
            }
            path.quadTo(p1, mid);
        }
    }
}

bool Freehand::isComplete() const
{
    return complete;
}

Shape* Freehand::clone() const
{
    Freehand* newFreehand = new Freehand(points.first());
    newFreehand->points = points;
    newFreehand->path = path;
    newFreehand->complete = complete;
    return newFreehand;
}
