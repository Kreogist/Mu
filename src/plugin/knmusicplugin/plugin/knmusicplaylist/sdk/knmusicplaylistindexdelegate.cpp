/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>

#include "kndpimanager.h"

#include "knmusicplaylistindexdelegate.h"

#define RowHeight   20
#define IconSize    14

KNMusicPlaylistIndexDelegate::KNMusicPlaylistIndexDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

void KNMusicPlaylistIndexDelegate::paint(QPainter *painter,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    //Save the painter
    painter->save();
    //Enabled text antialiasing.
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    //Draw the background.
    if(option.state & QStyle::State_Selected)
    {
        //Draw the selection background.
        painter->fillRect(option.rect,
                          option.palette.brush(QPalette::Highlight));
        //Set the color to hightlighted text.
        painter->setPen(option.palette.color(QPalette::HighlightedText));
    }
    //Using the original painting to paint delegate.
    //Draw the text.
    painter->drawText(option.rect,
                      Qt::AlignRight | Qt::AlignVCenter,
                      QString::number(index.row()+1));
    //Check the icon.
    QVariant itemIcon=index.data(Qt::DecorationRole);
    if(itemIcon.isValid())
    {
        //Get the item icon size.
        QSize itemIconSize=knDpi->size(IconSize, IconSize);
        //Draw the icon.
        painter->drawPixmap(
                    option.rect.topLeft() +
                    QPoint((knDpi->width(RowHeight)-itemIconSize.width())>>1,
                           (knDpi->height(RowHeight)-itemIconSize.height())>>1),
                    itemIcon.value<QIcon>().pixmap(itemIconSize).scaled(
                        itemIconSize,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    }
    painter->restore();
}

QSize KNMusicPlaylistIndexDelegate::sizeHint(const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    return QSize(option.fontMetrics.width(QString::number(index.row()+1))+
                 knDpi->width(RowHeight),
                 knDpi->height(RowHeight));
}

