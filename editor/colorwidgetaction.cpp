#include "colorwidgetaction.h"
#include "colorwidget.h"
#include <QAction>

class ColorWidgetActionPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ColorWidgetActionPrivate(ColorWidget *cw, QMenu *menu,
                                      ColorWidgetAction *ptr);

    QMenu *menu;
    ColorWidget *colorWidget;
    bool suppressRecurse;
    bool dismissOnColorSelection;
    ColorWidgetAction *q;

    void onHover();
    void setColor(const QColor &color);
};


/* ------------------------ ColorWidgetActionPrivate ------------------------ */

ColorWidgetActionPrivate::ColorWidgetActionPrivate(ColorWidget *cw,
                                                   QMenu *_menu,
                                                   ColorWidgetAction *ptr)
    : q(ptr), menu(_menu), colorWidget(cw), suppressRecurse(false),
      dismissOnColorSelection(true)
{
    connect(colorWidget, &ColorWidget::colorChanged, this,
            &ColorWidgetActionPrivate::setColor);
    connect(colorWidget, &ColorWidget::hovered, this,
            &ColorWidgetActionPrivate::onHover);
    connect(q, &ColorWidgetAction::hovered, this,
            &ColorWidgetActionPrivate::onHover);
}

void ColorWidgetActionPrivate::onHover()
{
    if (suppressRecurse) return;

    if (menu)
    {
        suppressRecurse = true;
        menu->setActiveAction(q);
        suppressRecurse = false;
    }
}

void ColorWidgetActionPrivate::setColor(const QColor &color)
{
    emit q->colorChanged(color);
    if (menu && dismissOnColorSelection)
    {
        //QAction::trigger();
        menu->hide();
    }
}

/* ---------------------------- ColorWidgetAction --------------------------- */

ColorWidgetAction::ColorWidgetAction(ColorWidget *cw, QMenu *menu,
                                     QWidget *parent)
    : QWidgetAction(parent), d(new ColorWidgetActionPrivate(cw, menu, this))
{
    setDefaultWidget(d->colorWidget);
}

ColorWidgetAction::~ColorWidgetAction() {}

ColorWidget *ColorWidgetAction::colorWidget() { return d->colorWidget; }

void ColorWidgetAction::setDismissOnColorSelection(bool dismiss)
{
    d->dismissOnColorSelection = dismiss;
}

bool ColorWidgetAction::dismissOnColorSelection()
{
    return d->dismissOnColorSelection;
}

#include "colorwidgetaction.moc"