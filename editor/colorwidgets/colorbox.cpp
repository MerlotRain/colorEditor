#include "colorbox.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOptionFrame>

class ColorBoxPrivate : public QObject
{
    Q_OBJECT
public:
    ColorBoxPrivate(ColorBox *ptr);
    ~ColorBoxPrivate();

    ColorBox *q;

    bool isDragging = false;
    int margin = 2;
    QImage *boxImage = nullptr;
    bool dirty = true;

    void createBox();
    int valueRangeX() const;
    int valueRangeY() const;
    ColorWidget::ColorComponent yComponent() const;
    int yComponentValue() const;
    ColorWidget::ColorComponent xComponent() const;
    int xComponentValue() const;
    void setColorFromPoint(QPointF point);
};

/* -------------------------------- ColorBox -------------------------------- */

ColorBox::ColorBox(QWidget *parent, ColorComponent component)
    : ColorWidget(parent, component), d(new ColorBoxPrivate(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    d->boxImage = new QImage(width() - d->margin * 2, height() - d->margin * 2,
                             QImage::Format_ARGB32);
}

ColorBox::~ColorBox()
{
    delete d;
    d = nullptr;
}

QSize ColorBox::sizeHint() const
{
    const int size = fontMetrics().horizontalAdvance('X') * 22;
    return QSize(size, size);
}

void ColorBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    QStyleOptionFrame option;
    option.initFrom(this);
    option.state = hasFocus() ? QStyle::State_Active : QStyle::State_None;
    style()->drawPrimitive(QStyle::PE_Frame, &option, &painter);

    if (d->dirty)
    {
        d->createBox();
    }

    // draw background image
    painter.drawImage(QPoint(d->margin, d->margin), *d->boxImage);

    // draw cross line
    const double xPos = d->margin + (width() - 2 * d->margin - 1) *
                                            static_cast<double>(d->xComponentValue()) /
                                            static_cast<double>(d->valueRangeX());
    const double yPos = d->margin + (height() - 2 * d->margin - 1) -
                        (height() - 2 * d->margin - 1) *
                                static_cast<double>(d->yComponentValue()) /
                                static_cast<double>(d->valueRangeY());

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);

    painter.drawRect(xPos - 1, d->margin, 3, height() - 2 * d->margin - 1);
    painter.drawRect(d->margin, yPos - 1, width() - 2 * d->margin - 1, 3);
    painter.setPen(Qt::black);
    painter.drawRect(xPos, d->margin, xPos, height() - d->margin - 1);
    painter.drawRect(d->margin, yPos, width() - d->margin - 1, yPos);

    painter.end();
}

void ColorBox::setComponent(ColorComponent component)
{
    if (component == mComponent)
    {
        d->dirty = true;
    }
    ColorWidget::setComponent(component);
}

void ColorBox::setColor(const QColor &color, bool emitSignals)
{
    if (mComponent == ColorWidget::Red && mCurrentColor.red() != color.red())
    {
        d->dirty = true;
    }
    else if (mComponent == ColorWidget::Green && mCurrentColor.green() != color.green())
    {
        d->dirty = true;
    }
    else if (mComponent == ColorWidget::Blue && mCurrentColor.blue() != color.blue())
    {
        d->dirty = true;
    }
    else if (mComponent == ColorWidget::Hue && mCurrentColor.hue() != color.hue())
    {
        d->dirty = true;
    }
    else if (mComponent == ColorWidget::Saturation &&
             mCurrentColor.hsvSaturation() != color.hsvSaturation())
    {
        d->dirty = true;
    }
    else if (mComponent == ColorWidget::Value && mCurrentColor.value() != color.value())
    {
        d->dirty = true;
    }
    ColorWidget::setColor(color, emitSignals);
}

void ColorBox::resizeEvent(QResizeEvent *event)
{
    d->dirty = true;
    delete d->boxImage;
    d->boxImage =
            new QImage(event->size().width() - d->margin * 2,
                       event->size().height() - d->margin * 2, QImage::Format_ARGB32);
    ColorWidget::resizeEvent(event);
}

