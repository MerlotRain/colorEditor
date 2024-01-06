#ifndef COLORWIDGETACTION_H
#define COLORWIDGETACTION_H

#include <QMenu>
#include <QWidget>
#include <QWidgetAction>
#include <coloreditor_global.h>

class ColorWidget;
class ColorWidgetActionPrivate;
class COLOR_EDITOR_EXPORT ColorWidgetAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit ColorWidgetAction(ColorWidget *cw, QMenu *menu = nullptr,
                               QWidget *parent = nullptr);
    ~ColorWidgetAction();

    ColorWidget *colorWidget();
    void setDismissOnColorSelection(bool dismiss);
    bool dismissOnColorSelection();

signals:
    void colorChanged(const QColor &color);

private:
    friend class ColorWidgetActionPrivate;
    ColorWidgetActionPrivate *d;
};

#endif//COLORWIDGETACTION_H