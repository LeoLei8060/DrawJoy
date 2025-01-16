#include "canvas.h"
#include "shapes/arrow.h"
#include "shapes/ellipse.h"
#include "shapes/freehand.h"
#include "shapes/highlighter.h"
#include "shapes/mosaic.h"
#include "shapes/polyline.h"
#include "shapes/rectangle.h"
#include "shapes/shape.h"
#include "shapes/text.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
    , m_currentMode(DrawMode::DrawPolyline)
    , m_mosaicType(0)
    , m_penColor(Qt::black) // 初始化画笔颜色为黑色
{
    setMouseTracking(true);
    setAttribute(Qt::WA_StaticContents);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);         // 允许接收键盘焦点
    setAttribute(Qt::WA_InputMethodEnabled); // 启用输入法
}

Canvas::~Canvas() {}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_currentShape) {
            if (m_currentMode == DrawMode::DrawText) {
                setCursor(Qt::IBeamCursor);
                setFocus(); // 获取键盘焦点

                // 获取点击位置的背景颜色
                QPixmap pixmap = grab(QRect(event->pos(), QSize(1, 1)));
                QImage  image = pixmap.toImage();
                QColor  backgroundColor = image.pixelColor(0, 0);

                createShape(event->pos());
                if (m_currentShape) {
                    auto textShape = static_cast<Text *>(m_currentShape.get());
                    textShape->updateBackgroundColor(backgroundColor);

                    // 获取文本框区域的背景
                    QRect bgRect = textShape->m_textRect;
                    textShape->setBackground(grab(bgRect));
                }
            } else if (m_currentMode == DrawMode::DrawMosaic) {
                m_currentShape = std::make_unique<Mosaic>(event->pos(),
                                                          static_cast<Mosaic::MosaicType>(
                                                              m_mosaicType),
                                                          m_penColor);
                if (auto *mosaic = static_cast<Mosaic *>(m_currentShape.get())) {
                    mosaic->setBackground(grab()); // 获取当前画布内容作为背景
                }
            } else {
                createShape(event->pos());
            }
        } else {
            if (m_currentMode == DrawMode::DrawPolyline) {
                // 对于折线，每次点击都添加新的点
                static_cast<Polyline *>(m_currentShape.get())->addPoint(event->pos());
                // TODO: delete
                //                static_cast<Polyline *>(currentShape.get())->m_points.append(event->pos());
            } else if (m_currentMode == DrawMode::DrawText) {
                // 如果点击在文本框外，结束编辑
                auto textShape = static_cast<Text *>(m_currentShape.get());
                if (!textShape->m_textRect.contains(event->pos())) {
                    if (textShape->isEmpty()) {
                        m_currentShape.reset(); // 如果没有输入文本，取消本次操作
                    } else {
                        textShape->finishEdit();
                        finishCurrentShape();
                    }
                    setCursor(Qt::ArrowCursor);
                    clearFocus();
                } else {
                    // 如果点击在文本框内，记录偏移量并更新背景
                    textShape->startMove(event->pos());
                    textShape->setBackground(grab(textShape->m_textRect));
                }
            } else {
                m_currentShape->updateShape(event->pos());
            }
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (m_currentShape) {
            if (m_currentMode == DrawMode::DrawPolyline) {
                auto tmpObj = static_cast<Polyline *>(m_currentShape.get());
                tmpObj->finishLine();
                if (tmpObj->isInvalid()) {
                    removeCurrentShape();
                    update();
                    return;
                }
            } /*else if (currentMode == DrawMode::DrawMosaic) {
                mosaicType = (mosaicType == 0) ? 1 : 0;
            }*/
            finishCurrentShape();
            update();
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (m_currentShape) {
        if (event->buttons() & Qt::LeftButton || m_currentMode == DrawMode::DrawPolyline) {
            if (m_currentMode == DrawMode::DrawText) {
                auto textShape = static_cast<Text *>(m_currentShape.get());
                textShape->updateShape(event->pos());
                // 更新移动后的背景
                textShape->setBackground(grab(textShape->m_textRect));
            } else {
                m_currentShape->updateShape(event->pos());
            }
            update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_currentShape) {
        if (m_currentMode != DrawMode::DrawPolyline && m_currentMode != DrawMode::DrawText) {
            m_currentShape->updateShape(event->pos());
            // 对于椭圆，检查起点和终点是否重合
            if (m_currentMode == DrawMode::DrawEllipse) {
                auto ellipse = static_cast<Ellipse *>(m_currentShape.get());
                if (ellipse->m_startPoint == ellipse->m_endPoint) {
                    m_currentShape.reset();
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
    if (m_currentShape && m_currentMode == DrawMode::DrawText) {
        auto textShape = static_cast<Text *>(m_currentShape.get());

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
    if (m_currentShape && m_currentMode == DrawMode::DrawText) {
        auto textShape = static_cast<Text *>(m_currentShape.get());

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
    if (m_currentShape && m_currentMode == DrawMode::DrawText) {
        auto         textShape = static_cast<Text *>(m_currentShape.get());
        QFontMetrics metrics(textShape->m_font);
        int          cursorX = textShape->m_textRect.left() + textShape->m_padding;
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
            return QRect(cursorX, textShape->m_textRect.top(), 1, textShape->m_textRect.height());
        case Qt::ImFont:
            return QVariant::fromValue(textShape->m_font);
        case Qt::ImCurrentSelection:
            return QString();
        case Qt::ImMaximumTextLength:
            return 65535;
        case Qt::ImHints:
            return int(Qt::ImhMultiLine | Qt::ImhPreferNumbers | Qt::ImhPreferUppercase
                       | Qt::ImhPreferLowercase | Qt::ImhNoPredictiveText);
        default:
            break;
        }
    }
    return QWidget::inputMethodQuery(query);
}

void Canvas::createShape(const QPoint &pos)
{
    switch (m_currentMode) {
    case DrawMode::DrawPolyline:
        m_currentShape = std::make_unique<Polyline>(pos, m_penColor);
        break;
    case DrawMode::DrawEllipse:
        m_currentShape = std::make_unique<Ellipse>(pos, m_penColor);
        break;
    case DrawMode::DrawRectangle:
        m_currentShape = std::make_unique<Rectangle>(pos, m_penColor);
        break;
    case DrawMode::DrawArrow:
        m_currentShape = std::make_unique<Arrow>(pos, m_penColor);
        break;
    case DrawMode::DrawFreehand:
        m_currentShape = std::make_unique<Freehand>(pos, m_penColor);
        break;
    case DrawMode::DrawHighlighter:
        m_currentShape = std::make_unique<Highlighter>(pos, m_penColor);
        break;
    case DrawMode::DrawMosaic:
        m_currentShape = std::make_unique<Mosaic>(pos,
                                                  static_cast<Mosaic::MosaicType>(m_mosaicType),
                                                  m_penColor);
        break;
    case DrawMode::DrawText:
        m_currentShape = std::make_unique<Text>(pos, m_penColor);
        break;
    default:
        break;
    }
}

void Canvas::handleTextInput(const QPoint &pos)
{
    createShape(pos);
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw all completed shapes
    for (const auto &shape : m_shapes) {
        shape->draw(painter);
    }

    // Draw current shape if exists
    if (m_currentShape) {
        m_currentShape->draw(painter);
    }
}

void Canvas::setDrawMode(int mode)
{
    m_currentMode = static_cast<DrawMode>(mode);
    if (m_currentMode == DrawMode::DrawText) {
        setCursor(Qt::IBeamCursor);
    } else {
        setCursor(Qt::ArrowCursor);
        clearFocus(); // 切换到其他模式时清除焦点
    }

    if (m_currentShape) {
        if (m_currentMode == DrawMode::DrawText && dynamic_cast<Text *>(m_currentShape.get())) {
            // 如果当前正在编辑文本，不要重置
            return;
        }
        finishCurrentShape();
        update();
    }
}

void Canvas::setPenColor(const QColor &color)
{
    m_penColor = color;
    if (m_currentShape) {
        m_currentShape->setPenColor(color);
        update();
    }
}

void Canvas::undo()
{
    if (!m_shapes.empty()) {
        m_redoStack.push_back(std::move(m_shapes.back()));
        m_shapes.pop_back();
        update();
    }
}

void Canvas::redo()
{
    if (!m_redoStack.empty()) {
        m_shapes.push_back(std::move(m_redoStack.back()));
        m_redoStack.pop_back();
        update();
    }
}

void Canvas::finishCurrentShape()
{
    if (m_currentShape) {
        m_shapes.push_back(std::move(m_currentShape));
        m_redoStack.clear();
    }
}

void Canvas::removeCurrentShape()
{
    if (m_currentShape) {
        m_currentShape.reset();
        m_redoStack.clear();
    }
}
