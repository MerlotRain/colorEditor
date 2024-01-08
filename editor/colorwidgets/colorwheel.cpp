#include "colorwheel.h"
#include <QConicalGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QResizeEvent>

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

    // draw current color marker
    const double triangleRadius = length - d->wheelThickness * devicePixelRatioF() - 1;
    const double lightless = mCurrentColor.lightnessF();
    const double huaRadians = (h * M_PI / 180.0);
    const double hx = std::cos(huaRadians) * triangleRadius;
    const double hy = -std::sin(huaRadians) * triangleRadius;
    const double sx = -std::cos(-huaRadians + (M_PI / 3.0)) * triangleRadius;
    const double sy = -std::sin(-huaRadians + (M_PI / 3.0)) * triangleRadius;
    const double vx = -std::cos(huaRadians + (M_PI / 3.0)) * triangleRadius;
    const double vy = std::sin(huaRadians + (M_PI / 3.0)) * triangleRadius;
    const double mx = (sx + vx) / 2.0;
    const double my = (sy + vy) / 2.0;

    const double a =
            (1 - 2.0 * std::fabs(lightless - 0.5)) * mCurrentColor.hslSaturationF();
    const double x = sx + (vx - sx) * lightless + (hx - mx) * a;
    const double y = sy + (vy - sy) * lightless + (hy - my) * a;
    const QPointF triangleCenter = QPointF(x, y);
    imagePainter.save();
    imagePainter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    imagePainter.setBrush(d->wheelBrush);
    imagePainter.drawEllipse(triangleCenter, triangleRadius, triangleRadius);
    imagePainter.restore();
    painter.drawImage(QPoint(0, 0), d->widgetImage);

    // adapt pen color for lightness
    pen.setColor(lightless > 0.7 ? Qt::black : Qt::white);
    imagePainter.setPen(pen);
    imagePainter.setBrush(Qt::NoBrush);
    imagePainter.drawEllipse(QPointF(x + center.x(), y + center.y()),
                             4.0 * devicePixelRatioF(), 4.0 * devicePixelRatioF());

    // draw image onto widget
    painter.drawImage(QRect(0, 0, width(), height()), d->widgetImage);
    painter.end();
}

void ColorWheel::setColor(const QColor &color, bool emitSignals)
{
    if (color.hue() >= 0 && color.hue() != hue())
    {
        d->triangleDirty = true;
    }
    ColorWidget::setColor(color, emitSignals);
}

void ColorWheel::resizeEvent(QResizeEvent *event)
{
    ColorWidget::resizeEvent(event);
#ifdef Q_OS_WIN
    if (event->size().width() > parentWidget()->size().width())
    {
        QSize newSize(
                std::min(event->size().width(), parentWidget()->size().width() - 2),
                std::min(event->size().height(), parentWidget()->size().height() - 2));
        resize(newSize);
        d->createImage(newSize);
    }
    else
    {
        d->createImage(event->size());
    }
#else
    d->createImage(event->size());
#endif
}

void ColorWheel::mouseMoveEvent(QMouseEvent *event)
{
    if (d->dragging) d->setColorFromPos(event->pos());
    ColorWidget::mouseMoveEvent(event);
}

void ColorWheel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        d->dragging = true;
        const QLineF line =
                QLineF(width() / 2.0, height() / 2.0, event->pos().x(), event->pos().y());
        const double innerLength =
                d->wheelImage.width() / 2.0 / devicePixelRatioF() - d->wheelThickness;
        if (line.length() < innerLength)
        {
            d->controlPart = ColorWheelPrivate::Triangle;
        }
        else
        {
            d->controlPart = ColorWheelPrivate::Wheel;
        }
        d->setColorFromPos(event->pos());
    }
    else
    {
        ColorWidget::mousePressEvent(event);
    }
}

void ColorWheel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        d->dragging = false;
        d->controlPart = ColorWheelPrivate::None;
    }
    else
    {
        ColorWidget::mouseReleaseEvent(event);
    }
}

/* ---------------------------- ColorWheelPrivate --------------------------- */

ColorWheelPrivate::ColorWheelPrivate(ColorWheel *ptr) : q(ptr) {}

