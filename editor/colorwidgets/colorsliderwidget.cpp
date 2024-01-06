#include "colorsliderwidget.h"
#include "colorrampwidget.h"
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

ColorSliderWidget::ColorSliderWidget(QWidget *parent, ColorComponent component) {}

void ColorSliderWidget::setComponent(ColorComponent component) {}

void ColorSliderWidget::setComponentValue(int value) {}

void ColorSliderWidget::setColor(const QColor &color, bool emitSignals) {}

/* ------------------------ ColorSliderWidgetPrivate ------------------------ */

ColorSliderWidgetPrivate::ColorSliderWidgetPrivate(ColorSliderWidget *ptr) {}

int ColorSliderWidgetPrivate::convertRealToDisplay(int realValue) const { return 0; }

int ColorSliderWidgetPrivate::convertDisplayToReal(int displayValue) const { return 0; }

void ColorSliderWidgetPrivate::rampColorChanged(const QColor &color) {}

void ColorSliderWidgetPrivate::spinChanged(int value) {}

void ColorSliderWidgetPrivate::rampChanged(int value) {}


#include "colorsliderwidget.moc"