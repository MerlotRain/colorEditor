#include "colorwheel.h"
#include <QConicalGradient>
#include <QPainter>

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

ColorWheel::ColorWheel(QWidget *parent)
    : ColorWidget(parent), d(new ColorWheelPrivate(this))
{
    QConicalGradient wheelGradient = QConicalGradient(0, 0, 0);
    const int wheelStops = 20l;
    QColor gradColor = QColor::fromHsvF(1., 1., 1.);
    for (int pos = 0; pos < wheelStops; ++pos)
    {
        const double relativePos = static_cast<double>(pos) / wheelStops;
        gradColor.setHsvF(relativePos, 1., 1.);
        wheelGradient.setColorAt(relativePos, gradColor);
    }
    d->wheelBrush = QBrush(wheelGradient);
}

ColorWheel::~ColorWheel() {}

QSize ColorWheel::sizeHint() const
{
    const int size = fontMetrics().horizontalAdvance('X') * 22;
    return QSize(size, size);
}

void ColorWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    if (d->widgetImage.isNull() || d->wheelImage.isNull() || d->triangleImage.isNull())
    {
        d->createImage(size());
    }

    d->widgetImage.fill(Qt::transparent);
    QPainter imagePainter(&d->widgetImage);
    imagePainter.setRenderHint(QPainter::Antialiasing);
    if (d->wheelDirty)
    {
        d->createWheel();
    }

    // draw wheel centered on widget
    const QPointF center =
            QPointF(d->widgetImage.width() / 2.0, d->widgetImage.height() / 2.0);
    imagePainter.drawImage(QPointF(center.x() - (d->wheelImage.width() / 2.0),
                                   center.y() - (d->wheelImage.height() / 2.0)),
                           d->wheelImage);

    // draw hue marker
    const int h = hue();
    const double length = d->wheelImage.width() / 2.0;
    QLineF hueMarkerLine =
            QLineF(center.x(), center.y(), center.x() + length, center.y());
    hueMarkerLine.setAngle(h);
    imagePainter.save();

    imagePainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    QPen pen;
    pen.setWidthF(2 * devicePixelRatioF());
    pen.setColor(h > 20 && h < 200 ? Qt::black : Qt::white);
    imagePainter.setPen(pen);
    imagePainter.drawLine(hueMarkerLine);
    imagePainter.restore();

    // draw triangle
    if (d->triangleDirty)
    {
        d->createTriangle();
    }
    imagePainter.drawImage(QPointF(center.x() - (d->wheelImage.width() / 2.0),
                                   center.y() - (d->wheelImage.height() / 2.0)),
                           d->triangleImage);
}

void ColorWheel::setColor(const QColor &color, bool emitSignals) {}

void ColorWheel::resizeEvent(QResizeEvent *event) {}

void ColorWheel::mouseMoveEvent(QMouseEvent *event) {}

void ColorWheel::mousePressEvent(QMouseEvent *event) {}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event) {}

/* ---------------------------- ColorWheelPrivate --------------------------- */

ColorWheelPrivate::ColorWheelPrivate(ColorWheel *ptr) : q(ptr) {}

void ColorWheelPrivate::createImage(QSizeF size) {}

void ColorWheelPrivate::createWheel() {}

void ColorWheelPrivate::createTriangle() {}

void ColorWheelPrivate::setColorFromPos(QPointF pos) {}
