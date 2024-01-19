#include "colordialog.h"
#include <QAbstractButton>

class ColorDialogPrivate : public QObject
{
    Q_OBJECT
public:
    ColorDialogPrivate(ColorDialog *ptr);
    ~ColorDialogPrivate();

    ColorDialog *q;
    QColor previousColor;
    bool allowOpacity = true;

    void onOk();
    void onCancel();
    void onButtonClicked(QAbstractButton *button);
    void discardColor();
    void showHelp();
};

/* ------------------------------- ColorDialog ------------------------------ */

ColorDialog::ColorDialog(QWidget *parent, Qt::WindowFlags flags,
                         const QColor &color)
{
}

ColorDialog::~ColorDialog() {}

QColor ColorDialog::color() const { return QColor(); }

void ColorDialog::setTitle(const QString &title) {}

void ColorDialog::setAllowOpacity(bool allow) {}

QColor ColorDialog::getColor(const QColor &initialColor, QWidget *parent,
                             const QString &title, bool allowOpacity)
{
    return QColor();
}

void ColorDialog::setColor(const QColor &color) {}

void ColorDialog::closeEvent(QCloseEvent *event) {}

/* --------------------------- ColorDialogPrivate --------------------------- */

ColorDialogPrivate::ColorDialogPrivate(ColorDialog *ptr) {}

ColorDialogPrivate::~ColorDialogPrivate() {}

void ColorDialogPrivate::onOk() {}

void ColorDialogPrivate::onCancel() {}

void ColorDialogPrivate::onButtonClicked(QAbstractButton *button) {}

void ColorDialogPrivate::discardColor() {}

void ColorDialogPrivate::showHelp() {}

#include "colordialog.moc"