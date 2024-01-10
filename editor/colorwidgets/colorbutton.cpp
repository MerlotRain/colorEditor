#include "colorbutton.h"
#include "../colorutility.h"
#include "../colorwidget.h"
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDrag>
#include <QMenu>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionToolButton>

class ColorButtonPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ColorButtonPrivate(ColorButton *ptr);
    ~ColorButtonPrivate();

    ColorButton *q;

    ColorButton::Behavior behavior = ColorButton::ShowDialog;
    QString colorDialogTitle;
    QColor color;
    QSize minimumSize;

    QColor defaultColor;

    QColor currentColor;
    QString context;
    bool allowOpacity = false;
    bool colorSet = false;

    bool showNoColorOption = false;
    QString noColorString;
    bool showNull;
    QString nullColorString;

    QPoint dragStartPosition;
    bool pickingColor;

    QMenu *menu = nullptr;
    QSize iconSize;
    QString linkedColorName;
    bool showMenu = true;

    bool colorFromMimeData(const QMimeData *mimeData, QColor &resultColor);
    void stopPicking(QPoint eventPos, bool samplingColor = true);
    QColor linkedProjectColor() const;


    void buttonClicked();
    void showColorDialog();
    void setValidTemporaryColor(const QColor &color);
    void addRecentColor(const QColor &color);
    void prepareMenu();
};

/* ------------------------------- ColorButton ------------------------------ */

ColorButton::ColorButton(QWidget *parent, const QString &cdt)
    : QToolButton(parent), d(new ColorButtonPrivate(this))
{
    d->colorDialogTitle = cdt.isEmpty() ? tr("Select Color") : cdt;
    d->noColorString = tr("No color");

    setAcceptDrops(true);
    setMinimumSize(QSize(24, 16));
}

QSize ColorButton::minimumSizeHint() const { return d->minimumSize; }

QSize ColorButton::sizeHint() const { return d->minimumSize; }

QColor ColorButton::color() const
{
    QColor c = d->linkedProjectColor();
    if (!c.isValid())
    {
        c = d->color;
    }
    return c;
}

void ColorButton::setAllowOpacity(bool allow) { d->allowOpacity = allow; }

bool ColorButton::allowOpacity() const { return d->allowOpacity; }

void ColorButton::setColorDialogTitle(const QString &title)
{
    d->colorDialogTitle = title;
}

QString ColorButton::colorDialogTitle() const { return d->colorDialogTitle; }

void ColorButton::setShowMenu(bool show)
{
    d->showMenu = show;
    setMenu(d->showMenu ? d->menu : nullptr);
    setPopupMode(d->showMenu ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);
    d->iconSize = QSize();
    //d->setButtonBackground();
}

bool ColorButton::showMenu() const { return d->menu ? true : false; }

void ColorButton::setBehavior(Behavior behavior) { d->behavior = behavior; }

ColorButton::Behavior ColorButton::behavior() const { return d->behavior; }

void ColorButton::setDefaultColor(const QColor &color) { d->defaultColor = color; }

QColor ColorButton::defaultColor() const { return d->defaultColor; }

void ColorButton::setShowNoColor(const bool show) { d->showNoColorOption = show; }

bool ColorButton::showNoColor() const { return d->showNoColorOption; }

void ColorButton::setNoColorString(const QString &str) { d->noColorString = str; }

void ColorButton::setShowNull(bool show, const QString &nullString)
{
    d->showNull = show;
    d->nullColorString = nullString;
}

bool ColorButton::showNull() const { return d->showNull; }

bool ColorButton::isNull() const { return !(d->color.isValid()); }

QString ColorButton::noColorString() const { return d->noColorString; }

void ColorButton::setContext(const QString &str) { d->context = str; }

QString ColorButton::context() const { return d->context; }

void ColorButton::setColor(const QColor &color)
{
    const QColor oldColor = d->color;
    d->color = color;

    if (oldColor != color || (d->color == QColor(Qt::black) && !(d->colorSet)))
    {
        setButtonBackground();
        if (isEnabled())
        {
            emit colorChanged(d->color);
        }
    }
    d->colorSet = true;
}

