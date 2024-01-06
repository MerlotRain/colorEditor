#include "colorpreviewwidget.h"

class ColorPreviewWidgetPrivate
{
public:
    QColor color2;
    QPoint dragStartPosition;

    void drawColor(const QColor &color, QRect rect, QPainter &painter);
};

/* --------------------------- ColorPreviewWidget --------------------------- */

ColorPreviewWidget::ColorPreviewWidget(QWidget *parent) {}

void ColorPreviewWidget::paintEvent(QPaintEvent *event) {}

QSize ColorPreviewWidget::sizeHint() const { return QSize(); }

QColor ColorPreviewWidget::color2() const { return QColor(); }

void ColorPreviewWidget::setColor2(const QColor &color) {}

void ColorPreviewWidget::mousePressEvent(QMouseEvent *event) {}

void ColorPreviewWidget::mouseMoveEvent(QMouseEvent *event) {}

void ColorPreviewWidget::mouseReleaseEvent(QMouseEvent *event) {}

/* ------------------------ ColorPreviewWidgetPrivate ----------------------- */

void ColorPreviewWidgetPrivate::drawColor(const QColor &color, QRect rect,
                                          QPainter &painter)
{
}
