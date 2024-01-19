#include "colorsliderwidget.h"
#include "colorrampwidget.h"
#include "colorwidget.h"
#include <QHBoxLayout>
#include <QSpinBox>

class ColorSliderWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ColorSliderWidgetPrivate(ColorSliderWidget *ptr);

    ColorSliderWidget *q;
    ColorRampWidget *rampWidget = nullptr;
    QSpinBox *spinBox = nullptr;

    int convertRealToDisplay(int realValue) const;
    int convertDisplayToReal(int displayValue) const;

    void rampColorChanged(const QColor &color);
    void spinChanged(int value);
    void rampChanged(int value);
};

/* ---------------------------- ColorSliderWidget --------------------------- */

ColorSliderWidget::ColorSliderWidget(QWidget *parent, ColorComponent component)
    : ColorWidget(parent, component), d(new ColorSliderWidgetPrivate(this))
{
}

void ColorSliderWidget::setComponent(ColorComponent component)
{
    ColorWidget::setComponent(component);
    d->rampWidget->setComponent(component);
    d->spinBox->setMaximum(d->convertRealToDisplay(componentRange()));
    if (component == ColorWidget::Hue)
    {
        d->spinBox->setSuffix(QChar(176));
    }
    else if (component == ColorWidget::Saturation ||
             component == ColorWidget::Value || component == ColorWidget::Alpha)
    {
        d->spinBox->setSuffix(tr("%"));
    }
    else
    {
        d->spinBox->setSuffix(QString());
    }
}

void ColorSliderWidget::setComponentValue(int value)
{
    ColorWidget::setComponentValue(value);
    d->rampWidget->blockSignals(true);
    d->rampWidget->setComponentValue(value);
    d->rampWidget->blockSignals(false);
    d->spinBox->blockSignals(true);
    d->spinBox->setValue(d->convertRealToDisplay(value));
    d->spinBox->blockSignals(false);
}

void ColorSliderWidget::setColor(const QColor &color, bool emitSignals)
{
    ColorWidget::setColor(color, emitSignals);
    d->rampWidget->setColor(color);
    d->spinBox->blockSignals(true);
    d->spinBox->setValue(d->convertRealToDisplay(componentValue()));
    d->spinBox->blockSignals(false);
}

void ColorSliderWidget::spinValueChanged(int value)
{
    ColorWidget::setComponentValue(value);
}

/* ------------------------ ColorSliderWidgetPrivate ------------------------ */

ColorSliderWidgetPrivate::ColorSliderWidgetPrivate(ColorSliderWidget *ptr)
    : q(ptr)
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(5);

    rampWidget = new ColorRampWidget(nullptr, q->mComponent);
    rampWidget->setColor(q->mCurrentColor);
    hLayout->addWidget(rampWidget, 1);

    spinBox = new QSpinBox();
    const int largestCharWidth =
            spinBox->fontMetrics().horizontalAdvance(QStringLiteral("888%"));
    spinBox->setMaximumWidth(largestCharWidth + 35);
    spinBox->setMinimum(0);
    spinBox->setMaximum(convertRealToDisplay(q->componentRange()));
    spinBox->setValue(convertRealToDisplay(q->componentValue()));
    if (q->mComponent == ColorWidget::Hue)
    {
        spinBox->setSuffix(QChar(176));
    }
    else if (q->mComponent == ColorWidget::Saturation ||
             q->mComponent == ColorWidget::Value ||
             q->mComponent == ColorWidget::Alpha)
    {
        spinBox->setSuffix(tr("%"));
    }
    hLayout->addWidget(spinBox);
    q->setLayout(hLayout);

    connect(rampWidget, &ColorRampWidget::valueChanged, this,
            &ColorSliderWidgetPrivate::rampChanged);
    connect(rampWidget, &ColorWidget::colorChanged, this,
            &ColorSliderWidgetPrivate::rampColorChanged);
    connect(spinBox, qOverload<int>(&QSpinBox::valueChanged), this,
            &ColorSliderWidgetPrivate::spinChanged);
}

int ColorSliderWidgetPrivate::convertRealToDisplay(int realValue) const
{
    if (q->mComponent == ColorWidget::Saturation ||
        q->mComponent == ColorWidget::Value ||
        q->mComponent == ColorWidget::Alpha)
    {
        return std::round(100. * realValue / 255.);
    }
    return realValue;
}

int ColorSliderWidgetPrivate::convertDisplayToReal(int displayValue) const
{
    if (q->mComponent == ColorWidget::Saturation ||
        q->mComponent == ColorWidget::Value ||
        q->mComponent == ColorWidget::Alpha)
    {
        return std::round(255. * displayValue / 100.);
    }
    return displayValue;
}

void ColorSliderWidgetPrivate::rampColorChanged(const QColor &color)
{
    emit q->colorChanged(color);
}

void ColorSliderWidgetPrivate::spinChanged(int value)
{
    const int convertedValue = convertDisplayToReal(value);
    q->spinValueChanged(value);
    rampWidget->setComponentValue(convertedValue);
    emit q->colorChanged(q->mCurrentColor);
}

void ColorSliderWidgetPrivate::rampChanged(int value)
{
    spinBox->blockSignals(true);
    spinBox->setValue(convertRealToDisplay(value));
    spinBox->blockSignals(false);
}


#include "colorsliderwidget.moc"