#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QColor>
#include <QMimeData>
#include <QPoint>
#include <coloreditor_global.h>

class ColorUtility
{
public:
    static const double UI_SCALE_FACTOR;

    static QColor colorFromMimeData(const QMimeData *mimeData, bool &hasAlpha);

    static QMimeData *colorToMimeData(const QColor &color);

    static QString encodeColor(const QColor &color);

    static QColor sampleColor(QPoint point);
};

#endif//COLORUTILITY_H