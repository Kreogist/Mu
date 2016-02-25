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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>

#include "knnotificationdelegate.h"

KNNotificationDelegate::KNNotificationDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

QSize KNNotificationDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 NotificationItemHeight);
}

void KNNotificationDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    //Set antialiasing.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Set the default text color.
    QColor textColor=option.palette.color(QPalette::Text);
    //Check selection state.
    //Draw the background part of the widget.
    if(option.state & QStyle::State_Selected)
    {
        //Draw the high light background.
        painter->fillRect(option.rect,
                          option.palette.highlight());
        //Update the text color to highlighted color.
        textColor=option.palette.color(QPalette::HighlightedText);
    }
    //Set the text color.
    painter->setPen(textColor);
    //Set the font bold.
    QFont boldFont=option.font;
    boldFont.setBold(true);
    painter->setFont(boldFont);
    //Draw title.
    painter->drawText(option.rect, index.data(Qt::DisplayRole).toString());
}

