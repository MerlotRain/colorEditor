#include "colorbox.h"

class ColorBoxPrivate
{
public:
    ColorBoxPrivate() = default;

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

ColorBox::ColorBox(QWidget *parent, ColorComponent component) {}

ColorBox::~ColorBox() {}

QSize ColorBox::sizeHint() const { return QSize(); }

void ColorBox::paintEvent(QPaintEvent *event) {}

void ColorBox::setComponent(ColorComponent component) {}

void ColorBox::setColor(const QColor &color, bool emitSignals) {}

void ColorBox::resizeEvent(QResizeEvent *event) {}

void ColorBox::mouseMoveEvent(QMouseEvent *event) {}

void ColorBox::mousePressEvent(QMouseEvent *event) {}

void ColorBox::mouseReleaseEvent(QMouseEvent *event) {}

/* ----------------------------- ColorBoxPrivate ---------------------------- */

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
