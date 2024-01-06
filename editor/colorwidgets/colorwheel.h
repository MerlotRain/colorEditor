#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include "../colorwidget.h"

class ColorWheelPrivate;
class COLOR_EDITOR_EXPORT ColorWheel : public ColorWidget
{
    Q_OBJECT
public:
    explicit ColorWheel(QWidget *parent = nullptr);
    ~ColorWheel();

    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;

public Q_SLOTS:
    void setColor(const QColor &color, bool emitSignals = false) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    ColorWheelPrivate *d;
    friend class ColorWheelPrivate;
};

#endif//COLORWHEEL_H