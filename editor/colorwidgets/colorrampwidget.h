#ifndef COLORRAMPWIDGET_H
#define COLORRAMPWIDGET_H

#include "../colorwidget.h"

class ColorRampWidgetPrivate;
class ColorRampWidget : public ColorWidget
{
    Q_OBJECT
public:
    enum Orientation
    {
        Horizontal,
        Vertical
    };
    explicit ColorRampWidget(QWidget *parent = nullptr,
                             ColorComponent component = ColorComponent::Red,
                             Orientation orientation = Horizontal);
    ~ColorRampWidget();

    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

    void setOrientation(Orientation orientation);
    Orientation orientation() const;

    void setInteriorMargin(int margin);
    int interiorMargin() const;
    void setShowFrame(bool show);
    bool showFrame() const;
    void setMarkerSize(int size);

signals:
    void valueChanged(int value);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    ColorRampWidgetPrivate *d;
    friend class ColorRampWidgetPrivate;
};

#endif//COLORRAMPWIDGET_H