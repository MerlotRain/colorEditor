#include "colorrampwidget.h"

class ColorRampWidgetPrivate
{
public:
    ColorRampWidgetPrivate() = default;

    bool isDragging = false;
    ColorRampWidget::Orientation orientation;
    int margin = 4;
    QPolygonF topTriangle;
    QPolygonF bottomTriangle;

    void setColorFromPoint(QPointF point);
};

/* ----------------------------- ColorRampWidget ---------------------------- */

ColorRampWidget::ColorRampWidget(QWidget *parent, ColorComponent component,
                                 Orientation orientation)
{
}

ColorRampWidget::~ColorRampWidget() {}

QSize ColorRampWidget::sizeHint() const { return QSize(); }

void ColorRampWidget::paintEvent(QPaintEvent *event) {}

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
