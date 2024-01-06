#include "colorwheel.h"

class ColorWheelPrivate
{
public:
    ColorWheelPrivate(ColorWheel *ptr);

    enum ControlPart
    {
        None,
        Wheel,
        Triangle,
    };

    bool dragging = false;
    int margin = 4;
    int wheelThickness = 13;
    ControlPart controlPart = None;
    QImage wheelImage;
    QImage triangleImage;
    QImage widgetImage;
    bool wheelDirty = true;
    bool triangleDirty = true;
    QBrush wheelBrush;
    ColorWheel *q;

    void createImage(QSizeF size);
    void createWheel();
    void createTriangle();
    void setColorFromPos(QPointF pos);
};

/* ------------------------------- ColorWheel ------------------------------- */

ColorWheel::ColorWheel(QWidget *parent) {}

ColorWheel::~ColorWheel() {}

QSize ColorWheel::sizeHint() const { return QSize(); }

void ColorWheel::paintEvent(QPaintEvent *event) {}

void ColorWheel::setColor(const QColor &color, bool emitSignals) {}

void ColorWheel::resizeEvent(QResizeEvent *event) {}

void ColorWheel::mouseMoveEvent(QMouseEvent *event) {}

void ColorWheel::mousePressEvent(QMouseEvent *event) {}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event) {}

/* ---------------------------- ColorWheelPrivate --------------------------- */

ColorWheelPrivate::ColorWheelPrivate(ColorWheel *ptr) {}

void ColorWheelPrivate::createImage(QSizeF size) {}

void ColorWheelPrivate::createWheel() {}

void ColorWheelPrivate::createTriangle() {}

void ColorWheelPrivate::setColorFromPos(QPointF pos) {}
