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

#include "knnotificationutil.h"

#include "knnotificationdelegate.h"

#define ImageSize 35
#define Spacing 5
#define ContentY 22
#define TitleY 7
#define TextLeft (ImageSize + (Spacing<<1))

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
    //Reset painter font.
    painter->setFont(option.font);
    painter->setOpacity(1.0);
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
    //Calculate the text width.
    int textWidth=option.rect.width()-TextLeft;
    //Draw the pixmap.
    painter->drawPixmap(Spacing,
                        option.rect.top() + Spacing,
                        index.data(Qt::DecorationRole).value<QPixmap>());
    painter->setOpacity(0.5);
    //Draw content.
    painter->drawText(QRect(TextLeft,
                            option.rect.top()+ContentY,
                            textWidth,
                            option.fontMetrics.height()<<1),
                      option.fontMetrics.elidedText(
                          index.data(
                              KNNotificationUtil::ContentRole).toString(),
                          Qt::ElideRight,
                          textWidth<<1));
    //Draw the border line except the last one.
    if(index.model()->rowCount()!=index.row()+1)
    {
        painter->drawLine(QPoint(TextLeft, option.rect.bottom()),
                          QPoint(option.rect.right()-1, option.rect.bottom()));
    }
    //Set the font bold.
    painter->setOpacity(1.0);
    QFont boldFont=option.font;
    boldFont.setBold(true);
    painter->setFont(boldFont);
    //Draw title.
    painter->drawText(QRect(TextLeft,
                            option.rect.top()+TitleY,
                            textWidth,
                            option.fontMetrics.height()),
                      option.fontMetrics.elidedText(
                          index.data(Qt::DisplayRole).toString(),
                          Qt::ElideRight,
                          textWidth));
}

