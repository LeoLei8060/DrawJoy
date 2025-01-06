#ifndef MOSAIC_H
#define MOSAIC_H

#include "shape.h"
#include <QVector>
#include <QPoint>
#include <QRect>

class Mosaic : public Shape
{
public:
    enum class MosaicType {
        Circle,
        Rectangle
    };

    Mosaic(const QPoint& startPoint, MosaicType type = MosaicType::Rectangle);
    ~Mosaic() override = default;

    void draw(QPainter& painter) const override;
    void updateShape(const QPoint& pos) override;
    bool isComplete() const override;
    Shape* clone() const override;

private:
    QVector<QPoint> points;
    MosaicType type;
    static constexpr int MOSAIC_SIZE = 10;  // 马赛克大小
    static constexpr int MIN_DISTANCE = 5;  // 两个马赛克之间的最小距离

    void drawMosaicUnit(QPainter& painter, const QPoint& center) const;
    bool isTooClose(const QPoint& newPoint) const;
};

#endif // MOSAIC_H
