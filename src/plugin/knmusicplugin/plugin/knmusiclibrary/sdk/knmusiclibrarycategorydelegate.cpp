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

#include "knmusiclibrarycategorydelegate.h"

#define IconSize 40
#define Spacing 2
#define IconSizeWithSpacing 49

KNMusicLibraryCategoryDelegate::KNMusicLibraryCategoryDelegate(QWidget *parent):
    QStyledItemDelegate(parent)
{
}

void KNMusicLibraryCategoryDelegate::paint(QPainter *painter,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
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
    //Draw the decoration icon.
    QPixmap &&categoryIcon=
            (index.data(Qt::DecorationRole).value<QPixmap>()).scaled(
                IconSize,
                IconSize,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
    //Draw the pixmap data.
    painter->drawPixmap(option.rect.x()+Spacing+
                        ((IconSize-categoryIcon.width())>>1),
                        option.rect.y()+Spacing+
                        ((IconSize-categoryIcon.height())>>1),
                        categoryIcon.width(),
                        categoryIcon.height(),
                        categoryIcon);
    //Draw the text.
    painter->setPen(textColor);
    int textWidth=option.rect.width()-IconSizeWithSpacing;
    painter->drawText(QRect(option.rect.x()+IconSizeWithSpacing,
                            option.rect.y()+2,
                            textWidth,
                            IconSize),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      option.fontMetrics.elidedText(
                          index.data(Qt::DisplayRole).toString(),
                          Qt::ElideRight,
                          textWidth));
}