void ColorButton::setButtonBackground(const QColor &color)
{
    QColor backgroundColor = color;
    bool isProjectColor = false;
    if (!backgroundColor.isValid() && !d->linkedColorName.isEmpty())
    {
        backgroundColor = d->linkedProjectColor();
        isProjectColor = backgroundColor.isValid();
        if (!isProjectColor)
        {
            d->linkedColorName.clear();//color has been deleted, renamed, etc...
            emit unlinked();
        }
    }
    if (!backgroundColor.isValid())
    {
        backgroundColor = d->color;
    }

    QSize currentIconSize;
    //icon size is button size with a small margin
    if (menu())
    {
        if (!d->iconSize.isValid())
        {
            //calculate size of push button part of widget (ie, without the menu drop-down button part)
            QStyleOptionToolButton opt;
            initStyleOption(&opt);
            const QRect buttonSize = QApplication::style()->subControlRect(
                    QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButton, this);
            //make sure height of icon looks good under different platforms
#ifdef Q_OS_WIN
            d->iconSize = QSize(buttonSize.width() - 10, height() - 6);
#else
            d->iconSize = QSize(buttonSize.width() - 10, height() - 12);
#endif
        }
        currentIconSize = d->iconSize;
    }
    else
    {
        //no menu
#ifdef Q_OS_WIN
        currentIconSize = QSize(width() - 10, height() - 6);
#else
        currentIconSize = QSize(width() - 10, height() - 12);
#endif
    }

    if (!currentIconSize.isValid() || currentIconSize.width() <= 0 ||
        currentIconSize.height() <= 0)
    {
        return;
    }

    //create an icon pixmap
    const double pixelRatio = devicePixelRatioF();
    QPixmap pixmap(currentIconSize * pixelRatio);
    pixmap.setDevicePixelRatio(pixelRatio);
    pixmap.fill(Qt::transparent);

    if (backgroundColor.isValid())
    {
        const QRectF rect(0, 0, currentIconSize.width(), currentIconSize.height());
        QPainter p;
        p.begin(&pixmap);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        if (d->allowOpacity && backgroundColor.alpha() < 255)
        {
            //start with check board pattern
            const QBrush checkBrush = QBrush(transparentBackground());
            p.setBrush(checkBrush);
            p.drawRoundedRect(rect, 3, 3);
        }

        //draw semi-transparent color on top
        p.setBrush(backgroundColor);
        p.drawRoundedRect(rect, 3, 3);
        p.end();
    }

    setIconSize(currentIconSize);
    setIcon(pixmap);
}

void ColorButton::copyColor()
{
    //copy color
    QColor c = d->linkedProjectColor();
    if (!c.isValid()) c = d->color;
    QApplication::clipboard()->setMimeData(ColorUtility::colorToMimeData(c));
}

void ColorButton::pasteColor()
{
    QColor clipColor;
    if (d->colorFromMimeData(QApplication::clipboard()->mimeData(), clipColor))
    {
        //paste color
        setColor(clipColor);
        d->addRecentColor(clipColor);
    }
}

void ColorButton::activatePicker()
{
    //activate picker color
    // Store current color
    d->currentColor = d->color;
    QApplication::setOverrideCursor(QCursor());
    grabMouse();
    grabKeyboard();
    d->pickingColor = true;
    setMouseTracking(true);
}

void ColorButton::setToNoColor()
{
    QColor noColor = QColor(d->color);
    noColor.setAlpha(0);
    setColor(noColor);
}

void ColorButton::setToDefaultColor()
{
    if (!d->defaultColor.isValid())
    {
        return;
    }

    setColor(d->defaultColor);
}

void ColorButton::setToNull()
{
    setColor(QColor());
    emit cleared();
}

void ColorButton::unlink()
{
    // d->linkToProjectColor(QString());
    emit unlinked();
}

