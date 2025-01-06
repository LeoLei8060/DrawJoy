#ifndef MARKER_H
#define MARKER_H

#include "shape.h"
#include <QPoint>
#include <QColor>

class Marker : public Shape
{
public:
    Marker(const QPoint& startPoint);
    ~Marker() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QPoint startPoint;
    QPoint endPoint;
    static constexpr int MARKER_WIDTH = 20;  // 马克笔宽度
    static constexpr int MARKER_ALPHA = 50;  // 透明度 (0-255)
    const QColor MARKER_COLOR = QColor(255, 255, 0, MARKER_ALPHA);  // 黄色半透明
};

#endif // MARKER_H
