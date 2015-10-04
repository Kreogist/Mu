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

#include "knmusicnowplayingdelegate.h"

#include <QDebug>

using namespace MusicUtil;

KNMusicNowPlayingDelegate::KNMusicNowPlayingDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void KNMusicNowPlayingDelegate::paint(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    //Check the index.
    if(!index.isValid())
    {
        return;
    }
    //Calculate the spacing.
    int spacing=option.rect.height()/10;
    //Check selection.
    if(option.state & QStyle::State_Selected)
    {
        //Paint the selection base.
        painter->fillRect(option.rect,
                          option.palette.color(QPalette::Highlight));
        //Change the pen.
        painter->setPen(option.palette.color(QPalette::HighlightedText));
    }
    else
    {
        //Use the bottom line color.
        painter->setPen(QColor(0x21, 0x21, 0x21));
        //Draw the bottom line.
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());

        //Use the normal text.
        painter->setPen(QColor(0xd1, 0xd1, 0xd1));
    }
    //Get the model.
    const QAbstractItemModel *proxyModel=index.model();

    //Draw the playing icon.
    QIcon indicatorIcon=proxyModel->data(proxyModel->index(index.row(),
                                                           MusicRowState),
                                         Qt::DecorationRole).value<QIcon>();
    //Check the valid of the decorate data.
    if(!indicatorIcon.isNull())
    {
        //Rescaled the icon.
        QPixmap rescaledPixmap=indicatorIcon.pixmap(16);
        //Calculate the offset.
        int positionOffset=((option.rect.height()-rescaledPixmap.height())>>1);
        //Draw the indicator icon.
        painter->drawPixmap(option.rect.x()+positionOffset,
                            option.rect.y()+positionOffset,
                            rescaledPixmap);
    }
    //Draw the music duraiton.
    QString timeText=textData(proxyModel, index, Time);
    painter->setFont(option.font);
    //Calculate the position for text.
    int textX=option.rect.x()+(spacing<<1)+option.rect.height(),
        textY=option.rect.y()+spacing,
        textWidth=option.rect.width()-spacing-(spacing<<1)-option.rect.height(),
        textHeight=option.rect.height()-(spacing<<1);
    painter->drawText(QRect(textX,
                            textY,
                            textWidth,
                            textHeight),
                      Qt::AlignRight | Qt::AlignVCenter,
                      timeText);
    //Get the time text width.
    int nameTextWidth=textWidth-spacing-option.fontMetrics.width(timeText);
    //Draw the music name.
    painter->drawText(QRect(textX,
                            textY,
                            nameTextWidth,
                            textHeight),
                      Qt::AlignLeft | Qt::AlignVCenter,
                      option.fontMetrics.elidedText(textData(proxyModel,
                                                             index,
                                                             Name),
                                                    Qt::ElideRight,
                                                    nameTextWidth));
}

QSize KNMusicNowPlayingDelegate::sizeHint(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    return QSize(option.fontMetrics.width(textData(index.model(),
                                                   index,
                                                   Name))+20,
                 option.fontMetrics.height()*1.618);
}

inline QString KNMusicNowPlayingDelegate::textData(
        const QAbstractItemModel *model,
        const QModelIndex &rowIndex,
        const int &column) const
{
    return model->data(model->index(rowIndex.row(), column)).toString();
}