bool ColorButton::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip && isEnabled())
    {
        QColor c = d->linkedProjectColor();
        const bool isProjectColor = c.isValid();
        if (!isProjectColor) c = d->color;

        const QString name = c.name();
        const int hue = c.hue();
        const int value = c.value();
        const int saturation = c.saturation();

        // create very large preview swatch
        const int width = static_cast<int>(fontMetrics().horizontalAdvance('X') * 23);
        const int height = static_cast<int>(width / 1.61803398875);// golden ratio

        const int margin = static_cast<int>(height * 0.1);
        QImage icon =
                QImage(width + 2 * margin, height + 2 * margin, QImage::Format_ARGB32);
        icon.fill(Qt::transparent);

        QPainter p;
        p.begin(&icon);

        //start with checkboard pattern
        const QBrush checkBrush = QBrush(transparentBackground());
        p.setPen(Qt::NoPen);
        p.setBrush(checkBrush);
        p.drawRect(margin, margin, width, height);

        //draw color over pattern
        p.setBrush(QBrush(c));

        //draw border
        p.setPen(QColor(197, 197, 197));
        p.drawRect(margin, margin, width, height);
        p.end();

        QByteArray data;
        QBuffer buffer(&data);
        icon.save(&buffer, "PNG", 100);

        const QString info =
                (isProjectColor ? QStringLiteral("<p>%1: %2</p>")
                                          .arg(tr("Linked color"), d->linkedColorName)
                                : QString()) +
                QStringLiteral("<b>HEX</b> %1<br>"
                               "<b>RGB</b> %2<br>"
                               "<b>HSV</b> %3,%4,%5<p>"
                               "<img src='data:image/png;base64, %0'>")
                        .arg(QString(data.toBase64()), name, ColorUtility::encodeColor(c))
                        .arg(hue)
                        .arg(saturation)
                        .arg(value);
        setToolTip(info);
    }
    return QToolButton::event(event);
}

void ColorButton::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange)
    {
        setButtonBackground();
    }
    QToolButton::changeEvent(event);
}

void ColorButton::showEvent(QShowEvent *event)
{
    setButtonBackground();
    QToolButton::showEvent(event);
}

void ColorButton::resizeEvent(QResizeEvent *event) {}

const QPixmap &ColorButton::transparentBackground()
{
    static QPixmap pixmap;
    return pixmap;
}

void ColorButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (d->pickingColor)
    {
        //end color picking operation by sampling the color under cursor
        d->stopPicking(event->globalPosition().toPoint());
        event->accept();
        return;
    }

    QToolButton::mouseReleaseEvent(event);
}

void ColorButton::mousePressEvent(QMouseEvent *event)
{
    if (d->pickingColor)
    {
        //don't show dialog if in color picker mode
        event->accept();
        return;
    }

    if (event->button() == Qt::RightButton)
    {
        QToolButton::showMenu();
        return;
    }
    else if (event->button() == Qt::LeftButton)
    {
        d->dragStartPosition = event->pos();
    }
    QToolButton::mousePressEvent(event);
}

void ColorButton::mouseMoveEvent(QMouseEvent *event)
{
    if (d->pickingColor)
    {
        setButtonBackground(ColorUtility::sampleColor(event->globalPosition().toPoint()));
        event->accept();
        return;
    }

    //handle dragging colors from button
    QColor c = d->linkedProjectColor();
    if (!c.isValid()) c = d->color;

    if (!(event->buttons() & Qt::LeftButton) || !c.isValid())
    {
        //left button not depressed or no color set, so not a drag
        QToolButton::mouseMoveEvent(event);
        return;
    }

    if ((event->pos() - d->dragStartPosition).manhattanLength() <
        QApplication::startDragDistance())
    {
        //mouse not moved, so not a drag
        QToolButton::mouseMoveEvent(event);
        return;
    }

    //user is dragging color
    QDrag *drag = new QDrag(this);
    drag->setMimeData(ColorUtility::colorToMimeData(c));
    drag->setPixmap(ColorWidget::createDragIcon(c));
    drag->exec(Qt::CopyAction);
    setDown(false);
}

void ColorButton::keyPressEvent(QKeyEvent *event)
{
    if (!d->pickingColor)
    {
        //if not picking a color, use default tool button behavior
        QToolButton::keyPressEvent(event);
        return;
    }

    //cancel picking, sampling the color if space was pressed
    d->stopPicking(QCursor::pos(), event->key() == Qt::Key_Space);
}

void ColorButton::dragEnterEvent(QDragEnterEvent *event)
{
    const bool isProjectColor = d->linkedProjectColor().isValid();
    if (isProjectColor) return;

    //is dragged data valid color data?
    QColor mimeColor;
    if (d->colorFromMimeData(event->mimeData(), mimeColor))
    {
        //if so, we accept the drag, and temporarily change the button's color
        //to match the dragged color. This gives immediate feedback to the user
        //that colors can be dropped here
        event->acceptProposedAction();
        setButtonBackground(mimeColor);
    }
}

