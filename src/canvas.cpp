#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "shapes/shape.h"
#include "shapes/polyline.h"
#include "shapes/ellipse.h"
#include "shapes/rectangle.h"
#include "shapes/arrow.h"
#include "shapes/freehand.h"
#include "shapes/marker.h"
#include "shapes/mosaic.h"
#include "shapes/text.h"

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , currentMode(DrawMode::DrawPolyline)
    , mosaicType(0)
    , penColor(Qt::black)  // 初始化画笔颜色为黑色
{
    setMouseTracking(true);
    setAttribute(Qt::WA_StaticContents);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);  // 允许接收键盘焦点
    setAttribute(Qt::WA_InputMethodEnabled);  // 启用输入法
}

Canvas::~Canvas()
{
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!currentShape) {
            if (currentMode == DrawMode::DrawText) {
                setCursor(Qt::IBeamCursor);
                setFocus();  // 获取键盘焦点
                
                // 获取点击位置的背景颜色
                QPixmap pixmap = grab(QRect(event->pos(), QSize(1, 1)));
                QImage image = pixmap.toImage();
                QColor backgroundColor = image.pixelColor(0, 0);
                
                createShape(event->pos());
                if (currentShape) {
                    auto textShape = static_cast<Text*>(currentShape.get());
                    textShape->updateBackgroundColor(backgroundColor);
                    
                    // 获取文本框区域的背景
                    QRect bgRect = textShape->textRect;
                    textShape->setBackground(grab(bgRect));
                }
            } else {
                createShape(event->pos());
            }
        } else {
            if (currentMode == DrawMode::DrawPolyline) {
                // 对于折线，每次点击都添加新的点
                static_cast<Polyline*>(currentShape.get())->points.append(event->pos());
            } else if (currentMode == DrawMode::DrawText) {
                // 如果点击在文本框外，结束编辑
                auto textShape = static_cast<Text*>(currentShape.get());
                if (!textShape->textRect.contains(event->pos())) {
                    if (textShape->isEmpty()) {
                        currentShape.reset();  // 如果没有输入文本，取消本次操作
                    } else {
                        textShape->finishEdit();
                        finishCurrentShape();
                    }
                    setCursor(Qt::ArrowCursor);
                    clearFocus();
                } else {
                    // 如果点击在文本框内，记录偏移量并更新背景
                    textShape->startMove(event->pos());
                    textShape->setBackground(grab(textShape->textRect));
                }
            } else {
                currentShape->updateShape(event->pos());
            }
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (currentShape) {
            if (currentMode == DrawMode::DrawPolyline) {
                static_cast<Polyline*>(currentShape.get())->finishLine();
                finishCurrentShape();
            } else if (currentMode == DrawMode::DrawMosaic) {
                mosaicType = (mosaicType == 0) ? 1 : 0;
            }
            update();
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (currentShape) {
        if (event->buttons() & Qt::LeftButton || currentMode == DrawMode::DrawPolyline) {
            if (currentMode == DrawMode::DrawText) {
                auto textShape = static_cast<Text*>(currentShape.get());
                textShape->updateShape(event->pos());
                // 更新移动后的背景
                textShape->setBackground(grab(textShape->textRect));
            } else {
                currentShape->updateShape(event->pos());
            }
            update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && currentShape) {
        if (currentMode != DrawMode::DrawPolyline && currentMode != DrawMode::DrawText) {
            currentShape->updateShape(event->pos());
            // 对于椭圆，检查起点和终点是否重合
            if (currentMode == DrawMode::DrawEllipse) {
                auto ellipse = static_cast<Ellipse*>(currentShape.get());
                if (ellipse->startPoint == ellipse->endPoint) {
                    currentShape.reset();
                    return;
                }
            }
            finishCurrentShape();
            update();
        }
    }
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (currentShape && currentMode == DrawMode::DrawText) {
        auto textShape = static_cast<Text*>(currentShape.get());
        
        if (event->key() == Qt::Key_Backspace) {
            textShape->removeLastChar();
            update();
        } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            if (!textShape->isEmpty()) {
                textShape->finishEdit();
                finishCurrentShape();
                setCursor(Qt::ArrowCursor);
                clearFocus();
            }
            update();
        } else {
            QString text = event->text();
            if (!text.isEmpty() && text[0].isPrint()) {
                textShape->appendText(text);
                update();
            }
        }
    }
    QWidget::keyPressEvent(event);
}

void Canvas::inputMethodEvent(QInputMethodEvent *event)
{
    if (currentShape && currentMode == DrawMode::DrawText) {
        auto textShape = static_cast<Text*>(currentShape.get());
        
        // 如果有提交的文本，直接添加到当前文本
        if (!event->commitString().isEmpty()) {
            textShape->appendText(event->commitString());
            update();
        }
    }
    QWidget::inputMethodEvent(event);
}

QVariant Canvas::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (currentShape && currentMode == DrawMode::DrawText) {
        auto textShape = static_cast<Text*>(currentShape.get());
        QFontMetrics metrics(textShape->font);
        int cursorX = textShape->textRect.left() + textShape->padding;
        if (!textShape->getText().isEmpty()) {
            cursorX += metrics.horizontalAdvance(textShape->getText());
        }
        
        switch (query) {
        case Qt::ImEnabled:
            return true;
        case Qt::ImCursorPosition:
            return textShape->getText().length();
        case Qt::ImAnchorPosition:
            return textShape->getText().length();
        case Qt::ImCursorRectangle:
            return QRect(cursorX, textShape->textRect.top(), 1, textShape->textRect.height());
        case Qt::ImFont:
            return QVariant::fromValue(textShape->font);
        case Qt::ImCurrentSelection:
            return QString();
        case Qt::ImMaximumTextLength:
            return 65535;
        case Qt::ImHints:
            return int(Qt::ImhMultiLine | Qt::ImhPreferNumbers | Qt::ImhPreferUppercase | 
                      Qt::ImhPreferLowercase | Qt::ImhNoPredictiveText);
        default:
            break;
        }
    }
    return QWidget::inputMethodQuery(query);
}

