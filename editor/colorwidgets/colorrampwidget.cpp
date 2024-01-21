#include "colorrampwidget.h"
#include "../colorutility.h"
#include <QFontMetrics>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QtCore/qnamespace.h>

class ColorRampWidgetPrivate
{
public:
    ColorRampWidgetPrivate(ColorRampWidget *ptr) : q(ptr) {}
    ColorRampWidget *q;
    bool isDragging = false;
    ColorRampWidget::Orientation orientation;
    int margin = 4;
    bool showFrame = false;
    QPolygonF topTriangle;
    QPolygonF bottomTriangle;

    void setColorFromPoint(QPointF point);
};

/* ----------------------------- ColorRampWidget ---------------------------- */

ColorRampWidget::ColorRampWidget(QWidget *parent, ColorComponent component,
                                 Orientation orientation)
    : ColorWidget(parent, component), d(new ColorRampWidgetPrivate(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setOrientation(orientation);
    setMarkerSize(5);
}

ColorRampWidget::~ColorRampWidget()
{
    delete d;
    d = nullptr;
}

QSize ColorRampWidget::sizeHint() const
{
    if (d->orientation == ColorRampWidget::Horizontal)
    {
        return QSize(ColorUtility::UI_SCALE_FACTOR *
                             fontMetrics().horizontalAdvance('X') * 22,
                     ColorUtility::UI_SCALE_FACTOR * fontMetrics().height() *
                             1.3);
    }
    else
    {
        return QSize(ColorUtility::UI_SCALE_FACTOR * fontMetrics().height() *
                             1.3,
                     ColorUtility::UI_SCALE_FACTOR *
                             fontMetrics().horizontalAdvance('X') * 22);
    }
}

void ColorRampWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    if (d->showFrame)
    {
        QStyleOptionFrame option;
        option.initFrom(this);
        option.state = hasFocus() ? QStyle::State_KeyboardFocusChange
                                  : QStyle::State_None;
        style()->drawPrimitive(QStyle::PE_Frame, &option, &painter);
    }

    if (hasFocus())
    {
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.state = QStyle::State_KeyboardFocusChange;
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter);
    }

    if (mComponent != ColorWidget::Alpha)
    {
        const int maxValue =
                (d->orientation == ColorRampWidget::Horizontal ? width()
                                                               : height()) -
                1 - 2 * d->margin;
        QColor color = QColor(mCurrentColor);
        color.setAlpha(255);
        QPen pen;
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        for (int c = 0; c <= maxValue; ++c)
        {
            int colorVal = static_cast<int>(componentRange() *
                                            static_cast<double>(c) / maxValue);
            if (d->orientation == ColorRampWidget::Vertical)
            {
                colorVal = componentRange() - colorVal;
            }
            alterColor(color, colorVal, mComponent);
            if (color.hue() < 0)
            {
                color.setHsv(hue(), color.saturation(), color.value());
            }
            pen.setColor(color);
            painter.setPen(pen);
            if (d->orientation == ColorRampWidget::Horizontal)
            {
                painter.drawLine(QLineF(c + d->margin, d->margin, c + d->margin,
                                        height() - d->margin - 1));
            }
            else
            {
                painter.drawLine(QLineF(d->margin, c + d->margin,
                                        width() - d->margin - 1,
                                        c + d->margin));
            }
        }
    }
    else
    {
        const QBrush checkBrush = QBrush(transparentBackground());
        painter.setBrush(checkBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(QRectF(d->margin, d->margin,
                                width() - 2 * d->margin - 1,
                                height() - 2 * d->margin - 1));
        QLinearGradient colorGrad;
        if (d->orientation == ColorRampWidget::Horizontal)
        {
            colorGrad =
                    QLinearGradient(d->margin, 0, width() - d->margin - 1, 0);
        }
        else
        {
            colorGrad =
                    QLinearGradient(0, d->margin, 0, height() - d->margin - 1);
        }
        QColor transparent = QColor(mCurrentColor);
        transparent.setAlpha(0);
        colorGrad.setColorAt(0, transparent);
        QColor opaque = QColor(mCurrentColor);
        opaque.setAlpha(255);
        colorGrad.setColorAt(1, opaque);
        const QBrush colorBrush = QBrush(colorGrad);
        painter.setBrush(colorBrush);
        painter.drawRect(QRectF(d->margin, d->margin,
                                width() - 2 * d->margin - 1,
                                height() - 2 * d->margin - 1));
    }

    if (d->orientation == ColorRampWidget::Horizontal)
    {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(Qt::black));
        painter.setPen(Qt::NoPen);
        painter.translate(
                d->margin + (width() - 2 * d->margin) *
                                    static_cast<double>(componentValue()) /
                                    componentRange(),
                d->margin - 1);
        painter.drawPolygon(d->topTriangle);
        painter.translate(0, height() - d->margin - 2);
        painter.setBrush(QBrush(Qt::white));
        painter.drawPolygon(d->bottomTriangle);
        painter.end();
    }
    else
    {
        const double yPos = d->margin + (height() - 2 * d->margin - 1) -
                            (height() - 2 * d->margin - 1) *
                                    static_cast<double>(componentValue()) /
                                    componentRange();
        painter.setBrush(QBrush(Qt::white));
        painter.setPen(Qt::NoPen);
        painter.drawRect(
                QRectF(d->margin, yPos - 1, width() - 2 * d->margin - 1, 3));
        painter.setPen(Qt::black);
        painter.drawLine(
                QLineF(d->margin, yPos, width() - d->margin - 1, yPos));
    }
}

