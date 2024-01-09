#include "colorbox.h"
#include <QPainter>
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

    
}

void ColorBox::setComponent(ColorComponent component) {}

void ColorBox::setColor(const QColor &color, bool emitSignals) {}

void ColorBox::resizeEvent(QResizeEvent *event) {}

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
