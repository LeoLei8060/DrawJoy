#ifndef POLYLINE_H
#define POLYLINE_H

#include "shape.h"
#include <QPoint>
#include <QVector>

class Canvas;  // 前向声明

class Polyline : public Shape
{
    friend class Canvas;  // 允许Canvas访问私有成员
public:
    Polyline(const QPoint& startPoint);
    ~Polyline() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;
    void finishLine();

private:
    QVector<QPoint> points;
    QPoint currentPoint;
    bool complete = false;
};

#endif // POLYLINE_H