void ColorRampWidget::setOrientation(Orientation orientation)
{
    d->orientation = orientation;
    if (d->orientation == ColorRampWidget::Horizontal)
    {
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    }
    else
    {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    }
    updateGeometry();
}

ColorRampWidget::Orientation ColorRampWidget::orientation() const
{
    return d->orientation;
}

void ColorRampWidget::setInteriorMargin(int margin)
{
    if (d->margin == margin) return;
    d->margin = margin;
    update();
}

int ColorRampWidget::interiorMargin() const { return d->margin; }

void ColorRampWidget::setShowFrame(bool show)
{
    if (d->showFrame == show) return;
    d->showFrame = show;
    update();
}

bool ColorRampWidget::showFrame() const { return d->showFrame; }

void ColorRampWidget::setMarkerSize(int size)
{
    d->topTriangle << QPoint(-size, 0) << QPoint(size, 0) << QPoint(0, size);
    d->bottomTriangle << QPoint(-size, 0) << QPoint(size, 0)
                      << QPoint(0, -size);
    update();
}

void ColorRampWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (d->isDragging)
    {
        d->setColorFromPoint(event->pos());
    }
    ColorWidget::mouseMoveEvent(event);
}

void ColorRampWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        d->isDragging = true;
        d->setColorFromPoint(event->pos());
    }
    else
    {
        ColorWidget::mousePressEvent(event);
    }
}

void ColorRampWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        d->isDragging = false;
    }
    else
    {
        ColorWidget::mouseReleaseEvent(event);
    }
}

void ColorRampWidget::keyPressEvent(QKeyEvent *event)
{

    const int oldValue = componentValue();
    if ((d->orientation == ColorRampWidget::Horizontal &&
         (event->key() == Qt::Key_Right || event->key() == Qt::Key_Up)) ||
        (d->orientation == ColorRampWidget::Vertical &&
         (event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)))
    {
        setComponentValue(componentValue() + 1);
    }
    else if ((d->orientation == ColorRampWidget::Horizontal &&
              (event->key() == Qt::Key_Left || event->key() == Qt::Key_Down)) ||
             (d->orientation == ColorRampWidget::Vertical &&
              (event->key() == Qt::Key_Right || event->key() == Qt::Key_Down)))
    {
        setComponentValue(componentValue() - 1);
    }
    else if ((d->orientation == ColorRampWidget::Horizontal &&
              event->key() == Qt::Key_PageDown) ||
             (d->orientation == ColorRampWidget::Vertical &&
              event->key() == Qt::Key_PageUp))
    {
        setComponentValue(componentValue() + 10);
    }
    else if ((d->orientation == ColorRampWidget::Horizontal &&
              event->key() == Qt::Key_PageUp) ||
             (d->orientation == ColorRampWidget::Vertical &&
              event->key() == Qt::Key_PageDown))
    {
        setComponentValue(componentValue() - 10);
    }
    else if ((d->orientation == ColorRampWidget::Horizontal &&
              event->key() == Qt::Key_Home) ||
             (d->orientation == ColorRampWidget::Vertical &&
              event->key() == Qt::Key_End))
    {
        setComponentValue(0);
    }
    else if ((d->orientation == ColorRampWidget::Horizontal &&
              event->key() == Qt::Key_End) ||
             (d->orientation == ColorRampWidget::Vertical &&
              event->key() == Qt::Key_Home))
    {
        setComponentValue(componentRange());
    }
    else
    {
        ColorWidget::keyPressEvent(event);
        return;
    }

    if (componentValue() != oldValue)
    {
        emit colorChanged(mCurrentColor);
        emit valueChanged(componentValue());
    }
}

/* ------------------------- ColorRampWidgetPrivate ------------------------- */

void ColorRampWidgetPrivate::setColorFromPoint(QPointF point)
{
    const int oldValue = q->componentValue();
    int val;
    if (orientation == ColorRampWidget::Horizontal)
    {
        val = q->componentRange() * (point.x() - margin) /
              (q->width() - 2 * margin);
    }
    else
    {
        val = q->componentRange() - q->componentRange() * (point.y() - margin) /
                                            (q->height() - 2 * margin);
    }
    val = std::max(0, std::min(val, q->componentRange()));
    q->setComponentValue(val);
    if (oldValue != q->componentValue())
    {
        emit q->colorChanged(q->mCurrentColor);
        emit q->valueChanged(q->componentValue());
    }
}
