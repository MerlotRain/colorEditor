#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QColor>
#include <QMimeData>
#include <coloreditor_global.h>

class ColorUtility
{
public:
    static QColor colorFromMimeData(const QMimeData *mimeData, bool &hasAlpha);
};

#endif//COLORUTILITY_H