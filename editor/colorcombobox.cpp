#include "colorcombobox.h"
#include <QAction>
#include <QColor>
#include <QColorDialog>
#include <QGridLayout>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QSize>
#include <QSizePolicy>
#include <QVariant>

// clang-format off
// http://downloads.esri.com/support/documentation/ao_/ArcScene_Basic.pdf
ColorComboBox::ColorInfo ColorComboBox::colorInfos[10][12] = {
{
            {QStringLiteral("Arctic White"),        QColor(255, 255, 255)},
            {QStringLiteral("Rose Quartz"),         QColor(255, 190, 190)},
            {QStringLiteral("Sahara Sand"),         QColor(255, 235, 190)},
            {QStringLiteral("Topaz Sand"),          QColor(255, 235, 175)},
            {QStringLiteral("Yucca Yellow"),        QColor(255, 255, 190)},
            {QStringLiteral("Olivine Yellow"),      QColor(233, 255, 190)},
            {QStringLiteral("Tzavorite Green"),     QColor(211, 255, 190)},
            {QStringLiteral("Indicolite Green"),    QColor(190, 255, 232)},
            {QStringLiteral("Sodalite Blue"),       QColor(190, 232, 255)},
            {QStringLiteral("Sugilite Sky"),        QColor(190, 210, 255)},
            {QStringLiteral("Lepidolite Lilac"),    QColor(232, 190, 255)},
            {QStringLiteral("Rhodolite Rose"),      QColor(255, 190, 232)}
        },
        {
            {QStringLiteral("Gray 10%"),            QColor(225, 225, 225)},
            {QStringLiteral("Medium Coral Light"),  QColor(255, 127, 127)},
            {QStringLiteral("Cantaloupe"),          QColor(255, 167, 127)},
            {QStringLiteral("Mango"),               QColor(255, 211, 127)},
            {QStringLiteral("Autunite Yellow"),     QColor(255, 255, 115)},
            {QStringLiteral("Lemongrass"),          QColor(209, 255, 115)},
            {QStringLiteral("Light Apple"),         QColor(163, 255, 115)},
            {QStringLiteral("Beryl Green"),         QColor(115, 255, 223)},
            {QStringLiteral("Apatite Blue"),        QColor(115, 223, 255)},
            {QStringLiteral("Yogo Blue"),           QColor(115, 178, 255)},
            {QStringLiteral("Heliotrope"),          QColor(223, 115, 255)},
            {QStringLiteral("Fushia Pink"),         QColor(255, 115, 223)}
        },
        {
            {QStringLiteral("Gray 20%"),            QColor(204, 204, 204)},
            {QStringLiteral("Mars Red"),            QColor(255,   0,   0)},
            {QStringLiteral("Fire Red"),            QColor(255,  85,   0)},
            {QStringLiteral("Electron Gold"),       QColor(255, 170,   0)},
            {QStringLiteral("Solar Yellow"),        QColor(255, 255,   0)},
            {QStringLiteral("Peridot Green"),       QColor(170, 255,   0)},
            {QStringLiteral("Medium Apple"),        QColor( 85, 255,   0)},
            {QStringLiteral("Tourmaline Green"),    QColor(  0, 255, 197)},
            {QStringLiteral("Big Sky Blue"),        QColor(  0, 197, 255)},
            {QStringLiteral("Cretean Blue"),        QColor(  0, 130, 255)},
            {QStringLiteral("Amethyst"),            QColor(197,   0, 255)},
            {QStringLiteral("Ginger Pink"),         QColor(255,   0, 197)}
        },
        {
            {QStringLiteral("Gray 30%"),            QColor(178, 178, 178)},
            {QStringLiteral("Poinsettia Red"),      QColor(230,   0,   0)},
            {QStringLiteral("Flame Red"),           QColor(230,  76,   0)},
            {QStringLiteral("Seville Orange"),      QColor(230, 152,   0)},
            {QStringLiteral("Citroen Yellow"),      QColor(230, 230,   0)},
            {QStringLiteral("Macaw Green"),         QColor(152, 230,   0)},
            {QStringLiteral("Quetzel Green"),       QColor( 76, 230,   0)},
            {QStringLiteral("Chrysophase"),         QColor(  0, 230, 169)},
            {QStringLiteral("Moorea Blue"),         QColor(  0, 169, 230)},
            {QStringLiteral("Lapis Lazuli"),        QColor(  0,  92, 230)},
            {QStringLiteral("Anemone Violet"),      QColor(169,   0, 230)},
            {QStringLiteral("Peony Pink"),          QColor(230,   0, 169)}
        },
        {
            {QStringLiteral("Gray 40%"),            QColor(156, 156, 156)},
            {QStringLiteral("Tuscan Red"),          QColor(168,   0,   0)},
            {QStringLiteral("Cherry Cola"),         QColor(168,  56,   0)},
            {QStringLiteral("Raw Umber"),           QColor(168, 112,   0)},
            {QStringLiteral("Olivenite Green"),     QColor(168, 168,   0)},
            {QStringLiteral("Tarragon Green"),      QColor(112, 168,   0)},
            {QStringLiteral("Leaf Green"),          QColor( 56, 168,   0)},
            {QStringLiteral("Malachite Green"),     QColor(  0, 168, 132)},
            {QStringLiteral("Delft Blue"),          QColor(  0, 132, 168)},
            {QStringLiteral("Ultra Blue"),          QColor(  0,  77, 168)},
            {QStringLiteral("Dark Amethyst"),       QColor(132,   0, 168)},
            {QStringLiteral("Cattleya Orchid"),     QColor(168,   0, 132)}
        },
        {
            {QStringLiteral("Gray 50%"),            QColor(130, 130, 130)},
            {QStringLiteral("Dark Umber"),          QColor(115,   0,   0)},
            {QStringLiteral("Cherrywood Brown"),    QColor(115,  38,   0)},
            {QStringLiteral("Burnt Umber"),         QColor(115,  76,   0)},
            {QStringLiteral("Dark Olivenite"),      QColor(115, 115,   0)},
            {QStringLiteral("Spruce Green"),        QColor( 76, 115,   0)},
            {QStringLiteral("Fir Green"),           QColor( 38, 115,   0)},
            {QStringLiteral("Peacock Green"),       QColor(  0, 115,  76)},
            {QStringLiteral("Steel Blue"),          QColor(  0,  76, 115)},
            {QStringLiteral("Dark Navy"),           QColor(  0,  38, 115)},
            {QStringLiteral("Ultramarine"),         QColor( 76,   0, 115)},
            {QStringLiteral("Purple Heart"),        QColor(115,   0,  76)}
        },
        {
            {QStringLiteral("Gray 60%"),            QColor(104, 104, 104)},
            {QStringLiteral("Rose Dust"),           QColor(215, 158, 158)},
            {QStringLiteral("Soapstone Dust"),      QColor(215, 176, 158)},
            {QStringLiteral("Tecate Dust"),         QColor(215, 194, 158)},
            {QStringLiteral("Lime Dust"),           QColor(215, 215, 158)},
            {QStringLiteral("Apple Dust"),          QColor(199, 215, 158)},
            {QStringLiteral("Sage Dust"),           QColor(180, 215, 158)},
            {QStringLiteral("Turquoise Dust"),      QColor(158, 215, 194)},
            {QStringLiteral("Blue Gray Dust"),      QColor(158, 187, 215)},
            {QStringLiteral("Violet Dust"),         QColor(158, 170, 215)},
            {QStringLiteral("Lilac Dust"),          QColor(194, 158, 215)},
            {QStringLiteral("Tudor Rose Dust"),     QColor(214, 157, 188)}
        },
        {
            {QStringLiteral("Gray 70%"),            QColor( 78,  78,  78)},
            {QStringLiteral("Medium Coral"),        QColor(245, 122, 122)},
            {QStringLiteral("Orange Dust"),         QColor(245, 162, 122)},
            {QStringLiteral("Medium Sand"),         QColor(245, 202, 122)},
            {QStringLiteral("Medium Yellow"),       QColor(245, 245, 122)},
            {QStringLiteral("Medium Lime"),         QColor(205, 245, 122)},
            {QStringLiteral("Medium Key Lime"),     QColor(165, 245, 122)},
            {QStringLiteral("Light Vert"),          QColor(122, 245, 202)},
            {QStringLiteral("Oxide Blue"),          QColor(122, 182, 245)},
            {QStringLiteral("Medium Azul"),         QColor(122, 142, 245)},
            {QStringLiteral("Medium Lilac"),        QColor(202, 122, 245)},
            {QStringLiteral("Medium Fushia"),       QColor(245, 122, 182)}
        },
        {
            {QStringLiteral("Gray 80%"),            QColor( 52,  52,  52)},
            {QStringLiteral("Tulip Pink"),          QColor(205, 102, 102)},
            {QStringLiteral("Nubuck Tan"),          QColor(205, 137, 102)},
            {QStringLiteral("Light Sienna"),        QColor(205, 170, 102)},
            {QStringLiteral("Light Olivenite"),     QColor(205, 205, 102)},
            {QStringLiteral("Medium Olivenite"),    QColor(171, 205, 102)},
            {QStringLiteral("Fern Green"),          QColor(137, 205, 102)},
            {QStringLiteral("Jadeite"),             QColor(102, 205, 171)},
            {QStringLiteral("Atlantic Blue"),       QColor(102, 153, 205)},
            {QStringLiteral("Pacific Blue"),        QColor(102, 119, 205)},
            {QStringLiteral("Aster Purple"),        QColor(170, 102, 205)},
            {QStringLiteral("Protea Pink"),         QColor(205, 102, 153)}
        },
        {
            {QStringLiteral("Black"),               QColor(  0,   0,   0)},
            {QStringLiteral("Cordovan Brown"),      QColor(137,  68,  68)},
            {QStringLiteral("Cocoa Brown"),         QColor(137,  90,  68)},
            {QStringLiteral("Leather Brown"),       QColor(137, 112,  68)},
            {QStringLiteral("Lichen Green"),        QColor(137, 137,  68)},
            {QStringLiteral("Moss Green"),          QColor(114, 137,  68)},
            {QStringLiteral("Lotus Pond Green"),    QColor( 92, 137,  68)},
            {QStringLiteral("Deep Forest"),         QColor( 68, 137, 112)},
            {QStringLiteral("Larkspur Blue"),       QColor( 68, 101, 137)},
            {QStringLiteral("Glacier Blue"),        QColor( 38,  79, 137)},
            {QStringLiteral("Blackberry"),          QColor(112,  68, 137)},
            {QStringLiteral("Cabernet"),            QColor(137,  68, 101)}
        }
};
// clang-format on

