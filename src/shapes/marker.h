#ifndef MARKER_H
#define MARKER_H

#include "shape.h"
#include <QPoint>
#include <QVector>
#include <QColor>

class Marker : public Shape
{
public:
    Marker(const QPoint& startPoint, const QColor& color = Qt::black);
    ~Marker() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QVector<QPoint> points;
    bool complete = false;
    static constexpr int MARKER_WIDTH = 10;
};

#endif // MARKER_H
