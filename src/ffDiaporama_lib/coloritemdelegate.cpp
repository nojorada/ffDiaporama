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
#include "coloritemdelegate.h"

ColorItemDelegate::ColorItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    mColorOverlayImage = QImage(":/img/colorize.png");
}

void ColorItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    painter->fillRect(option.rect,Qt::lightGray);
    QColor color = index.data(Qt::UserRole).value<QColor>();
    QRect colorRect = option.rect.adjusted(1, 1, -1, -1);

    if ((option.state & QStyle::State_Selected) || (option.state & QStyle::State_MouseOver))
    {
        painter->fillRect(option.rect, option.palette.highlight());
        colorRect = option.rect.adjusted(2, 2, -2, -2);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawRect(colorRect);

    if(index.row() == mOwnerDefinedColorIndex)
    {
        painter->drawImage(QRectF(option.rect.x() + (option.rect.width() - 16) / 2, option.rect.y() + (option.rect.height() - 16) / 2, 16, 16), mColorOverlayImage);
    }

    painter->restore();
}

QSize ColorItemDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(32, 24);
}