void ColorButton::dragLeaveEvent(QDragLeaveEvent *event)
{

    Q_UNUSED(event)
    //reset button color
    setButtonBackground();
}

void ColorButton::dropEvent(QDropEvent *event)
{
    const bool isProjectColor = d->linkedProjectColor().isValid();
    if (isProjectColor) return;

    //is dropped data valid color data?
    QColor mimeColor;
    if (d->colorFromMimeData(event->mimeData(), mimeColor))
    {
        //accept drop and set new color
        event->acceptProposedAction();
        setColor(mimeColor);
        d->addRecentColor(mimeColor);
    }
}

void ColorButton::wheelEvent(QWheelEvent *event)
{
    if (d->allowOpacity && isEnabled() && !isNull())
    {
        const double increment =
                ((event->modifiers() & Qt::ControlModifier) ? 0.01 : 0.1) *
                (event->angleDelta().y() > 0 ? 1 : -1);
        const double alpha = std::min(std::max(0.0, d->color.alphaF() + increment), 1.0);
        d->color.setAlphaF(alpha);

        setButtonBackground();
        emit colorChanged(d->color);
        event->accept();
    }
    else
    {
        QToolButton::wheelEvent(event);
    }
}

/* --------------------------- ColorButtonPrivate --------------------------- */

ColorButtonPrivate::ColorButtonPrivate(ColorButton *ptr) : q(ptr)
{
    connect(q, &QAbstractButton::clicked, this, &ColorButtonPrivate::buttonClicked);
    menu = new QMenu(q);
    connect(menu, &QMenu::aboutToShow, this, &ColorButtonPrivate::prepareMenu);
    q->setMenu(menu);
    q->setPopupMode(QToolButton::MenuButtonPopup);

#ifdef Q_OS_WIN
    minimumSize = QSize(120, 22);
#else
    minimumSize = QSize(120, 28);
#endif
    minimumSize.setHeight(std::max(static_cast<int>(q->fontMetrics().height() * 1.1),
                                   minimumSize.height()));
}

ColorButtonPrivate::~ColorButtonPrivate() {}

bool ColorButtonPrivate::colorFromMimeData(const QMimeData *mimeData, QColor &resultColor)
{

    bool hasAlpha = false;
    QColor mimeColor = ColorUtility::colorFromMimeData(mimeData, hasAlpha);

    if (mimeColor.isValid())
    {
        if (!allowOpacity)
        {
            //remove alpha channel
            mimeColor.setAlpha(255);
        }
        else if (!hasAlpha)
        {
            //mime color has no explicit alpha component, so keep existing alpha
            mimeColor.setAlpha(color.alpha());
        }
        resultColor = mimeColor;
        return true;
    }

    //could not get color from mime data
    return false;
}

void ColorButtonPrivate::stopPicking(QPoint eventPos, bool samplingColor)
{
    //release mouse and keyboard, and reset cursor
    q->releaseMouse();
    q->releaseKeyboard();
    QApplication::restoreOverrideCursor();
    q->setMouseTracking(false);
    pickingColor = false;

    if (!samplingColor)
    {
        //not sampling color, restore old color
        q->setButtonBackground();
        return;
    }

    q->setColor(ColorUtility::sampleColor(eventPos));
    addRecentColor(color);
}

QColor ColorButtonPrivate::linkedProjectColor() const { return QColor(); }

void ColorButtonPrivate::buttonClicked()
{
    if (linkedProjectColor().isValid())
    {
        q->showMenu();
    }
    else
    {
        switch (behavior)
        {
            case ColorButton::ShowDialog:
                showColorDialog();
                return;
            case ColorButton::SignalOnly:
                emit q->colorClicked(color);
                return;
        }
    }
}

void ColorButtonPrivate::showColorDialog() {}

void ColorButtonPrivate::setValidTemporaryColor(const QColor &color) {}

void ColorButtonPrivate::addRecentColor(const QColor &color) {}

void ColorButtonPrivate::prepareMenu() {}

#include "colorbutton.moc"