void ColorWheelPrivate::createImage(QSizeF size)
{
    const double wheelSize = std::min(size.width(), size.height()) - margin * 2.0;
    wheelThickness = wheelSize / 15.0;

    const double pixelRatio = q->devicePixelRatioF();
    wheelImage =
            QImage(wheelSize * pixelRatio, wheelSize * pixelRatio, QImage::Format_ARGB32);
    triangleImage =
            QImage(wheelSize * pixelRatio, wheelSize * pixelRatio, QImage::Format_ARGB32);
    widgetImage = QImage(size.width() * pixelRatio, size.height() * pixelRatio,
                         QImage::Format_ARGB32);

    wheelDirty = true;
    triangleDirty = true;
}

void ColorWheelPrivate::createWheel()
{
    if (wheelImage.isNull())
    {
        return;
    }

    const int maxSize = std::min(wheelImage.width(), wheelImage.height());
    const double wheelRadius = maxSize / 2.0;

    wheelImage.fill(Qt::transparent);
    QPainter painter(&wheelImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(wheelBrush);
    painter.translate(wheelRadius, wheelRadius);
    painter.drawEllipse(QPointF(0., 0.), wheelRadius, wheelRadius);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPointF(0., 0.),
                        wheelRadius - wheelThickness * q->devicePixelRatioF(),
                        wheelRadius - wheelThickness * q->devicePixelRatioF());
    painter.end();

    wheelDirty = false;
}

void ColorWheelPrivate::createTriangle()
{
    if (wheelImage.isNull() || triangleImage.isNull())
    {
        return;
    }

    const QPointF center = QPointF(wheelImage.width() / 2.0, wheelImage.height() / 2.0);
    triangleImage.fill(Qt::transparent);

    QPainter imagePainter(&triangleImage);
    imagePainter.setRenderHint(QPainter::Antialiasing);

    const int angle = q->hue();
    const double wheelRadius = wheelImage.width() / 2.0;
    const double triangleRadius =
            wheelRadius - wheelThickness * q->devicePixelRatioF() - 1;

    //pure version of hue (at full saturation and value)
    const QColor pureColor = QColor::fromHsv(angle, 255, 255);
    //create copy of color but with 0 alpha
    QColor alphaColor = QColor(pureColor);
    alphaColor.setAlpha(0);

    //some rather ugly shortcuts to obtain corners and midpoints of triangle
    QLineF line1 = QLineF(center.x(), center.y(),
                          center.x() - triangleRadius * std::cos(M_PI / 3.0),
                          center.y() - triangleRadius * std::sin(M_PI / 3.0));
    QLineF line2 =
            QLineF(center.x(), center.y(), center.x() + triangleRadius, center.y());
    QLineF line3 = QLineF(center.x(), center.y(),
                          center.x() - triangleRadius * std::cos(M_PI / 3.0),
                          center.y() + triangleRadius * std::sin(M_PI / 3.0));
    QLineF line4 = QLineF(center.x(), center.y(),
                          center.x() - triangleRadius * std::cos(M_PI / 3.0), center.y());
    QLineF line5 = QLineF(center.x(), center.y(), (line2.p2().x() + line1.p2().x()) / 2.0,
                          (line2.p2().y() + line1.p2().y()) / 2.0);
    line1.setAngle(line1.angle() + angle);
    line2.setAngle(line2.angle() + angle);
    line3.setAngle(line3.angle() + angle);
    line4.setAngle(line4.angle() + angle);
    line5.setAngle(line5.angle() + angle);
    const QPointF p1 = line1.p2();
    const QPointF p2 = line2.p2();
    const QPointF p3 = line3.p2();
    const QPointF p4 = line4.p2();
    const QPointF p5 = line5.p2();

    //inspired by Tim Baumann's work at https://github.com/timjb/colortriangle/blob/master/colortriangle.js
    QLinearGradient colorGrad = QLinearGradient(p4.x(), p4.y(), p2.x(), p2.y());
    colorGrad.setColorAt(0, alphaColor);
    colorGrad.setColorAt(1, pureColor);
    QLinearGradient whiteGrad = QLinearGradient(p3.x(), p3.y(), p5.x(), p5.y());
    whiteGrad.setColorAt(0, QColor(255, 255, 255, 255));
    whiteGrad.setColorAt(1, QColor(255, 255, 255, 0));

    QPolygonF triangle;
    triangle << p2 << p1 << p3 << p2;
    imagePainter.setPen(Qt::NoPen);
    //start with a black triangle
    imagePainter.setBrush(QBrush(Qt::black));
    imagePainter.drawPolygon(triangle);
    //draw a gradient from transparent to the pure color at the triangle's tip
    imagePainter.setBrush(QBrush(colorGrad));
    imagePainter.drawPolygon(triangle);
    //draw a white gradient using additive composition mode
    imagePainter.setCompositionMode(QPainter::CompositionMode_Plus);
    imagePainter.setBrush(QBrush(whiteGrad));
    imagePainter.drawPolygon(triangle);

    //above process results in some small artifacts on the edge of the triangle. Let's clear these up
    //use source composition mode and draw an outline using a transparent pen
    //this clears the edge pixels and leaves a nice smooth image
    imagePainter.setCompositionMode(QPainter::CompositionMode_Source);
    imagePainter.setBrush(Qt::NoBrush);
    imagePainter.setPen(QPen(Qt::transparent));
    imagePainter.drawPolygon(triangle);

    imagePainter.end();
    triangleDirty = false;
}

