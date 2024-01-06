#include "colortextwidget.h"
#include <QLineEdit>
#include <QToolButton>

class ColorTextWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    ColorTextWidgetPrivate(ColorTextWidget *ptr);

    QLineEdit *lineEdit = nullptr;
    QToolButton *menuButton = nullptr;
    ColorTextWidget::ColorTextFormat format = ColorTextWidget::HexRgb;
    bool allowAlpha = true;

    void updateText();
    void textChanged();
    void showMenu();
};

/* ----------------------------- ColorTextWidget ---------------------------- */

ColorTextWidget::ColorTextWidget(QWidget *parent) {}

void ColorTextWidget::setColor(const QColor &color, bool emitSignals) {}

void ColorTextWidget::setAllowOpacity(bool allow) {}

void ColorTextWidget::resizeEvent(QResizeEvent *event) {}

/* ------------------------- ColorTextWidgetPrivate ------------------------- */

void ColorTextWidgetPrivate::updateText() {}

void ColorTextWidgetPrivate::textChanged() {}

void ColorTextWidgetPrivate::showMenu() {}

#include "colortextwidget.moc"