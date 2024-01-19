#include "colorrampwidget.h"
#include "../colorutility.h"
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOptionFrame>

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

void ColorRampWidget::setOrientation(Orientation orientation) {}

ColorRampWidget::Orientation ColorRampWidget::orientation() const
{
    return Orientation();
}

void ColorRampWidget::setInteriorMargin(int margin) {}

int ColorRampWidget::interiorMargin() const { return 0; }

void ColorRampWidget::setShowFrame(bool show) {}

bool ColorRampWidget::showFrame() const { return false; }

void ColorRampWidget::setMarkerSize(int size) {}

void ColorRampWidget::resizeEvent(QResizeEvent *event) {}

void ColorRampWidget::mouseMoveEvent(QMouseEvent *event) {}

void ColorRampWidget::mousePressEvent(QMouseEvent *event) {}

void ColorRampWidget::mouseReleaseEvent(QMouseEvent *event) {}

void ColorRampWidget::keyPressEvent(QKeyEvent *event) {}

/* ------------------------- ColorRampWidgetPrivate ------------------------- */

void ColorRampWidgetPrivate::setColorFromPoint(QPointF point) {}
