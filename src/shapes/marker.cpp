#include "marker.h"
#include <QtMath>

Marker::Marker(const QPoint& point)
    : startPoint(point)
    , endPoint(point)
{
}

void Marker::draw(QPainter& painter) const
{
    if (startPoint == endPoint) return;

    // 保存当前画笔设置
    painter.save();

    // 设置画笔
    QPen pen(MARKER_COLOR, MARKER_WIDTH);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);

    // 启用抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制马克笔线条
    painter.drawLine(startPoint, endPoint);

    // 恢复之前的画笔设置
    painter.restore();
}

void Marker::updateShape(const QPoint& pos)
{
    if (!complete) {
        endPoint = pos;
    }
}

bool Marker::isComplete() const
{
    return complete;
}

Shape* Marker::clone() const
{
    Marker* newMarker = new Marker(startPoint);
    newMarker->endPoint = endPoint;
    newMarker->complete = complete;
    return newMarker;
}
