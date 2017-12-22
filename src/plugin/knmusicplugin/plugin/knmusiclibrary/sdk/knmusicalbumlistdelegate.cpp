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

#include "knmusicutil.h"
#include "kndpimanager.h"

#include "knmusicalbumlistdelegate.h"

#define IconSize 16
#define ItemHeight 25
#define TitleHeight 50
#define TextMarginX 45
#define Spacing 5

using namespace MusicUtil;

KNMusicAlbumListDelegate::KNMusicAlbumListDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void KNMusicAlbumListDelegate::paint(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    //Get the model.
    const QAbstractItemModel *proxyModel=index.model();
    int itemHeight=knDpi->height(ItemHeight),
            itemY=option.rect.y()+option.rect.height()-itemHeight;
    //Configure the painter.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Check out the selection.
    if(option.state & QStyle::State_Selected)
    {
        //Paint the selection base.
        painter->fillRect(QRect(option.rect.x(), itemY,
                                option.rect.width(), itemHeight),
                          option.palette.color(QPalette::Highlight));
    }
    //Use the normal text.
    painter->setPen(option.palette.color(QPalette::WindowText));
    //Draw the playing icon.
    QIcon &&icon=proxyModel->data(proxyModel->index(index.row(),
                                                    MusicRowState),
                                  Qt::DecorationRole).value<QIcon>();
    //Check out the validation of icon.
    if(!icon.isNull())
    {
        //Calculate the scaled size.
        QSize scaledSize=knDpi->size(IconSize, IconSize);
        //Scale the icon.
        QPixmap &&itemIcon=icon.pixmap(scaledSize).scaled(
                    scaledSize,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation);
        //Calculate the offset.
        int positionOffset=((itemHeight-itemIcon.height())>>1);
        //Draw the scaled icon.
        painter->drawPixmap(option.rect.x()+positionOffset,
                            itemY+positionOffset,
                            itemIcon);
    }
    //Get text color.
    QColor textColor=option.palette.color(QPalette::Text);
    //Set the color for others.
    textColor.setAlpha(0x9A);
    painter->setPen(textColor);
    //Calculate the text x.
    int textX=option.rect.x()+knDpi->width(TextMarginX+Spacing),
        textWidth=option.rect.width()-knDpi->width(TextMarginX+(Spacing<<2));
    //Check the size.
    if(option.rect.height()>itemHeight)
    {
        //Need to draw the title.
        painter->drawText(textX, option.rect.y(),
                          option.rect.width(), itemHeight,
                          Qt::AlignVCenter | Qt::AlignLeft,
                          QString("DISC %1").arg(
                              QString::number(
                                  index.data(AlbumDiscRole).toInt())));
    }
    //Get the duration text.
    QString &&durationText=proxyModel->data(proxyModel->index(index.row(),
                                                              Time),
                                            Qt::DisplayRole).toString();
    //Draw the duration text.
    painter->drawText(textX,
                      itemY,
                      textWidth,
                      itemHeight,
                      Qt::AlignRight | Qt::AlignVCenter,
                      durationText);
    //Draw the track index.
    painter->drawText(QRect(QPoint(option.rect.x(), itemY),
                            knDpi->size(TextMarginX, ItemHeight)),
                      Qt::AlignRight | Qt::AlignVCenter,
                      QString::number(index.data(AlbumTrackRole).toInt()));

    //Draw the title text.
    painter->setPen(option.palette.color(QPalette::Text));
    textWidth-=option.fontMetrics.width(durationText);
    painter->drawText(QRect(textX, itemY, textWidth, itemHeight),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      option.fontMetrics.elidedText(
                          proxyModel->data(proxyModel->index(index.row(),
                                                             Name),
                                                    Qt::DisplayRole).toString(),
                          Qt::ElideRight,
                          textWidth));
}

QSize KNMusicAlbumListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    //Check for the first line.
    if(index.data(AlbumDiscTotalRole).toInt()>1 && (index.row()==0 ||
            (index.data(AlbumDiscRole).toInt()!=
            index.model()->index(index.row()-1,
                                 index.column()).data(AlbumDiscRole).toInt())))
    {
        return QSize(option.fontMetrics.width(
                         index.data(Qt::DisplayRole).toString())+38,
                     knDpi->height(TitleHeight));
    }
    //Calculate the size hint for normal line.
    return QSize(option.fontMetrics.width(
                     index.data(Qt::DisplayRole).toString())+38,
                 knDpi->height(ItemHeight));
}
