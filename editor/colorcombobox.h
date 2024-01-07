#ifndef COLORCOMBOBOX_H
#define COLORCOMBOBOX_H

#include <QToolButton>
#include <coloreditor_global.h>

class COLOR_EDITOR_EXPORT ColorComboBox : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorComboBox(QWidget *parent = nullptr, QColor color = Qt::black,
                         QSize size = QSize(50, 25));
    ~ColorComboBox();
    void setButtonColor(QColor color);
    inline QColor currentColor() { return m_currentColor; }
    void setShowInvalidColor(bool bShow);
    bool showInvalidColor();

signals:
    void selectColorChanged(QColor color);

private slots:
    void onColorChanged();
    void onShowColorBoard();

private:
    struct ColorInfo
    {
        QString name;
        QColor color;
    };
    static ColorInfo colorInfos[10][12];
    QColor m_currentColor;
    bool m_bShowInvalidColor = true;
    QToolButton *m_pBtnOtherColor = NULL;

    QMenu *createColorMenu(const char *slot, const char *slotColorBoard);
    QIcon createColorToolButtonIcon(QColor color);
    QIcon createColorIcon(QColor color);
};

#endif//COLORCOMBOBOX_H