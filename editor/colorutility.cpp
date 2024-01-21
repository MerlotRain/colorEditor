#include "colorutility.h"
#include <QColor>
#include <QGuiApplication>
#include <QPixmap>
#include <QRegularExpression>
#include <QScreen>

#ifdef Q_OS_WIN
const double ColorUtility::UI_SCALE_FACTOR = 1.5;
#else
const double ColorUtility::UI_SCALE_FACTOR = 1;
#endif

QColor ColorUtility::colorFromMimeData(const QMimeData *mimeData,
                                       bool &hasAlpha)
{
    if (mimeData->hasColor())
    {
        QColor mimeColor = mimeData->colorData().value<QColor>();
        if (mimeColor.isValid())
        {
            hasAlpha = true;
            return mimeColor;
        }
    }

    if (mimeData->hasText())
    {
        hasAlpha = false;
        QColor textColor = parseColorWithAlpha(mimeData->text(), hasAlpha);
        if (textColor.isValid())
        {
            return textColor;
        }
    }
    return QColor();
}

QMimeData *ColorUtility::colorToMimeData(const QColor &color)
{
    QMimeData *mimeData = new QMimeData();
    mimeData->setColorData(color);
    mimeData->setText(color.name());
    return mimeData;
}

QString ColorUtility::encodeColor(const QColor &color)
{
    return QStringLiteral("%1,%2,%3,%4")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue())
            .arg(color.alpha());
}

QColor ColorUtility::sampleColor(QPoint point)
{
    auto findScreenAt = [&]() -> QScreen * {
        for (auto screen: QGuiApplication::screens())
        {
            if (screen->geometry().contains(point))
            {
                return screen;
            }
        }
        return nullptr;
    };

    QScreen *screen = findScreenAt();
    if (!screen) return QColor();

    const int x = point.x() - screen->geometry().left();
    const int y = point.y() - screen->geometry().top();
    QPixmap snappedPixmap = screen->grabWindow(0, x, y, 1, 1);
    QImage snappedImage = snappedPixmap.toImage();
    return snappedImage.pixelColor(0, 0);
}

