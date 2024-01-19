#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <coloreditor_global.h>

class ColorDialogPrivate;
class COLOR_EDITOR_EXPORT ColorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ColorDialog(QWidget *parent = 0,
                         Qt::WindowFlags flags = Qt::WindowFlags(),
                         const QColor &color = QColor());
    ~ColorDialog();

    QColor color() const;
    void setTitle(const QString &title);
    void setAllowOpacity(bool allow);

    static QColor getColor(const QColor &initialColor, QWidget *parent,
                           const QString &title = QString(),
                           bool allowOpacity = false);

signals:
    void currentColorChanged(const QString &color);

public slots:
    void setColor(const QColor &color);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    ColorDialogPrivate *d;
    friend class ColorDialogPrivate;
};

#endif//COLORDIALOG_H