ColorComboBox::ColorComboBox(QWidget *parent, QColor color, QSize size)
    : QToolButton(parent), m_currentColor(color)
{
    setPopupMode(QToolButton::MenuButtonPopup);
    setMenu(createColorMenu(SLOT(onColorChanged()), SLOT(onShowColorBoard())));
    setFixedSize(size);
    setButtonColor(color);
}

ColorComboBox::~ColorComboBox() {}

void ColorComboBox::setButtonColor(QColor color)
{
    setIconSize(size());
    setIcon(createColorToolButtonIcon(color));
    if (m_currentColor != color)
    {
        m_currentColor = color;
        emit selectColorChanged(m_currentColor);
    }
}

void ColorComboBox::setShowInvalidColor(bool bShow) {}

bool ColorComboBox::showInvalidColor() { return false; }

QMenu *ColorComboBox::createColorMenu(const char *slot,
                                      const char *slotColorBoard)
{
    QAction *pActionTransparent = new QAction(this);
    pActionTransparent->setData(QColor(0, 0, 0, 0));
    pActionTransparent->setText(QObject::tr("No Color"));
    connect(pActionTransparent, SIGNAL(triggered()), this, slot);
    QToolButton *pBtnTransparent = new QToolButton();
    pBtnTransparent->setFixedHeight(20);
    pBtnTransparent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pBtnTransparent->setText(QObject::tr("No Color"));
    pBtnTransparent->setDefaultAction(pActionTransparent);

    QToolButton *pBtnOtherColor = new QToolButton();
    pBtnOtherColor->setText(QObject::tr("More Color"));
    pBtnOtherColor->setFixedHeight(20);
    pBtnOtherColor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pBtnOtherColor->setAutoRaise(true);
    pBtnOtherColor->setToolTip(QObject::tr("More Color"));
    connect(pBtnOtherColor, SIGNAL(clicked()), this, slotColorBoard);

    QGridLayout *pGridLayout = new QGridLayout(this);
    pGridLayout->setAlignment(Qt::AlignCenter);
    pGridLayout->setContentsMargins(0, 0, 0, 0);
    pGridLayout->setSpacing(2);

    for (int row = 0; row < 10; ++row)
    {
        QHBoxLayout *horizontalLayout = new QHBoxLayout();
        for (int column = 0; column < 12; ++column)
        {
            QAction *action = new QAction(this);
            action->setData(colorInfos[row][column].color);
            action->setIcon(createColorIcon(colorInfos[row][column].color));
            action->setToolTip(colorInfos[row][column].name);
            connect(action, SIGNAL(triggered()), this, slot);

            QToolButton *pBtnColor = new QToolButton;
            pBtnColor->setFixedSize(QSize(16, 16));
            pBtnColor->setAutoRaise(true);
            pBtnColor->setDefaultAction(action);

            pGridLayout->addWidget(pBtnColor, row, column);
        }
    }

    QWidget *pWidget = new QWidget();
    pWidget->setLayout(pGridLayout);

    QVBoxLayout *pVLayout = new QVBoxLayout();
    pVLayout->addWidget(pBtnTransparent);
    pVLayout->addStretch();
    pVLayout->addWidget(pWidget);
    pVLayout->addStretch();
    pVLayout->addWidget(pBtnOtherColor);

    QMenu *colorMenu = new QMenu(this);
    colorMenu->setLayout(pVLayout);
    colorMenu->addSeparator();

    return colorMenu;
}

