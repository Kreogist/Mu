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
#include <QScrollBar>
#include <QPainter>

#include "knthememanager.h"

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomesongview.h"

#include <QDebug>

#define PaintTopMargin      9
#define SongArtworkSize     44
#define SongItemHeight      44
#define SongItemWidth       270
#define SongAreaWidth       290
#define SongAreaHeight      49
#define SongViewHeight      220
#define SongItemHSpacing    20
#define SongTextWidth       218
#define SongTextHSpacing    8

KNMusicStoreHomeSongView::KNMusicStoreHomeSongView(QWidget *parent) :
    KNMusicStoreHomeItemView(parent),
    m_noAlbumArtCache(QPixmap(":/plugin/music/public/noalbum.png")),
    m_leftShadowCache(QPixmap(":/plugin/music/store/store_view_left.png")),
    m_rightShadowCache(QPixmap(":/plugin/music/store/store_view_right.png"))
{
    setObjectName("MusicStoreHomeNewSong");
    //Set properties.
    setFixedHeight(SongViewHeight);
    //Configure the scroll bar.
    horizontalScrollBar()->setSingleStep(SongAreaWidth>>2);
    horizontalScrollBar()->setPageStep(SongAreaWidth>>1);
    //Customized the scroll bar.
    QScrollBar *customScrollBar=scrollBar();
    customScrollBar->setSingleStep(SongAreaWidth);
    customScrollBar->setPageStep(SongAreaWidth<<2);
    //Initial the cache.
    m_noAlbumArtCache=m_noAlbumArtCache.scaled(SongArtworkSize, SongArtworkSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);

    //Link theme.
    knTheme->registerWidget(this);
}

QModelIndex KNMusicStoreHomeSongView::indexAt(const QPoint &point) const
{
    //Check model is nullptr or not.
    if(listModel()==nullptr)
    {
        //For null model, return null value.
        return QModelIndex();
    }
    //Get the x and y position.
    int xPos=point.x()+horizontalScrollBar()->value(), yPos=point.y();
    //Check the point position.
    if(yPos<PaintTopMargin || yPos>PaintTopMargin+(SongItemHeight<<2))
    {
        //Must be a null position.
        return QModelIndex();
    }
    //Calculate the index position.
    int targetColumn=xPos/SongAreaWidth,
        targetRow=(yPos-PaintTopMargin)/SongAreaHeight;
    //Check the inner spacing.
    if(((targetColumn-1)*SongAreaWidth-xPos)>SongAreaWidth)
    {
        //Click at the spacing.
        return QModelIndex();
    }
    //Calculate the index.
    return listModel()->index(targetColumn*4+targetRow);
}

QRect KNMusicStoreHomeSongView::visualRect(const QModelIndex &index) const
{
    //Check invalidation of the index.
    if(!index.isValid())
    {
        //Null data for invalid index.
        return QRect();
    }
    //Calculate the table position of the index.
    int tableXPos=index.row()>>2, tableYPos=index.row()-(tableXPos<<2),
            xPos=(tableXPos*SongAreaWidth)-horizontalScrollBar()->value(),
            yPos=(tableYPos*SongAreaHeight)+PaintTopMargin;
    //Return the rect.
    return QRect(xPos, yPos, SongItemWidth, SongItemHeight);
}

void KNMusicStoreHomeSongView::setModel(QAbstractItemModel *model)
{
    //Do original event.
    KNMusicStoreHomeItemView::setModel(model);
    //This model must be a list model, recast the model.
    KNMusicStoreHomeListModel *albumListModel=
            static_cast<KNMusicStoreHomeListModel *>(model);
    //Update artwork size.
    albumListModel->setAlbumArtSize(SongArtworkSize);
}

void KNMusicStoreHomeSongView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Check the model pointer, if the model pointer is null, ignore the paint
    //event.
    if(!listModel())
    {
        //Ignore to paint null model.
        return;
    }
    //Get the list model.
    KNMusicStoreHomeListModel *homeListModel=listModel();
    //Initial the painter.
    QPainter painter(viewport());
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    painter.setPen(palette().color(QPalette::WindowText));
    //Calculate the start index.
    int currentRow=(horizontalScrollBar()->value()/SongAreaWidth)<<2,
            //Calculate the start x.
            currentX=(currentRow>>2)*SongAreaWidth-
                horizontalScrollBar()->value(),
            currentY=PaintTopMargin,
            columnCounter=0;
    //Start to paint the album item.
    while(currentRow<homeListModel->rowCount())
    {
        //Draw the pixmap first.
        const KNMusicStoreHomeItem &songItem=
                homeListModel->homeItem(currentRow);
        //Draw the song album.
        painter.drawPixmap(currentX, currentY,
                           songItem.artwork.isNull()?
                               m_noAlbumArtCache:
                               songItem.artwork);
        //Calculate the text position.
        int textXPos=currentX+SongArtworkSize+SongTextHSpacing,
            textYPos=currentY+10;
        //Draw the song album title.
        painter.drawText(QRect(textXPos, textYPos,
                               SongTextWidth, fontMetrics().height()),
                         Qt::AlignTop | Qt::AlignLeft,
                         fontMetrics().elidedText(songItem.title,
                                                  Qt::ElideRight,
                                                  SongTextWidth));
        //Change opacity.
        painter.setOpacity(0.5);
        //Increase the y pos.
        textYPos+=fontMetrics().height();
        //Draw the subtitle.
        painter.drawText(QRect(textXPos, textYPos,
                               SongTextWidth, fontMetrics().height()),
                         Qt::AlignTop | Qt::AlignLeft,
                         fontMetrics().elidedText(songItem.subheading,
                                                  Qt::ElideRight,
                                                  SongTextWidth));
        //Reset the opacity.
        painter.setOpacity(1.0);
        //Increase counter.
        ++columnCounter;
        //Check counter.
        if(4==columnCounter)
        {
            //Reset counter.
            columnCounter=0;
            //Reset current Y.
            currentY=PaintTopMargin;
            //Increase column.
            currentX+=SongAreaWidth;
        }
        else
        {
            //Increase current y.
            currentY+=SongAreaHeight;
        }
        //Increase the row.
        ++currentRow;
    }
    //Get the horizontal scroll bar value.
    int horizontalValue=horizontalScrollBar()->value();
    //Check the value.
    if(horizontalValue!=0)
    {
        //Draw the left part.
        qreal opacityLeft=(qreal)horizontalValue/(qreal)SongTextWidth;
        //Check the maximum
        painter.setOpacity(opacityLeft>1.0000?1.0:opacityLeft);
        //Draw left shadow.
        painter.drawPixmap(QRect(0, 0, m_leftShadowCache.width(), height()),
                           m_leftShadowCache,
                           QRect(QPoint(0, 0), m_leftShadowCache.size()));
    }
    if(horizontalValue!=horizontalScrollBar()->maximum())
    {
        //Draw right part.
        qreal opacityRight=(qreal)(horizontalScrollBar()->maximum() -
                                   horizontalValue) /
                            (qreal)SongTextWidth;
        //Check the maximum, set opacity.
        painter.setOpacity(opacityRight>1.0000?1.0:opacityRight);
        //Draw right shadow.
        painter.drawPixmap(QRect(width()-m_rightShadowCache.width(), 0,
                                 m_rightShadowCache.width(), height()),
                           m_rightShadowCache,
                           QRect(QPoint(0, 0), m_rightShadowCache.size()));
    }
}

void KNMusicStoreHomeSongView::updateGeometries()
{
    //Update the range of the horizontal scroll bar.
    horizontalScrollBar()->setRange(
                0,
                (SongAreaWidth<<3)-width());
}
