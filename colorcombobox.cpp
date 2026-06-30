/* ======================================================================
    Copyright (C) 2026 Gerhard Kokerbeck ffDiaporama@SBK2day.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
   ====================================================================== */

#include "colorcombobox.h"
#include "coloritemdelegate.h"
#include <QListView>
#include <QStandardItemModel>
#include <QStylePainter>
#include <QColorDialog>

// extern const int MAXCOLORREF;
// extern const int NBCOLORCOL;
// extern const int NBCOLORROW;
// extern const int ITEMSIZEW ;
// extern const int ITEMSIZEH ;
//extern QString  ColorRef[MAXCOLORREF];

QStringList ColorComboBox::predefinedColors;

ColorComboBox::ColorComboBox(QWidget *parent) : QComboBox(parent)
{
    if(predefinedColors.size() == 0)
    {
        predefinedColors
            << QStringLiteral("#fff2f2f2")<< QStringLiteral("#ffd8d8d8")<< QStringLiteral("#ffbfbfbf")<< QStringLiteral("#ffa5a5a5")<< QStringLiteral("#ff7f7f7f")      // LIGHT-GRAY
            << QStringLiteral("#ff595959")<< QStringLiteral("#ff3f3f3f")<< QStringLiteral("#ff262626")<< QStringLiteral("#ff0c0c0c")<< QStringLiteral("#ff000000")      // DARK-GRAY
            << QStringLiteral("#ffdae1eb")<< QStringLiteral("#ffb5c4d7")<< QStringLiteral("#ff91a7c3")<< QStringLiteral("#ff3c526f")<< QStringLiteral("#ff28374a")      // BLUE-GRAY
            << QStringLiteral("#ffc8eefc")<< QStringLiteral("#ff91defa")<< QStringLiteral("#ff5acef8")<< QStringLiteral("#ff0578a2")<< QStringLiteral("#ff03506c")      // BLUE-1
            << QStringLiteral("#ffe0e6f5")<< QStringLiteral("#ffc1ceeb")<< QStringLiteral("#ffa2b5e2")<< QStringLiteral("#ff365bb0")<< QStringLiteral("#ff243c75")      // BLUE-2
            << QStringLiteral("#ffe8eeee")<< QStringLiteral("#ffd1dede")<< QStringLiteral("#ffb9cdce")<< QStringLiteral("#ff61888a")<< QStringLiteral("#ff405b5c")      // BLUE-3
            << QStringLiteral("#ffe5ecd8")<< QStringLiteral("#ffcbd9b2")<< QStringLiteral("#ffb2c78c")<< QStringLiteral("#ff5c7237")<< QStringLiteral("#ff3d4c25")      // GREEN-1
            << QStringLiteral("#ffe8efe8")<< QStringLiteral("#ffd2dfd1")<< QStringLiteral("#ffbbcfba")<< QStringLiteral("#ff648c60")<< QStringLiteral("#ff425d40")      // GREEN-2
            << QStringLiteral("#ffe1dca5")<< QStringLiteral("#ffd0c974")<< QStringLiteral("#ffa29a36")<< QStringLiteral("#ff514d1b")<< QStringLiteral("#ff201e0a")      // GREEN-3
            << QStringLiteral("#fff5f2d8")<< QStringLiteral("#ffece5b2")<< QStringLiteral("#ffe2d88c")<< QStringLiteral("#ffa39428")<< QStringLiteral("#ff6d621a")      // GREEN-4
            << QStringLiteral("#fff2eee8")<< QStringLiteral("#ffe6ded1")<< QStringLiteral("#ffdacdba")<< QStringLiteral("#ffa38557")<< QStringLiteral("#ff6d593a")      // MARROON-1
            << QStringLiteral("#fff6e6d5")<< QStringLiteral("#ffeeceaa")<< QStringLiteral("#ffe6b681")<< QStringLiteral("#ffa2641f")<< QStringLiteral("#ff6c4315")      // MARROON-2
            << QStringLiteral("#fff2e0c6")<< QStringLiteral("#ffe6c28d")<< QStringLiteral("#ffdaa454")<< QStringLiteral("#ff664515")<< QStringLiteral("#ff442e0e")      // MARROON-3
            << QStringLiteral("#fffff7c1")<< QStringLiteral("#fffff084")<< QStringLiteral("#ffffe947")<< QStringLiteral("#ff998700")<< QStringLiteral("#ff665a00")      // YELLOW-1
            << QStringLiteral("#fffde1d1")<< QStringLiteral("#fffcc3a3")<< QStringLiteral("#fffba576")<< QStringLiteral("#ffc94b05")<< QStringLiteral("#ff863203")      // ORANGE
            << QStringLiteral("#fffbc7bc")<< QStringLiteral("#fff78f7a")<< QStringLiteral("#fff35838")<< QStringLiteral("#ff711806")<< QStringLiteral("#ff4b1004")      // RED-1
            << QStringLiteral("#ffe5e1f4")<< QStringLiteral("#ffcbc3e9")<< QStringLiteral("#ffb1a6de")<< QStringLiteral("#ff533da9")<< QStringLiteral("#ff372970")      // VIOLET-1
            << QStringLiteral("#ffece4f1")<< QStringLiteral("#ffdac9e3")<< QStringLiteral("#ffc7aed6")<< QStringLiteral("#ff7d4d99")<< QStringLiteral("#ff533366")      // VIOLET-2
            << QStringLiteral("#ff000000")<< QStringLiteral("#ffff0000")<< QStringLiteral("#ff00ff00")<< QStringLiteral("#ff0000ff")<< QStringLiteral("#ffffffff")      // Full-colors
            << QStringLiteral("#ffffff00")<< QStringLiteral("#ffff00ff")<< QStringLiteral("#ff00ffff")<< QStringLiteral("#ff3a3a3a");
    }
    m_view = new QListView(this);

    m_view->setViewMode(QListView::IconMode);
    m_view->setFlow(QListView::LeftToRight);
    m_view->setWrapping(true);
    m_view->setResizeMode(QListView::Adjust);
    m_view->setUniformItemSizes(true);
    m_view->setMouseTracking(true);

    constexpr int columns = NBCOLORCOL;
    constexpr int rows = NBCOLORROW;

    m_view->setGridSize(QSize(ITEMSIZEW, ITEMSIZEH));
    m_view->setFixedSize( columns * ITEMSIZEW + 22, rows * ITEMSIZEH);

    setView(m_view);

    m_model = new QStandardItemModel(this);
    setModel(m_model);

    auto *delegate = new ColorItemDelegate(this);
    m_view->setItemDelegate(delegate);

    populateColors();
    delegate->mOwnerDefinedColorIndex = m_model->rowCount()-1;

    setMaxVisibleItems(10);
    //connect(m_view,&QListView::clicked,this,&ColorComboBox::item_clicked);
    connect(m_view,&QListView::pressed,this,&ColorComboBox::item_clicked);
    //connect(m_view,&QListView::activated,this,&ColorComboBox::item_clicked);
}