void Canvas::createShape(const QPoint& pos)
{
    switch (currentMode) {
    case DrawMode::DrawPolyline:
        currentShape = std::make_unique<Polyline>(pos, penColor);
        break;
    case DrawMode::DrawEllipse:
        currentShape = std::make_unique<Ellipse>(pos, penColor);
        break;
    case DrawMode::DrawRectangle:
        currentShape = std::make_unique<Rectangle>(pos, penColor);
        break;
    case DrawMode::DrawArrow:
        currentShape = std::make_unique<Arrow>(pos, penColor);
        break;
    case DrawMode::DrawFreehand:
        currentShape = std::make_unique<Freehand>(pos, penColor);
        break;
    case DrawMode::DrawMarker:
        currentShape = std::make_unique<Marker>(pos, penColor);
        break;
    case DrawMode::DrawMosaic:
        currentShape = std::make_unique<Mosaic>(pos, static_cast<Mosaic::MosaicType>(mosaicType), penColor);
        break;
    case DrawMode::DrawText:
        currentShape = std::make_unique<Text>(pos, penColor);
        break;
    default:
        break;
    }
}

void Canvas::handleTextInput(const QPoint& pos)
{
    createShape(pos);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw all completed shapes
    for (const auto& shape : shapes) {
        shape->draw(painter);
    }

    // Draw current shape if exists
    if (currentShape) {
        currentShape->draw(painter);
    }
}

void Canvas::setDrawMode(int mode)
{
    currentMode = static_cast<DrawMode>(mode);
    if (currentMode == DrawMode::DrawText) {
        setCursor(Qt::IBeamCursor);
    } else {
        setCursor(Qt::ArrowCursor);
        clearFocus();  // 切换到其他模式时清除焦点
    }
    
    if (currentShape) {
        if (currentMode == DrawMode::DrawText && 
            dynamic_cast<Text*>(currentShape.get())) {
            // 如果当前正在编辑文本，不要重置
            return;
        }
        finishCurrentShape();
        update();
    }
}

void Canvas::setPenColor(const QColor& color)
{
    penColor = color;
    if (currentShape) {
        currentShape->setPenColor(color);
        update();
    }
}

void Canvas::undo()
{
    if (!shapes.empty()) {
        redoStack.push_back(std::move(shapes.back()));
        shapes.pop_back();
        update();
    }
}

void Canvas::redo()
{
    if (!redoStack.empty()) {
        shapes.push_back(std::move(redoStack.back()));
        redoStack.pop_back();
        update();
    }
}

void Canvas::finishCurrentShape()
{
    if (currentShape) {
        shapes.push_back(std::move(currentShape));
        redoStack.clear();
    }
}
