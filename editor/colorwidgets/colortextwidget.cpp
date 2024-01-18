#include "colortextwidget.h"
#include "colorutility.h"
#include "colorwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QStyle>
#include <QToolButton>
#include <QtWidgets/qboxlayout.h>

class ColorTextWidgetPrivate : public QObject
{
    Q_OBJECT
public:
    ColorTextWidgetPrivate(ColorTextWidget *ptr);

    ColorTextWidget *q;
    QLineEdit *lineEdit = nullptr;
    QToolButton *menuButton = nullptr;
    ColorTextWidget::ColorTextFormat format = ColorTextWidget::HexRgb;
    bool allowAlpha = true;

    void updateText();
    void textChanged();
    void showMenu();
};

/* ----------------------------- ColorTextWidget ---------------------------- */

ColorTextWidget::ColorTextWidget(QWidget *parent)
    : ColorWidget(parent), d(new ColorTextWidgetPrivate(this))
{
}

ColorTextWidget::~ColorTextWidget()
{
    delete d;
    d = nullptr;
}

void ColorTextWidget::setColor(const QColor &color, bool emitSignals)
{
    ColorWidget::setColor(color, emitSignals);
    d->updateText();
}

void ColorTextWidget::setAllowOpacity(bool allow) { d->allowAlpha = allow; }

void ColorTextWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    const QSize sz = d->menuButton->sizeHint();
    const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    d->menuButton->move(d->lineEdit->rect().right() - frameWidth - sz.width(),
                        (d->lineEdit->rect().bottom() + 1 - sz.height() / 2));
}

/* ------------------------- ColorTextWidgetPrivate ------------------------- */

ColorTextWidgetPrivate::ColorTextWidgetPrivate(ColorTextWidget *ptr) : q(ptr)
{
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    lineEdit = new QLineEdit(nullptr);
    hLayout->addWidget(lineEdit);

    menuButton = new QToolButton(lineEdit);
    menuButton->setIcon(QIcon(":/resources/mIconDropDownMenu.svg"));
    menuButton->setCursor(Qt::ArrowCursor);
    menuButton->setFocusPolicy(Qt::NoFocus);
    menuButton->setStyleSheet(QStringLiteral("QToolButton {border:none; padding:0px;}"));
    q->setLayout(hLayout);

    const int frameWidth = lineEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    lineEdit->setStyleSheet(
            QStringLiteral("QLineEdit{padding-right: %1px; }")
                    .arg(menuButton->sizeHint().width() + frameWidth + 1));

    connect(lineEdit, &QLineEdit::editingFinished, this,
            &ColorTextWidgetPrivate::textChanged);
    connect(menuButton, &QAbstractButton::clicked, this,
            &ColorTextWidgetPrivate::showMenu);

    format = ColorTextWidget::HexRgb;
    updateText();
}

void ColorTextWidgetPrivate::updateText()
{
    switch (format)
    {
        case ColorTextWidget::HexRgb:
            lineEdit->setText(q->mCurrentColor.name());
            break;
        case ColorTextWidget::HexRgba:
            lineEdit->setText(q->mCurrentColor.name() +
                              QStringLiteral("%1").arg(q->mCurrentColor.alpha(), 2, 16,
                                                       QChar('0')));
            break;
        case ColorTextWidget::Rgb:
            lineEdit->setText(tr("rgb(%1, %2, %3)")
                                      .arg(q->mCurrentColor.red())
                                      .arg(q->mCurrentColor.green())
                                      .arg(q->mCurrentColor.blue()));
            break;
        case ColorTextWidget::Rgba:
            lineEdit->setText(
                    tr("rgba(%1, %2, %3, %4)")
                            .arg(q->mCurrentColor.red())
                            .arg(q->mCurrentColor.green())
                            .arg(q->mCurrentColor.blue())
                            .arg(QString::number(q->mCurrentColor.alphaF(), 'f', 2)));
            break;
        default:
            break;
    }
}

void ColorTextWidgetPrivate::textChanged()
{
    const QString testString = lineEdit->text();
    bool containsAlpha;
    QColor color = ColorUtility::parseColorWithAlpha(testString, containsAlpha);
    if (!color.isValid())
    {
        updateText();
        return;
    }

    if (color != q->mCurrentColor)
    {
        if (!containsAlpha)
        {
            color.setAlpha(q->mCurrentColor.alpha());
        }
        q->mCurrentColor = color;
        emit q->colorChanged(color);
    }
}

void ColorTextWidgetPrivate::showMenu()
{

    QMenu colorContextMenu;
    QAction *hexRgbaAction = nullptr;
    QAction *rgbaAction = nullptr;

    QAction *hexRgbAction = new QAction(tr("#RRGGBB"), nullptr);
    colorContextMenu.addAction(hexRgbAction);
    if (allowAlpha)
    {
        hexRgbaAction = new QAction(tr("#RRGGBBAA"), nullptr);
        colorContextMenu.addAction(hexRgbaAction);
    }
    QAction *rgbAction = new QAction(tr("rgb( r, g, b )"), nullptr);
    colorContextMenu.addAction(rgbAction);
    if (allowAlpha)
    {
        rgbaAction = new QAction(tr("rgba( r, g, b, a )"), nullptr);
        colorContextMenu.addAction(rgbaAction);
    }

    QAction *selectedAction = colorContextMenu.exec(QCursor::pos());
    if (selectedAction == hexRgbAction)
    {
        format = ColorTextWidget::HexRgb;
    }
    else if (hexRgbaAction && selectedAction == hexRgbaAction)
    {
        format = ColorTextWidget::HexRgba;
    }
    else if (selectedAction == rgbAction)
    {
        format = ColorTextWidget::Rgb;
    }
    else if (rgbaAction && selectedAction == rgbaAction)
    {
        format = ColorTextWidget::Rgba;
    }

    updateText();
}

#include "colortextwidget.moc"