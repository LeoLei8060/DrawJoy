#ifndef FREEHAND_H
#define FREEHAND_H

#include "shape.h"
#include <QVector>
#include <QPoint>
#include <QColor>

class Freehand : public Shape
{
public:
    Freehand(const QPoint& startPoint, const QColor& color = Qt::black);
    ~Freehand() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QVector<QPoint> points;
    QColor color;
    static constexpr int PEN_WIDTH = 2;
    bool complete = false;
};

#endif // FREEHAND_H
