#include "colorbutton.h"
#include <QMenu>
#include <QMimeData>

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

ColorButton::ColorButton(QWidget *parent, const QString &cdt) {}

QSize ColorButton::minimumSizeHint() const { return QSize(); }

QSize ColorButton::sizeHint() const { return QSize(); }

QColor ColorButton::color() const { return QColor(); }

void ColorButton::setAllowOpacity(bool allow) {}

bool ColorButton::allowOpacity() const { return false; }

void ColorButton::setColorDialogTitle(const QString &title) {}

QString ColorButton::colorDialogTitle() const { return QString(); }

void ColorButton::setShowMenu(bool show) {}

bool ColorButton::showMenu() const { return false; }

void ColorButton::setBehavior(Behavior behavior) {}

ColorButton::Behavior ColorButton::behavior() const { return Behavior(); }

void ColorButton::setDefaultColor(const QColor &color) {}

QColor ColorButton::defaultColor() const { return QColor(); }

void ColorButton::setShowNoColor(const bool show) {}

bool ColorButton::showNoColor() const { return false; }

void ColorButton::setNoColorString(const QString &str) {}

void ColorButton::setShowNull(bool show, const QString &nullString) {}

bool ColorButton::showNull() const { return false; }

bool ColorButton::isNull() const { return false; }

QString ColorButton::noColorString() const { return QString(); }

void ColorButton::setContext(const QString &str) {}

QString ColorButton::context() const { return QString(); }

void ColorButton::setColor(const QColor &color) {}

void ColorButton::setButtonBackground(const QColor &color) {}

void ColorButton::copyColor() {}

void ColorButton::pasteColor() {}

void ColorButton::activatePicker() {}

void ColorButton::setToNoColor() {}

void ColorButton::setToDefaultColor() {}

void ColorButton::setToNull() {}

void ColorButton::unlink() {}

bool ColorButton::event(QEvent *event) { return false; }

void ColorButton::changeEvent(QEvent *event) {}

void ColorButton::showEvent(QShowEvent *event) {}

void ColorButton::resizeEvent(QResizeEvent *event) {}

const QPixmap &ColorButton::transparentBackground()
{
    static QPixmap pixmap;
    return pixmap;
}

void ColorButton::mouseReleaseEvent(QMouseEvent *event) {}

void ColorButton::mousePressEvent(QMouseEvent *event) {}

void ColorButton::mouseMoveEvent(QMouseEvent *event) {}

void ColorButton::keyPressEvent(QKeyEvent *event) {}

void ColorButton::dragEnterEvent(QDragEnterEvent *event) {}

void ColorButton::dragLeaveEvent(QDragLeaveEvent *event) {}

void ColorButton::dropEvent(QDropEvent *event) {}

void ColorButton::wheelEvent(QWheelEvent *event) {}

/* --------------------------- ColorButtonPrivate --------------------------- */

ColorButtonPrivate::ColorButtonPrivate(ColorButton *ptr) {}

ColorButtonPrivate::~ColorButtonPrivate() {}

bool ColorButtonPrivate::colorFromMimeData(const QMimeData *mimeData, QColor &resultColor)
{
    return false;
}

void ColorButtonPrivate::stopPicking(QPoint eventPos, bool samplingColor) {}

QColor ColorButtonPrivate::linkedProjectColor() const { return QColor(); }

void ColorButtonPrivate::buttonClicked() {}

void ColorButtonPrivate::showColorDialog() {}

void ColorButtonPrivate::setValidTemporaryColor(const QColor &color) {}

void ColorButtonPrivate::addRecentColor(const QColor &color) {}

void ColorButtonPrivate::prepareMenu() {}

#include "colorbutton.moc"