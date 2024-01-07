#include "colorwidget.h"
#include "colorutility.h"
#include <QColor>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QVariant>

ColorWidget::ColorWidget(QWidget *parent, ColorComponent component)
    : QWidget(parent), mCurrentColor(Qt::red), mComponent(component)
{
    setAcceptDrops(true);
}

QColor ColorWidget::color() const { return mCurrentColor; }

ColorWidget::ColorComponent ColorWidget::component() const { return ColorComponent(); }

int ColorWidget::componentValue() const { return componentValue(mComponent); }

QPixmap ColorWidget::createDragIcon(const QColor &color)
{
    const int iconSize = 50;
    QPixmap pixmap(iconSize, iconSize);
    pixmap.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&pixmap);
    painter.fillRect(QRect(0, 0, iconSize, iconSize), QBrush(QColor(200, 200, 200)));
    QColor pixmapColor = color;
    pixmapColor.setAlpha(255);
    painter.setBrush(QBrush(pixmapColor));
    painter.setPen(QPen(Qt::white));
    painter.drawRect(QRect(1, 1, iconSize - 2, iconSize - 2));
    painter.end();
    return pixmap;
}

void ColorWidget::setColor(const QColor &color, bool emitSignals)
{
    if (color == mCurrentColor) return;

    mCurrentColor = color;
    if (mCurrentColor.hue() >= 0)
    {
        mExplicitHue = color.hue();
    }
    if (emitSignals)
    {
        emit colorChanged(mCurrentColor);
    }
    update();
}

void ColorWidget::setComponent(ColorComponent component)
{
    if (component == mComponent)
    {
        return;
    }
    mComponent = component;
    update();
}

void ColorWidget::setComponentValue(int value)
{
    if (mComponent == Multiple)
    {
        return;
    }

    int valueClipped = std::min(value, componentRange());
    valueClipped = std::max(valueClipped, 0);

    int r, g, b, a;
    mCurrentColor.getRgb(&r, &g, &b, &a);
    int h, s, v;
    mCurrentColor.getHsv(&h, &s, &v);
    h = hue();

    switch (mComponent)
    {
        case ColorComponent::Red:
            if (r == valueClipped) return;
            mCurrentColor.setRed(valueClipped);
            break;
        case ColorComponent::Green:
            if (g == valueClipped) return;
            mCurrentColor.setGreen(valueClipped);
            break;
        case ColorComponent::Blue:
            if (b == valueClipped) return;
            mCurrentColor.setBlue(valueClipped);
            break;
        case ColorComponent::Alpha:
            if (a == valueClipped) return;
            mCurrentColor.setAlpha(valueClipped);
            break;
        case ColorComponent::Hue:
            if (h == valueClipped) return;
            mCurrentColor.setHsv(valueClipped, s, v, a);
            break;
        case ColorComponent::Saturation:
            if (s == valueClipped) return;
            mCurrentColor.setHsv(h, valueClipped, v, a);
            break;
        case ColorComponent::Value:
            if (v == valueClipped) return;
            mCurrentColor.setHsv(h, s, valueClipped, a);
            break;
        default:
            return;
    }

    if (mCurrentColor.hue() >= 0)
    {
        mExplicitHue = mCurrentColor.hue();
    }
    update();
}

void ColorWidget::dragEnterEvent(QDragEnterEvent *event)
{
    bool hasAlpha;
    const QColor color = ColorUtility::colorFromMimeData(event->mimeData(), hasAlpha);
    if (color.isValid())
    {
        event->acceptProposedAction();
    }
}

void ColorWidget::dropEvent(QDropEvent *event)
{
    bool hasAlpha;
    QColor color = ColorUtility::colorFromMimeData(event->mimeData(), hasAlpha);
    if (color.isValid())
    {
        event->acceptProposedAction();
        if (!hasAlpha)
        {
            color.setAlpha(mCurrentColor.alpha());
        }
        setColor(color);
        emit colorChanged(mCurrentColor);
    }
}

void ColorWidget::mouseMoveEvent(QMouseEvent *event)
{
    emit hovered();
    event->accept();
}

void ColorWidget::mousePressEvent(QMouseEvent *event) { event->accept(); }

void ColorWidget::mouseReleaseEvent(QMouseEvent *event) { event->accept(); }

int ColorWidget::componentRange() const { return componentRange(mComponent); }

int ColorWidget::componentRange(ColorComponent component) const
{
    if (component == ColorComponent::Multiple)
    {
        return -1;
    }

    if (component == ColorComponent::Hue)
    {
        return 359;
    }
    else
    {
        return 255;
    }
}

int ColorWidget::componentValue(ColorComponent component) const
{
    if (!mCurrentColor.isValid())
    {
        return -1;
    }

    switch (component)
    {
        case ColorComponent::Red:
            return mCurrentColor.red();
        case ColorComponent::Green:
            return mCurrentColor.green();
        case ColorComponent::Blue:
            return mCurrentColor.blue();
        case ColorComponent::Hue:
            return hue();
        case ColorComponent::Saturation:
            return mCurrentColor.saturation();
        case ColorComponent::Value:
            return mCurrentColor.value();
        case ColorComponent::Alpha:
            return mCurrentColor.alpha();
        default:
            return -1;
    }
}

int ColorWidget::hue() const
{
    if (mCurrentColor.hue() >= 0) return mCurrentColor.hue();
    else
        return mExplicitHue;
}

void ColorWidget::alterColor(QColor &color, int value, ColorComponent component) const
{
    int h, s, v, a;
    color.getHsv(&h, &s, &v, &a);

    const int clippedValue = std::min(std::max(0, value), componentRange(component));

    switch (component)
    {
        case ColorComponent::Red:
            color.setRed(clippedValue);
            return;
        case ColorComponent::Green:
            color.setGreen(clippedValue);
            return;
        case ColorComponent::Blue:
            color.setBlue(clippedValue);
            return;
        case ColorComponent::Hue:
            color.setHsv(clippedValue, s, v, a);
            return;
        case ColorComponent::Saturation:
            color.setHsv(h, clippedValue, v, a);
            return;
        case ColorComponent::Value:
            color.setHsv(h, s, clippedValue, a);
            return;
        case ColorComponent::Alpha:
            color.setAlpha(clippedValue);
            return;
        default:
            return;
    }
}

const QPixmap &ColorWidget::transparentBackground()
{
    static QPixmap pixmap;
    if (pixmap.isNull())
    {
        pixmap = QPixmap(":/resources/transp-background_8x8.png");
    }
    return pixmap;
}