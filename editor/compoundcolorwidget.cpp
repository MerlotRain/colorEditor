#include "compoundcolorwidget.h"
#include "ui_compoundcolorwidget.h"

CompoundColorWidget::CompoundColorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CompoundColorWidget)
{
    ui->setupUi(this);
}

CompoundColorWidget::~CompoundColorWidget()
{
    delete ui;
}
