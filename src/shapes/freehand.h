#ifndef FREEHAND_H
#define FREEHAND_H

#include "shape.h"
#include <QVector>
#include <QPoint>
#include <QPainterPath>

class Freehand : public Shape
{
public:
    Freehand(const QPoint& startPoint);
    ~Freehand() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QVector<QPoint> points;
    QPainterPath path;
    static constexpr int PEN_WIDTH = 2;
};

#endif // FREEHAND_H
