#include "colorbox.h"
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOptionFrame>

class ColorBoxPrivate
{
public:
    ColorBoxPrivate(ColorBox *ptr);
    ~ColorBoxPrivate();

    ColorBox *q;

    bool isDragging = false;
    int margin = 2;
    QImage *boxImage = nullptr;
    bool dirty = true;

    void createBox();
    int valueChangeX() const;
    int valueChangeY() const;
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
                                            static_cast<double>(d->valueChangeX());
    const double yPos = d->margin + (height() - 2 * d->margin - 1) -
                        (height() - 2 * d->margin - 1) *
                                static_cast<double>(d->yComponentValue()) /
                                static_cast<double>(d->valueChangeY());

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

void ColorBox::mouseMoveEvent(QMouseEvent *event) {}

void ColorBox::mousePressEvent(QMouseEvent *event) {}

void ColorBox::mouseReleaseEvent(QMouseEvent *event) {}

/* ----------------------------- ColorBoxPrivate ---------------------------- */

ColorBoxPrivate::ColorBoxPrivate(ColorBox *ptr) : q(ptr) {}

ColorBoxPrivate::~ColorBoxPrivate()
{
    delete boxImage;
    boxImage = nullptr;
}

void ColorBoxPrivate::createBox() {}

int ColorBoxPrivate::valueChangeX() const { return 0; }

int ColorBoxPrivate::valueChangeY() const { return 0; }

ColorWidget::ColorComponent ColorBoxPrivate::yComponent() const
{
    return ColorWidget::ColorComponent();
}

int ColorBoxPrivate::yComponentValue() const { return 0; }

ColorWidget::ColorComponent ColorBoxPrivate::xComponent() const
{
    return ColorWidget::ColorComponent();
}

int ColorBoxPrivate::xComponentValue() const { return 0; }

void ColorBoxPrivate::setColorFromPoint(QPointF point) {}
