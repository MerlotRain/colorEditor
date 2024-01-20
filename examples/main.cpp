#include <QApplication>
#include <colorcombobox.h>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    ColorComboBox combo;
    combo.show();
    return app.exec();
}