QIcon ColorComboBox::createColorIcon(QColor color)
{
    QPixmap pixmap(16, 16);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 16, 16), color);
    return QIcon(pixmap);
}

QIcon ColorComboBox::createColorToolButtonIcon(QColor color)
{
    int nWidth = this->width();
    int nHeight = this->height();
    if (width() > 22 && height() > 8)
    {
        nWidth = this->width() - 22;
        nHeight = this->height() - 8;
    }
    QPixmap pixmap(nWidth, nHeight);

    if (color.alpha() == 0)
    {
        QPainter painter(&pixmap);
        QColor lineColor(161, 161, 161);
        QPen pen(lineColor, 1, Qt::SolidLine);
        QColor newColor(225, 225, 225);
        painter.setBrush(newColor);
        painter.setPen(pen);
        painter.drawRect(QRect(0, 0, nWidth - 1, nHeight - 1));

        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawLine(0, nHeight, nWidth, 0);
    }
    else
    {
        pixmap.fill(color);
    }

    QIcon icon(pixmap);
    return icon;
}

void ColorComboBox::onColorChanged()
{
    QAction *pFillColorAction = new QAction(this);
    pFillColorAction = qobject_cast<QAction *>(sender());
    QColor color = qvariant_cast<QColor>(pFillColorAction->data());
    setButtonColor(color);
}

void ColorComboBox::onShowColorBoard()
{
    this->menu()->close();
    QColor color = QColorDialog::getColor(m_currentColor, this, "",
                                          QColorDialog::ShowAlphaChannel);
    if (!color.isValid()) return;
    setButtonColor(color);
}