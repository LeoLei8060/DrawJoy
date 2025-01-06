#include "arrow.h"
#include <QtMath>

Arrow::Arrow(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
{
    endPoint = startPoint;
}

void Arrow::draw(QPainter& painter) const
{
    painter.setPen(QPen(penColor, 2));
    painter.setBrush(penColor);

    // 绘制主线
    painter.drawLine(start, endPoint);

    // 绘制箭头
    if (start != endPoint) {
        painter.drawPolygon(createArrowHead());
    }
}

void Arrow::updateShape(const QPoint& pos)
{
    if (!complete) {
        endPoint = pos;
    }
}

bool Arrow::isComplete() const
{
    return complete;
}

Shape* Arrow::clone() const
{
    auto* newShape = new Arrow(start, penColor);
    newShape->endPoint = endPoint;
    newShape->complete = complete;
    return newShape;
}

QPolygon Arrow::createArrowHead() const
{
    QPolygon arrowHead;
    QPointF direction(endPoint - start);
    qreal angle = qAtan2(-direction.y(), direction.x());

    // 计算箭头两个点的位置
    qreal arrowAngle1 = angle + qDegreesToRadians(ARROW_HEAD_ANGLE);
    qreal arrowAngle2 = angle - qDegreesToRadians(ARROW_HEAD_ANGLE);

    QPointF arrowPoint1(endPoint.x() - ARROW_HEAD_SIZE * qCos(arrowAngle1),
                       endPoint.y() + ARROW_HEAD_SIZE * qSin(arrowAngle1));
    QPointF arrowPoint2(endPoint.x() - ARROW_HEAD_SIZE * qCos(arrowAngle2),
                       endPoint.y() + ARROW_HEAD_SIZE * qSin(arrowAngle2));

    arrowHead << endPoint << arrowPoint1.toPoint() << arrowPoint2.toPoint();
    return arrowHead;
}
