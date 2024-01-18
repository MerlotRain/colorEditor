#include "colorutility.h"

#ifdef Q_OS_WIN
const double ColorUtility::UI_SCALE_FACTOR = 1.5;
#else
const double ColorUtility::UI_SCALE_FACTOR = 1;
#endif

QColor ColorUtility::colorFromMimeData(const QMimeData *mimeData, bool &hasAlpha)
{
    return QColor();
}

QMimeData *ColorUtility::colorToMimeData(const QColor &color) { return nullptr; }

QString ColorUtility::encodeColor(const QColor &color) { return QString(); }

QColor ColorUtility::sampleColor(QPoint point) { return QColor(); }

QColor ColorUtility::parseColorWithAlpha(const QString &colorStr, bool &containsAlpha,
                                         bool strictEval)
{
    return QColor();
}
