#ifndef COLORPREVIEWWIDGET_H
#define COLORPREVIEWWIDGET_H

#include "../colorwidget.h"

class ColorPreviewWidgetPrivate;
class COLOR_EDITOR_EXPORT ColorPreviewWidget : public ColorWidget
{
    Q_OBJECT
public:
    explicit ColorPreviewWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

    QColor color2() const;

public Q_SLOTS:
    void setColor2(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    ColorPreviewWidgetPrivate *d;
    friend class ColorPreviewWidgetPrivate;
};

#endif//COLORPREVIEWWIDGET_H