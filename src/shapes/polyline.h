#ifndef POLYLINE_H
#define POLYLINE_H

#include "shape.h"
#include <QColor>
#include <QPoint>
#include <QVector>

class Canvas; // 前向声明

class Polyline : public Shape
{
    friend class Canvas; // 允许Canvas访问私有成员
public:
    Polyline(const QPoint &startPoint, const QColor &color = Qt::black);
    ~Polyline() override = default;

    void   draw(QPainter &painter) const override;
    void   updateShape(const QPoint &pos) override;
    bool   isComplete() const override;
    Shape *clone() const override;
    void   finishLine();

    void addPoint(const QPoint &point);        // 添加新的点
    void setPreviewPoint(const QPoint &point); // 设置预览点

    bool isInvalid() const { return m_isInvalid; }

private:
    QVector<QPoint> m_points;             // 已确认的点
    QPoint          m_previewPoint;       // 预览点（跟随鼠标移动）
    bool            m_hasPreview = false; // 是否显示预览线
    bool            m_complete = false;
    bool            m_isInvalid = false;
    QColor          m_color;
};

#endif // POLYLINE_H
