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

#include "knmusiclyricsitemdelegate.h"

#include <QDebug>

KNMusicLyricsItemDelegate::KNMusicLyricsItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    ;
}

KNMusicLyricsItemDelegate::~KNMusicLyricsItemDelegate()
{

}

QSize KNMusicLyricsItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    return QSize(QStyledItemDelegate::sizeHint(option, index).width(),
                 50);
}

void KNMusicLyricsItemDelegate::paint(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    //Set the painter.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing, true);
    QColor textColor=option.palette.color(QPalette::WindowText);
    if(option.state & QStyle::State_Selected)
    {
        //Draw the background.
        painter->fillRect(option.rect, option.palette.highlight());
        //Set the text color.
//        textColor=option.palette.color(QPalette::HighlightedText);
    }
    //Draw the text.
    int textTop=(option.rect.height()-(option.fontMetrics.height()<<1))>>1;
    painter->setPen(textColor);
    painter->drawText(option.rect.x(),
                      option.rect.top()+textTop,
                      option.rect.width(),
                      option.fontMetrics.height(),
                      Qt::AlignLeft,
                      index.data(Qt::DisplayRole).toString());
    textColor.setAlpha(100);
    painter->setPen(textColor);
    painter->drawText(option.rect.x(),
                      option.rect.top()+textTop+option.fontMetrics.height(),
                      option.rect.width(),
                      option.rect.height(),
                      Qt::AlignLeft,
                      index.data(LyricsArtistRole).toString());
}

