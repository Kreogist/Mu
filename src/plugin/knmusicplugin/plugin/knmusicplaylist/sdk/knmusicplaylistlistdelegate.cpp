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

#include "knmusicplaylistlistdelegate.h"

#define ItemHeight 26
#define IconSize 24
#define Spacing 3

KNMusicPlaylistListDelegate::KNMusicPlaylistListDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

void KNMusicPlaylistListDelegate::paint(QPainter *painter,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    //Set antialiasing.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Set the default text color.
    QColor textColor=option.palette.color(QPalette::Text);
    //Draw the background part of the widget.
    if(option.state & QStyle::State_Selected)
    {
        //Draw the high light background.
        painter->fillRect(option.rect,
                          option.palette.highlight());
        //Update the text color to highlighted color.
        textColor=option.palette.color(QPalette::HighlightedText);
    }
    //Draw the icon.
    index.data(Qt::DecorationRole).value<QIcon>().paint(
                painter,
                option.rect.x()+Spacing,
                option.rect.y(),
                IconSize,
                IconSize);
    //Draw the text.
    painter->setPen(textColor);
    painter->setFont(option.font);
    painter->drawText(QRect(option.rect.x()+IconSize+(Spacing<<1),
                            option.rect.y(),
                            option.rect.width()-IconSize-(Spacing<<1),
                            option.rect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      index.data(Qt::DisplayRole).toString());
}

QSize KNMusicPlaylistListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    //Set the item size as the fixed item height.
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 ItemHeight);
}
