#ifndef COMPOUNDCOLORWIDGET_H
#define COMPOUNDCOLORWIDGET_H

#include <QWidget>

namespace Ui {
class CompoundColorWidget;
}

class CompoundColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompoundColorWidget(QWidget *parent = nullptr);
    ~CompoundColorWidget();

private:
    Ui::CompoundColorWidget *ui;
};

#endif // COMPOUNDCOLORWIDGET_H