void ColorWheelPrivate::setColorFromPos(QPointF pos)
{
    const QPointF center = QPointF(q->width() / 2.0, q->height() / 2.0);
    //line from center to mouse position
    const QLineF line = QLineF(center.x(), center.y(), pos.x(), pos.y());

    QColor newColor = QColor();

    int h, s, l, alpha;
    q->mCurrentColor.getHsl(&h, &s, &l, &alpha);
    //override hue with explicit hue, so we don't get -1 values from QColor for hue
    h = q->hue();

    if (controlPart == ColorWheelPrivate::Triangle)
    {
        const double x = pos.x() - center.x();
        const double y = pos.y() - center.y();

        double eventAngleRadians = line.angle() * M_PI / 180.0;
        const double hueRadians = h * M_PI / 180.0;
        double rad0 = std::fmod(eventAngleRadians + 2.0 * M_PI - hueRadians, 2.0 * M_PI);
        double rad1 = std::fmod(rad0, ((2.0 / 3.0) * M_PI)) - (M_PI / 3.0);
        const double length = wheelImage.width() / 2.0 / q->devicePixelRatioF();
        const double triangleLength = length - wheelThickness - 1;

        const double a = 0.5 * triangleLength;
        double b = std::tan(rad1) * a;
        double r = std::sqrt(x * x + y * y);
        const double maxR = std::sqrt(a * a + b * b);

        if (r > maxR)
        {
            const double dx = std::tan(rad1) * r;
            double rad2 = std::atan(dx / maxR);
            rad2 = std::min(rad2, M_PI / 3.0);
            rad2 = std::max(rad2, -M_PI / 3.0);
            eventAngleRadians += rad2 - rad1;
            rad0 = std::fmod(eventAngleRadians + 2.0 * M_PI - hueRadians, 2.0 * M_PI);
            rad1 = std::fmod(rad0, ((2.0 / 3.0) * M_PI)) - (M_PI / 3.0);
            b = std::tan(rad1) * a;
            r = std::sqrt(a * a + b * b);
        }

        const double triangleSideLength = std::sqrt(3.0) * triangleLength;
        const double newL = ((-std::sin(rad0) * r) / triangleSideLength) + 0.5;
        const double widthShare = 1.0 - (std::fabs(newL - 0.5) * 2.0);
        const double newS = (((std::cos(rad0) * r) + (triangleLength / 2.0)) /
                             (1.5 * triangleLength)) /
                            widthShare;
        s = std::min(static_cast<int>(std::round(std::max(0.0, newS) * 255.0)), 255);
        l = std::min(static_cast<int>(std::round(std::max(0.0, newL) * 255.0)), 255);
        newColor = QColor::fromHsl(h, s, l);
        //explicitly set the hue again, so that it's exact
        newColor.setHsv(h, newColor.hsvSaturation(), newColor.value(), alpha);
    }
    else if (controlPart == ColorWheelPrivate::Wheel)
    {
        //use hue angle
        s = q->mCurrentColor.hsvSaturation();
        const int v = q->mCurrentColor.value();
        const int newHue = line.angle();
        newColor = QColor::fromHsv(newHue, s, v, alpha);
        //hue has changed, need to redraw triangle
        triangleDirty = true;
    }

    if (newColor.isValid() && newColor != q->mCurrentColor)
    {
        //color has changed
        q->mCurrentColor = QColor(newColor);

        if (q->mCurrentColor.hue() >= 0)
        {
            //color has a valid hue, so update the QgsColorWidget's explicit hue
            q->mExplicitHue = q->mCurrentColor.hue();
        }

        q->update();
        emit q->colorChanged(q->mCurrentColor);
    }
}
