#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <coloreditor_global.h>

class COLOR_EDITOR_EXPORT ColorWidget : public QWidget
{
    Q_OBJECT
public:
    enum ColorComponent
    {
        Multiple,
        Red,
        Green,
        Blue,
        Hue,
        Saturation,
        Value,
        Alpha,
    };

    explicit ColorWidget(QWidget *parent = nullptr, ColorComponent component = Multiple);
    QColor color() const;
    ColorComponent component() const;
    int componentValue() const;

    static QPixmap createDragIcon(const QColor &color);

public slots:
    virtual void setColor(const QColor &color, bool emitSignals = false);
    virtual void setComponent(ColorComponent component);
    virtual void setComponentValue(int value);

signals:
    void colorChanged(const QColor &color);
    void hovered();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    int componentRange() const;
    int componentRange(ColorComponent component) const;
    int componentValue(ColorComponent component) const;
    int hue() const;
    void alterColor(QColor &color, int value, ColorComponent component) const;

    static const QPixmap &transparentBackground();

    QColor mCurrentColor;
    ColorComponent mComponent;
    int mExplicitHue = 0;
};

#endif//COLORWIDGET_H