#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"
#include <QPoint>
#include <QColor>

class Canvas;  // 前向声明

class Ellipse : public Shape
{
    friend class Canvas;  // 允许Canvas访问私有成员
public:
    Ellipse(const QPoint& startPoint, const QColor& color = Qt::black);
    ~Ellipse() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    bool complete = false;
    QRect boundingRect() const;
};

#endif // ELLIPSE_H
