#include "marker.h"
#include <QtMath>

Marker::Marker(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
{
    points.append(startPoint);
}

void Marker::draw(QPainter& painter) const
{
    QColor markerColor = penColor;
    markerColor.setAlpha(50);  // 设置透明度
    painter.setPen(Qt::NoPen);
    painter.setBrush(markerColor);

    if (points.size() > 1) {
        for (int i = 1; i < points.size(); ++i) {
            const QPoint& p1 = points[i-1];
            const QPoint& p2 = points[i];
            
            // 计算线段的方向向量
            QPointF dir = QPointF(p2 - p1);
            qreal length = qSqrt(dir.x() * dir.x() + dir.y() * dir.y());
            if (length > 0) {
                dir /= length;
                
                // 计算垂直于方向的向量
                QPointF normal(-dir.y(), dir.x());
                normal *= MARKER_WIDTH / 2.0;

                // 构建多边形的四个顶点
                QPointF points[4] = {
                    QPointF(p1) + normal,
                    QPointF(p1) - normal,
                    QPointF(p2) - normal,
                    QPointF(p2) + normal
                };

                painter.drawPolygon(points, 4);
            }
        }
    }
}

void Marker::updateShape(const QPoint& pos)
{
    if (!complete) {
        points.append(pos);
    }
}

bool Marker::isComplete() const
{
    return complete;
}

Shape* Marker::clone() const
{
    auto* newShape = new Marker(start, penColor);
    newShape->points = points;
    newShape->complete = complete;
    return newShape;
}
