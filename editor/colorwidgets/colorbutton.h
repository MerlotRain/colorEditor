#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>
#include <coloreditor_global.h>

class ColorButtonPrivate;
class COLOR_EDITOR_EXPORT ColorButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QString colorDialogTitle READ colorDialogTitle WRITE
                       setColorDialogTitle)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(bool allowOpacity READ allowOpacity WRITE setAllowOpacity)
    Q_PROPERTY(Behavior behavior READ behavior WRITE setBehavior)
    Q_PROPERTY(QColor defaultColor READ defaultColor WRITE setDefaultColor)
    Q_PROPERTY(bool showNoColor READ showNoColor WRITE setShowNoColor)
    Q_PROPERTY(QString noColorString READ noColorString WRITE setNoColorString)
    Q_PROPERTY(QString context READ context WRITE setContext)

public:
    enum Behavior
    {
        ShowDialog = 0,
        SignalOnly
    };
    Q_ENUM(Behavior)

    explicit ColorButton(QWidget *parent = nullptr,
                         const QString &cdt = QString());
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    QColor color() const;
    void setAllowOpacity(bool allow);
    bool allowOpacity() const;

    void setColorDialogTitle(const QString &title);
    QString colorDialogTitle() const;

    void setShowMenu(bool show);
    bool showMenu() const;

    void setBehavior(Behavior behavior);
    Behavior behavior() const;

    void setDefaultColor(const QColor &color);
    QColor defaultColor() const;
    void setShowNoColor(const bool show);
    bool showNoColor() const;
    void setNoColorString(const QString &str);
    void setShowNull(bool show, const QString &nullString = QString());
    bool showNull() const;
    bool isNull() const;
    QString noColorString() const;
    void setContext(const QString &str);
    QString context() const;

public:
    void setColor(const QColor &color);
    void setButtonBackground(const QColor &color = QColor());
    void copyColor();
    void pasteColor();
    void activatePicker();
    void setToNoColor();
    void setToDefaultColor();
    void setToNull();
    void unlink();

signals:
    void colorChanged(const QColor &color);
    void colorClicked(const QColor &color);
    void cleared();
    void unlinked();

protected:
    bool event(QEvent *event) override;
    void changeEvent(QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    static const QPixmap &transparentBackground();

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    ColorButtonPrivate *d;
    friend class ColorButtonPrivate;
};

#endif//COLORBUTTON_H