void ColorComboBox::populateColors()
{
    for (const QString &cs : std::as_const(predefinedColors))
    {
        auto *item = new QStandardItem;
        QColor c(cs);
        item->setData(c, Qt::UserRole);
        item->setEditable(false);

        m_model->appendRow(item);
    }
    auto *item = new QStandardItem;
    mSavedCustomColor = QColor(Qt::black).name(QColor::HexArgb);
    QColor c(mSavedCustomColor);
    item->setData(c, Qt::UserRole);
    item->setEditable(false);
    m_model->appendRow(item);
}

QString ColorComboBox::currentColor() const
{
    QModelIndex idx = model()->index(currentIndex(), 0);
    return idx.data(Qt::UserRole).value<QColor>().name(QColor::HexArgb);
}

/*--------------------------------------------------------------------------------
void ColorComboBox::setCurrentColor(const QColor &color)
{
    const QString colorName = color.name(QColor::HexArgb);
    int index = predefinedColors.indexOf(colorName);
    if(index < 0)
        index = predefinedColors.size();
    setCurrentIndex(index);
    update();
    return;
}
----------------------------------------------------------------------------------*/

void ColorComboBox::setCurrentColor(const QString &colorName)
{
    QString cName = colorName;
    if(cName.length() < 9 )
        cName.replace("#","#ff");
    int index = predefinedColors.indexOf(cName);
    if (index < 0)
    {
        index = predefinedColors.size();
        this->setItemData(index, QColor(colorName), Qt::UserRole);
    }
    setCurrentIndex(index);
    update();
    return;
}

void ColorComboBox::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);

    QStyleOptionComboBox opt;
    initStyleOption(&opt);

    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    QRect fieldRect =  style()->subControlRect( QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);
    QColor color = currentColor();
    QRect colorRect = fieldRect.adjusted(4, 4, -4, -4);

    painter.setPen(Qt::black);
    painter.setBrush(color);
    painter.fillRect(colorRect,color);

    if(this->currentIndex() >= MAXCOLORREF)
    {
        QImage *img2 = &((ColorItemDelegate*)this->itemDelegate())->mColorOverlayImage;
        painter.drawImage(QRectF(colorRect.x() + (colorRect.width() - 16) / 2, colorRect.y() + (colorRect.height() - 16) / 2, 16, 16), *img2);
    }
}

void ColorComboBox::item_clicked(const QModelIndex &index)
{
    if (index.row() >= MAXCOLORREF)
    {
        // Open box to select custom color
        QColor c(this->mSavedCustomColor);
        QColor color = QColorDialog::getColor(c, 0, QString(), QColorDialog::ShowAlphaChannel);
        if (color.isValid())
        {
            this->mSavedCustomColor = color.name(QColor::HexArgb);
            this->m_model->item(index.row())->setData(color, Qt::UserRole);
            setCurrentIndex(index.row());
            emit currentIndexChanged(index.row());
        }
    }
    else
    {
        setCurrentIndex(index.row());
        emit currentIndexChanged(index.row());
    }
}
