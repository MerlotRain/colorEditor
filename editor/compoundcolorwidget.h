#ifndef COMPOUNDCOLORWIDGET_H
#define COMPOUNDCOLORWIDGET_H

#include "ui_compoundcolorwidget.h"
#include <QWidget>
#include <coloreditor_global.h>

class COLOR_EDITOR_EXPORT CompoundColorWidget : public QWidget, Ui::CompoundColorWidget
{
    Q_OBJECT

public:
    explicit CompoundColorWidget(QWidget *parent = nullptr);
    ~CompoundColorWidget();
};

#endif// COMPOUNDCOLORWIDGET_H
