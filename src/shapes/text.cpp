#include "text.h"
#include <QFontMetrics>

Text::Text(const QPoint& startPoint, const QColor& color)
    : Shape(startPoint, color)
    , position(startPoint)
    , backgroundColor(color)
    , font("Arial", 12)
{
    updateTextRect();
}

void Text::draw(QPainter& painter) const
{
    // 如果正在编辑，绘制虚线边框
    if (editing) {
        drawDashedRect(painter, textRect);
    }

    // 绘制背景
    if (backgroundColor.isValid()) {
        painter.fillRect(textRect, backgroundColor);
    }

    // 绘制文本
    if (!text.isEmpty()) {
        painter.setFont(font);
        painter.setPen(getContrastColor(backgroundColor));
        painter.drawText(textRect.adjusted(padding, padding, -padding, -padding),
                        Qt::AlignLeft | Qt::AlignVCenter,
                        text);
    }
}

void Text::drawDashedRect(QPainter& painter, const QRect& rect) const
{
    if (background.isNull()) return;

    QImage bgImage = background.toImage();
    
    // 绘制四条边
    for (int x = rect.left(); x <= rect.right(); ++x) {
        // 顶边
        if ((x - rect.left()) % 4 < 2) {  // 虚线模式：2像素显示，2像素隐藏
            QColor bgColor = bgImage.pixelColor(x - rect.left(), 0);
            painter.setPen(getContrastColor(bgColor));
            painter.drawPoint(x, rect.top());
        }
        // 底边
        if ((x - rect.left()) % 4 < 2) {
            QColor bgColor = bgImage.pixelColor(x - rect.left(), rect.height() - 1);
            painter.setPen(getContrastColor(bgColor));
            painter.drawPoint(x, rect.bottom());
        }
    }
    
    for (int y = rect.top(); y <= rect.bottom(); ++y) {
        // 左边
        if ((y - rect.top()) % 4 < 2) {
            QColor bgColor = bgImage.pixelColor(0, y - rect.top());
            painter.setPen(getContrastColor(bgColor));
            painter.drawPoint(rect.left(), y);
        }
        // 右边
        if ((y - rect.top()) % 4 < 2) {
            QColor bgColor = bgImage.pixelColor(rect.width() - 1, y - rect.top());
            painter.setPen(getContrastColor(bgColor));
            painter.drawPoint(rect.right(), y);
        }
    }
}

void Text::updateShape(const QPoint& pos)
{
    // 更新位置时考虑偏移量
    position = pos - moveOffset;
    updateTextRect();
}

void Text::startMove(const QPoint& pos)
{
    // 计算鼠标点击位置相对于输入框左上角的偏移
    moveOffset = pos - position;
}

bool Text::isComplete() const
{
    return !editing;
}

Shape* Text::clone() const
{
    auto* cloned = new Text(position, backgroundColor);
    cloned->text = text;
    cloned->editing = editing;
    cloned->textRect = textRect;
    cloned->font = font;
    cloned->background = background;
    return cloned;
}

void Text::setText(const QString& newText)
{
    text = newText;
    updateTextRect();
}

void Text::appendText(const QString& appendText)
{
    text += appendText;
    updateTextRect();
}

void Text::removeLastChar()
{
    if (!text.isEmpty()) {
        text.chop(1);
        updateTextRect();
    }
}

void Text::finishEdit()
{
    editing = false;
}

void Text::updateBackgroundColor(const QColor& color)
{
    backgroundColor = color;
}

QColor Text::getContrastColor(const QColor& color) const
{
    // 计算亮度
    int brightness = (color.red() * 299 + color.green() * 587 + color.blue() * 114) / 1000;
    return brightness > 128 ? Qt::black : Qt::white;
}

void Text::updateTextRect()
{
    QFontMetrics metrics(font);
    int width = std::max(100, metrics.horizontalAdvance(text) + 2 * padding);  // 最小宽度100
    int height = std::max(30, metrics.height() + 2 * padding);  // 最小高度30
    textRect = QRect(position, QSize(width, height));
}
