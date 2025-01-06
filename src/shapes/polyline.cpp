#include "polyline.h"

Polyline::Polyline(const QPoint& startPoint)
{
    points.append(startPoint);
    currentPoint = startPoint;
}

void Polyline::draw(QPainter& painter) const
{
    if (points.isEmpty()) return;

    QPen pen(Qt::black, 2);
    painter.setPen(pen);

    // 绘制已确定的线段
    for (int i = 0; i < points.size() - 1; ++i) {
        painter.drawLine(points[i], points[i + 1]);
    }

    // 绘制当前正在绘制的线段（鼠标移动时的预览）
    if (!complete && !points.isEmpty()) {
        painter.drawLine(points.last(), currentPoint);
    }
}

void Polyline::updateShape(const QPoint& pos)
{
    if (complete) return;
    currentPoint = pos;
}

bool Polyline::isComplete() const
{
    return complete;
}

Shape* Polyline::clone() const
{
    Polyline* newPolyline = new Polyline(points.first());
    newPolyline->points = points;
    newPolyline->currentPoint = currentPoint;
    newPolyline->complete = complete;
    return newPolyline;
}

void Polyline::finishLine()
{
    if (!complete && points.size() >= 2) {
        complete = true;
    }
}