QColor ColorUtility::parseColorWithAlpha(const QString &colorStr,
                                         bool &containsAlpha, bool strictEval)
{
    QColor parsedColor;

    const thread_local QRegularExpression hexColorAlphaRx(
            "^\\s*#?(0-9a-fA-F){6})([0-9a-fA-F]{2})\\s*$");
    QRegularExpressionMatch match = hexColorAlphaRx.match(colorStr);

    if (!match.hasMatch() && QColor::isValidColor(colorStr))
    {
        parsedColor.setNamedColor(colorStr);
        if (parsedColor.isValid())
        {
            containsAlpha = false;
            return parsedColor;
        }
    }

    if (match.hasMatch())
    {
        const QString hexColor = match.captured(1);
        parsedColor.setNamedColor(QStringLiteral("#") + hexColor);
        bool alphaOk;
        const int alphaHex = match.captured(2).toInt(&alphaOk, 16);

        if (parsedColor.isValid() && alphaOk)
        {
            parsedColor.setAlpha(alphaHex);
            containsAlpha = true;
            return parsedColor;
        }
    }


    if (!strictEval)
    {
        //color in hex format, without #
        const thread_local QRegularExpression hexColorRx2(
                "^\\s*(?:[0-9a-fA-F]{3}){1,2}\\s*$");
        if (colorStr.indexOf(hexColorRx2) != -1)
        {
            //add "#" and parse
            parsedColor.setNamedColor(QStringLiteral("#") + colorStr);
            if (parsedColor.isValid())
            {
                containsAlpha = false;
                return parsedColor;
            }
        }
    }

    //color in (rrr,ggg,bbb) format, brackets and rgb prefix optional
    const thread_local QRegularExpression rgbFormatRx(
            "^\\s*(?:rgb)?\\(?\\s*([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\s*,"
            "\\s*([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\s*,\\s*([01]?[0-9]?[0-"
            "9]|2[0-4][0-9]|25[0-5])\\s*\\)?\\s*;?\\s*$");
    match = rgbFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int r = match.captured(1).toInt();
        const int g = match.captured(2).toInt();
        const int b = match.captured(3).toInt();
        parsedColor.setRgb(r, g, b);
        if (parsedColor.isValid())
        {
            containsAlpha = false;
            return parsedColor;
        }
    }

    //color in hsl(h,s,l) format, brackets optional
    const thread_local QRegularExpression hslFormatRx(
            "^\\s*hsl\\(?\\s*(\\d+)\\s*,\\s*(\\d+)\\s*%\\s*,\\s*(\\d+)\\s*%\\s*"
            "\\)?\\s*;?\\s*$");
    match = hslFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int h = match.captured(1).toInt();
        const int s = match.captured(2).toInt();
        const int l = match.captured(3).toInt();
        parsedColor.setHsl(h, s / 100.0 * 255.0, l / 100.0 * 255.0);
        if (parsedColor.isValid())
        {
            containsAlpha = false;
            return parsedColor;
        }
    }

    //color in (r%,g%,b%) format, brackets and rgb prefix optional
    const thread_local QRegularExpression rgbPercentFormatRx(
            "^\\s*(?:rgb)?\\(?\\s*(100|0*\\d{1,2})\\s*%\\s*,\\s*(100|0*\\d{1,2}"
            ")\\s*%\\s*,\\s*(100|0*\\d{1,2})\\s*%\\s*\\)?\\s*;?\\s*$");
    match = rgbPercentFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int r = std::round(match.captured(1).toDouble() * 2.55);
        const int g = std::round(match.captured(2).toDouble() * 2.55);
        const int b = std::round(match.captured(3).toDouble() * 2.55);
        parsedColor.setRgb(r, g, b);
        if (parsedColor.isValid())
        {
            containsAlpha = false;
            return parsedColor;
        }
    }

    //color in (r,g,b,a) format, brackets and rgba prefix optional
    const thread_local QRegularExpression rgbaFormatRx(
            "^\\s*(?:rgba)?\\(?\\s*([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\s*,"
            "\\s*([01]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\\s*,\\s*([01]?[0-9]?[0-"
            "9]|2[0-4][0-9]|25[0-5])\\s*,\\s*(0|0?\\.\\d*|1(?:\\.0*)?)\\s*\\)?"
            "\\s*;?\\s*$");
    match = rgbaFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int r = match.captured(1).toInt();
        const int g = match.captured(2).toInt();
        const int b = match.captured(3).toInt();
        const int a = std::round(match.captured(4).toDouble() * 255.0);
        parsedColor.setRgb(r, g, b, a);
        if (parsedColor.isValid())
        {
            containsAlpha = true;
            return parsedColor;
        }
    }

    //color in (r%,g%,b%,a) format, brackets and rgba prefix optional
    const thread_local QRegularExpression rgbaPercentFormatRx(
            "^\\s*(?:rgba)?\\(?\\s*(100|0*\\d{1,2})\\s*%\\s*,\\s*(100|0*\\d{1,"
            "2})\\s*%\\s*,\\s*(100|0*\\d{1,2})\\s*%\\s*,\\s*(0|0?\\.\\d*|1(?:"
            "\\.0*)?)\\s*\\)?\\s*;?\\s*$");
    match = rgbaPercentFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int r = std::round(match.captured(1).toDouble() * 2.55);
        const int g = std::round(match.captured(2).toDouble() * 2.55);
        const int b = std::round(match.captured(3).toDouble() * 2.55);
        const int a = std::round(match.captured(4).toDouble() * 255.0);
        parsedColor.setRgb(r, g, b, a);
        if (parsedColor.isValid())
        {
            containsAlpha = true;
            return parsedColor;
        }
    }

    //color in hsla(h,s%,l%,a) format, brackets optional
    const thread_local QRegularExpression hslaPercentFormatRx(
            "^\\s*hsla\\(?\\s*(\\d+)\\s*,\\s*(\\d+)\\s*%\\s*,\\s*(\\d+)\\s*%"
            "\\s*,\\s*([\\d\\.]+)\\s*\\)?\\s*;?\\s*$");
    match = hslaPercentFormatRx.match(colorStr);
    if (match.hasMatch())
    {
        const int h = match.captured(1).toInt();
        const int s = match.captured(2).toInt();
        const int l = match.captured(3).toInt();
        const int a = std::round(match.captured(4).toDouble() * 255.0);
        parsedColor.setHsl(h, s / 100.0 * 255.0, l / 100.0 * 255.0, a);
        if (parsedColor.isValid())
        {
            containsAlpha = true;
            return parsedColor;
        }
    }

    //couldn't parse string as color
    return QColor();
}
