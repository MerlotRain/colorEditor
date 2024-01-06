#ifndef COLORBOX_H
#define COLORBOX_H

#include "../colorwidget.h"

class ColorBoxPrivate;
class COLOR_EDITOR_EXPORT ColorBox : public ColorWidget
{
    Q_OBJECT
public:
    explicit ColorBox(QWidget *parent = nullptr, ColorComponent component = Multiple);
    ~ColorBox();

    QSize sizeHint() const override;
    void paintEvent(QPaintEvent *event) override;
    void setComponent(ColorComponent component) override;

public slots:
    void setColor(const QColor &color, bool emitSignals = false) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    ColorBoxPrivate *d;
    friend class ColorBoxPrivate;
};

#endif//COLORBOX_H