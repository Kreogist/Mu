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
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>

#include "kndpimanager.h"

#include "knmusicgeneraldelegate.h"

#include <QDebug>

#define RowHeight   20
#define IconSize    14

KNMusicGeneralDelegate::KNMusicGeneralDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void KNMusicGeneralDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    //Check the decoration.
    QVariant itemIcon=index.data(Qt::DecorationRole);
    if(itemIcon.isValid())
    {
        //Check the selected state.
        if(option.state & QStyle::State_Selected)
        {
            //Draw the selection background.
            painter->fillRect(option.rect,
                              option.palette.brush(QPalette::Highlight));
        }
        //Calculate the size.
        QSize iconSize=knDpi->size(IconSize, IconSize);
        //Draw the item icon.
        QPixmap scaledItemIcon=itemIcon.value<QIcon>().pixmap(iconSize);
        painter->drawPixmap(QPoint(option.rect.x(), option.rect.y()) +
                            knDpi->pos(3, 3),
                            scaledItemIcon.scaled(iconSize,
                                                  Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation));
        return;
    }
    else
    {
        //Do orignal paint mission.
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize KNMusicGeneralDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    //Simply modified the height to what we want.
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 knDpi->height(RowHeight));
}
