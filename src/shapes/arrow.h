#ifndef ARROW_H
#define ARROW_H

#include "shape.h"
#include <QPoint>
#include <QColor>
#include <QPolygon>

class Arrow : public Shape
{
public:
    Arrow(const QPoint& startPoint, const QColor& color = Qt::black);
    ~Arrow() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    QColor color;
    bool complete = false;
    QPolygon createArrowHead() const;
    static constexpr double ARROW_HEAD_SIZE = 20.0; // 箭头大小
    static constexpr double ARROW_HEAD_ANGLE = 30.0; // 箭头角度（度）
};

#endif // ARROW_H
