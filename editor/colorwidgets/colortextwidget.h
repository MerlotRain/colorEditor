#ifndef COLORTEXTWIDGET_H
#define COLORTEXTWIDGET_H

#include "../colorwidget.h"

class ColorTextWidgetPrivate;
class COLOR_EDITOR_EXPORT ColorTextWidget : public ColorWidget
{
    Q_OBJECT
public:
    enum ColorTextFormat
    {
        HexRgb,
        HexRgba,
        Rgb,
        Rgba
    };
    Q_ENUM(ColorTextFormat)

    explicit ColorTextWidget(QWidget *parent = nullptr);
    ~ColorTextWidget();
    void setColor(const QColor &color, bool emitSignals = false) override;
    void setAllowOpacity(bool allow);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ColorTextWidgetPrivate *d;
    friend class ColorTextWidgetPrivate;
};

#endif//COLORTEXTWIDGET_H