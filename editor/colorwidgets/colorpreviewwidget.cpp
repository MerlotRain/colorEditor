#include "colorpreviewwidget.h"
#include "../colorutility.h"
#include <QApplication>
#include <QDrag>
#include <QMouseEvent>
#include <QPainter>

class ColorPreviewWidgetPrivate
{
public:
    ColorPreviewWidgetPrivate(ColorPreviewWidget *ptr);

    ColorPreviewWidget *q;
    QColor color2;
    QPoint dragStartPosition;

    void drawColor(const QColor &color, QRect rect, QPainter &painter);
};

/* --------------------------- ColorPreviewWidget --------------------------- */

ColorPreviewWidget::ColorPreviewWidget(QWidget *parent)
    : ColorWidget(parent), d(new ColorPreviewWidgetPrivate(this))
{
}

void ColorPreviewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    if (d->color2.isValid())
    {
        //drawing with two color sections
        const int verticalSplit = std::round(height() / 2.0);
        d->drawColor(mCurrentColor, QRect(0, 0, width(), verticalSplit), painter);
        d->drawColor(d->color2,
                     QRect(0, verticalSplit, width(), height() - verticalSplit), painter);
    }
    else if (mCurrentColor.isValid())
    {
        d->drawColor(mCurrentColor, QRect(0, 0, width(), height()), painter);
    }

    painter.end();
}

QSize ColorPreviewWidget::sizeHint() const
{
    return QSize(ColorUtility::UI_SCALE_FACTOR * fontMetrics().horizontalAdvance('X') *
                         22,
                 ColorUtility::UI_SCALE_FACTOR * fontMetrics().horizontalAdvance('X') *
                         22 * 0.75);
}

QColor ColorPreviewWidget::color2() const { return d->color2; }

void ColorPreviewWidget::setColor2(const QColor &color)
{
    if (color == d->color2) return;
    d->color2 = color;
    update();
}

void ColorPreviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        d->dragStartPosition = event->pos();
    }
    ColorWidget::mousePressEvent(event);
}

void ColorPreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    //handle dragging colors from button

    if (!(event->buttons() & Qt::LeftButton))
    {
        //left button not depressed, so not a drag
        ColorWidget::mouseMoveEvent(event);
        return;
    }

    if ((event->pos() - d->dragStartPosition).manhattanLength() <
        QApplication::startDragDistance())
    {
        //mouse not moved, so not a drag
        ColorWidget::mouseMoveEvent(event);
        return;
    }

    //user is dragging color

    //work out which color is being dragged
    QColor dragColor = mCurrentColor;
    if (d->color2.isValid())
    {
        //two color sections, check if dragged color was the second color
        const int verticalSplit = std::round(height() / 2.0);
        if (d->dragStartPosition.y() >= verticalSplit)
        {
            dragColor = d->color2;
        }
    }

    QDrag *drag = new QDrag(this);
    drag->setMimeData(ColorUtility::colorToMimeData(dragColor));
    drag->setPixmap(createDragIcon(dragColor));
    drag->exec(Qt::CopyAction);
}

void ColorPreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->pos() - d->dragStartPosition).manhattanLength() >=
        QApplication::startDragDistance())
    {
        //mouse moved, so a drag. nothing to do here
        ColorWidget::mouseReleaseEvent(event);
        return;
    }

    //work out which color was clicked
    QColor clickedColor = mCurrentColor;
    if (d->color2.isValid())
    {
        //two color sections, check if dragged color was the second color
        const int verticalSplit = std::round(height() / 2.0);
        if (d->dragStartPosition.y() >= verticalSplit)
        {
            clickedColor = d->color2;
        }
    }
    emit colorChanged(clickedColor);
}

/* ------------------------ ColorPreviewWidgetPrivate ----------------------- */

ColorPreviewWidgetPrivate::ColorPreviewWidgetPrivate(ColorPreviewWidget *ptr) : q(ptr)
{
    color2 = QColor();
}

void ColorPreviewWidgetPrivate::drawColor(const QColor &color, QRect rect,
                                          QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    //if color has an alpha, start with a check board pattern
    if (color.alpha() < 255)
    {
        const QBrush checkBrush = QBrush(q->transparentBackground());
        painter.setBrush(checkBrush);
        painter.drawRect(rect);

        //draw half of widget showing solid color, the other half showing color with alpha

        //ensure at least a 1px overlap to avoid artifacts
        const QBrush colorBrush = QBrush(color);
        painter.setBrush(colorBrush);
        painter.drawRect(std::floor(rect.width() / 2.0) + rect.left(), rect.top(),
                         rect.width() - std::floor(rect.width() / 2.0), rect.height());

        QColor opaqueColor = QColor(color);
        opaqueColor.setAlpha(255);
        const QBrush opaqueBrush = QBrush(opaqueColor);
        painter.setBrush(opaqueBrush);
        painter.drawRect(rect.left(), rect.top(), std::ceil(rect.width() / 2.0),
                         rect.height());
    }
    else
    {
        //no alpha component, just draw a solid rectangle
        const QBrush brush = QBrush(color);
        painter.setBrush(brush);
        painter.drawRect(rect);
    }
}
