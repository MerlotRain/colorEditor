#ifndef COLORSLIDERWIDGET_H
#define COLORSLIDERWIDGET_H

#include "../colorwidget.h"

class ColorSliderWidgetPrivate;
class COLOR_EDITOR_EXPORT ColorSliderWidget : public ColorWidget
{
    Q_OBJECT
public:
    explicit ColorSliderWidget(QWidget *parent = nullptr,
                               ColorComponent component = ColorComponent::Red);
    void setComponent(ColorComponent component) override;
    void setComponentValue(int value) override;
    void setColor(const QColor &color, bool emitSignals = false) override;

private:
    void spinValueChanged(int value);

private:
    ColorSliderWidgetPrivate *d;
    friend class ColorSliderWidgetPrivate;
};

#endif//COLORSLIDERWIDGET_H