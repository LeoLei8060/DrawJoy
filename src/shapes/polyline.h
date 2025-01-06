#ifndef POLYLINE_H
#define POLYLINE_H

#include "shape.h"
#include <QPoint>
#include <QVector>
#include <QColor>

class Canvas;  // 前向声明

class Polyline : public Shape
{
    friend class Canvas;  // 允许Canvas访问私有成员
public:
    Polyline(const QPoint& startPoint, const QColor& color = Qt::black);
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
    QColor color;
};

#endif // POLYLINE_H
