#include "arrow.h"
#include <QtMath>

Arrow::Arrow(const QPoint& point)
    : startPoint(point)
    , endPoint(point)
{
}

void Arrow::draw(QPainter& painter) const
{
    QPen pen(Qt::black, 2);
    painter.setPen(pen);
    painter.setBrush(Qt::black);

    // 绘制箭头主干线
    painter.drawLine(startPoint, endPoint);

    // 绘制箭头头部
    if (startPoint != endPoint) {
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
    Arrow* newArrow = new Arrow(startPoint);
    newArrow->endPoint = endPoint;
    newArrow->complete = complete;
    return newArrow;
}

QPolygon Arrow::createArrowHead() const
{
    QPolygon arrowHead;
    
    // 计算箭头方向角度
    double angle = qAtan2(endPoint.y() - startPoint.y(), 
                         endPoint.x() - startPoint.x());
    
    // 计算箭头两个翼的端点
    double leftAngle = angle + qDegreesToRadians(ARROW_HEAD_ANGLE);
    double rightAngle = angle - qDegreesToRadians(ARROW_HEAD_ANGLE);
    
    QPoint leftPoint(
        endPoint.x() - ARROW_HEAD_SIZE * qCos(leftAngle),
        endPoint.y() - ARROW_HEAD_SIZE * qSin(leftAngle)
    );
    
    QPoint rightPoint(
        endPoint.x() - ARROW_HEAD_SIZE * qCos(rightAngle),
        endPoint.y() - ARROW_HEAD_SIZE * qSin(rightAngle)
    );
    
    arrowHead << endPoint << leftPoint << rightPoint;
    return arrowHead;
}
