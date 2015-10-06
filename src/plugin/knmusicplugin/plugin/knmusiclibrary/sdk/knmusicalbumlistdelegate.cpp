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

#include "knmusicalbumlistdelegate.h"

#define IconSize 16
#define ItemHeight 25
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
    //Configure the painter.
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform, true);
    //Check out the selection.
    if(option.state & QStyle::State_Selected)
    {
        //Paint the selection base.
        painter->fillRect(option.rect,
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
        //Scaled playing icon.
        QPixmap &&scaledIcon=icon.pixmap(IconSize);
        //Draw the scaled icon.
        painter->drawPixmap(option.rect.x()+Spacing,
                            option.rect.y()+
                            ((ItemHeight-scaledIcon.height())>>1),
                            scaledIcon);
    }
    //Get text color.
    QColor textColor=option.palette.color(QPalette::Text);
    //Set the color for others.
    textColor.setAlpha(0x9A);
    painter->setPen(textColor);
    //Calculate the text x.
    int textX=option.rect.x()+TextMarginX+Spacing,
        textWidth=option.rect.width()-TextMarginX-(Spacing<<1)-Spacing;
    //Get the duration text.
    QString &&durationText=proxyModel->data(proxyModel->index(index.row(),
                                                              Time),
                                            Qt::DisplayRole).toString();
    //Draw the duration text.
    painter->drawText(textX,
                      option.rect.y(),
                      textWidth,
                      option.rect.height(),
                      Qt::AlignRight | Qt::AlignVCenter,
                      durationText);
    //Draw the track index.
    painter->drawText(option.rect.x(),
                      option.rect.y(),
                      TextMarginX,
                      ItemHeight,
                      Qt::AlignRight | Qt::AlignVCenter,
                      proxyModel->data(proxyModel->index(index.row(),
                                                         TrackNumber),
                                       Qt::DisplayRole).toString());

    //Draw the title text.
    painter->setPen(option.palette.color(QPalette::Text));
    textWidth-=option.fontMetrics.width(durationText);
    painter->drawText(QRect(textX,
                            option.rect.y(),
                            textWidth,
                            option.rect.height()),
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
    //Calculate the size hint.
    return QSize(option.fontMetrics.width(index.data(Qt::DisplayRole).toString())+38,
                 25);
}