void ColorBox::mouseMoveEvent(QMouseEvent *event)
{
    if (d->isDragging)
    {
        d->setColorFromPoint(event->pos());
    }
    ColorWidget::mouseMoveEvent(event);
}

void ColorBox::mousePressEvent(QMouseEvent *event)
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

void ColorBox::mouseReleaseEvent(QMouseEvent *event)
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

/* ----------------------------- ColorBoxPrivate ---------------------------- */

ColorBoxPrivate::ColorBoxPrivate(ColorBox *ptr) : q(ptr) {}

ColorBoxPrivate::~ColorBoxPrivate()
{
    delete boxImage;
    boxImage = nullptr;
}

void ColorBoxPrivate::createBox()
{
    const int maxValueX = boxImage->width();
    const int maxValueY = boxImage->height();

    QColor currentColor = QColor(q->mCurrentColor);
    int colorComponentValue;

    for (int y = 0; y < maxValueY; ++y)
    {
        QRgb *scanLine = (QRgb *) boxImage->scanLine(y);

        colorComponentValue =
                int(valueRangeY() - valueRangeY() * (double(y) / maxValueY));
        q->alterColor(currentColor, colorComponentValue, yComponent());
        for (int x = 0; x < maxValueX; ++x)
        {
            colorComponentValue = int(valueRangeX() * (double(x) / maxValueX));
            q->alterColor(currentColor, colorComponentValue, xComponent());
            scanLine[x] = currentColor.rgb();
        }
    }
    dirty = false;
}

int ColorBoxPrivate::valueRangeX() const { return q->componentRange(xComponent()); }

int ColorBoxPrivate::valueRangeY() const { return q->componentRange(yComponent()); }

ColorWidget::ColorComponent ColorBoxPrivate::yComponent() const
{
    switch (q->mComponent)
    {
        case ColorWidget::Red:
            return ColorWidget::Green;
        case ColorWidget::Green:
            return ColorWidget::Blue;
        case ColorWidget::Blue:
            return ColorWidget::Red;
        case ColorWidget::Hue:
            return ColorWidget::Saturation;
        case ColorWidget::Saturation:
        case ColorWidget::Value:
            return ColorWidget::Value;
        default:
            return ColorWidget::Red;
    }
}

int ColorBoxPrivate::yComponentValue() const { return q->componentValue(yComponent()); }

ColorWidget::ColorComponent ColorBoxPrivate::xComponent() const
{
    switch (q->mComponent)
    {
        case ColorWidget::Red:
        case ColorWidget::Green:
            return ColorWidget::Blue;
        case ColorWidget::Blue:
            return ColorWidget::Green;
        case ColorWidget::Hue:
        case ColorWidget::Saturation:
            return ColorWidget::Value;
        case ColorWidget::Value:
            return ColorWidget::Saturation;
        default:
            return ColorWidget::Red;
    }
}

int ColorBoxPrivate::xComponentValue() const { return q->componentValue(xComponent()); }

void ColorBoxPrivate::setColorFromPoint(QPointF point)
{
    int valX = valueRangeX() * (point.x() - margin) / (q->width() - 2 * margin - 1);
    valX = std::min(std::max(valX, 0), valueRangeX());
    int valY = valueRangeY() * valueRangeY() * (point.y() - margin) /
               (q->height() - 2 * margin - 1);
    valY = std::min(std::max(valY, 0), valueRangeY());

    QColor color = QColor(q->mCurrentColor);
    q->alterColor(color, valX, xComponent());
    q->alterColor(color, valY, yComponent());

    if (color == q->mCurrentColor)
    {
        return;
    }

    if (color.hue() >= 0)
    {
        q->mExplicitHue = color.hue();
    }

    q->mCurrentColor = color;
    q->update();
    emit q->colorChanged(color);
}

#include "colorbox.moc"