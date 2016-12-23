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
#include <QMouseEvent>
#include <QScrollBar>

#include "knthememanager.h"

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomealbumview.h"

#include <QDebug>

#define AlbumArtSize            125
#define AlbumHorizontalSpacing  20
#define AlbumHorizontalSize     145
#define AlbumVerticalSize       191
#define AlbumViewHeight         390
#define AlbumAreaHeight         172
#define AlbumAreaWidth          125
#define AlbumTitleSpacing       9
#define AlbumTextAreaHeight     44
#define ShadowSize              151
#define ShadowIncrease          12
#define PaintTopMargin          7

KNMusicStoreHomeAlbumView::KNMusicStoreHomeAlbumView(QWidget *parent) :
    KNMusicStoreHomeItemView(parent),
    m_noAlbumArtCache(QPixmap(":/plugin/music/public/noalbum.png")),
    m_albumShadow(QPixmap(ShadowSize, ShadowSize)),
    m_leftShadowCache(QPixmap(":/plugin/music/store/store_view_left.png")),
    m_rightShadowCache(QPixmap(":/plugin/music/store/store_view_right.png"))
{
    setObjectName("MusicStoreHomeNewAlbum");
    //Set properties.
    setFixedHeight(AlbumViewHeight);
    //Configure the scroll bar.
    horizontalScrollBar()->setSingleStep(AlbumHorizontalSize>>2);
    horizontalScrollBar()->setPageStep(AlbumHorizontalSize>>1);
    //Customized the scroll bar.
    QScrollBar *customScrollBar=scrollBar();
    customScrollBar->setSingleStep(100);
    customScrollBar->setPageStep(AlbumHorizontalSize<<3);
    //Initial the cache.
    m_noAlbumArtCache=m_noAlbumArtCache.scaled(AlbumArtSize, AlbumArtSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
    //Initial the album shadow.
    QPixmap shadowSource("://public/shadow.png");
    //Initial the shadow pixmap.
    m_albumShadow.fill(QColor(255, 255, 255, 0));
    //Prepare the parameters.
    int sourceSize=shadowSource.width(),
        blockSize=sourceSize/3,
        blockSize2x=blockSize<<1,
        destinationWidth=ShadowSize-blockSize2x,
        destinationHeight=ShadowSize-blockSize2x;
    //Initial the antialiasing painter.
    QPainter painter(&m_albumShadow);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    painter.setOpacity(0.8);
    //Draw Top-left shadow.
    painter.drawPixmap(QRect(0,0,blockSize,blockSize),
                       shadowSource,
                       QRect(0,0,blockSize,blockSize));
    //Draw Top-Middle shadow.
    painter.drawPixmap(QRect(blockSize,0,destinationWidth-blockSize,blockSize),
                       shadowSource,
                       QRect(blockSize,0,blockSize,blockSize));
    //Draw Top-Right shadow.
    painter.drawPixmap(QRect(destinationWidth,0,blockSize2x, blockSize),
                       shadowSource,
                       QRect(blockSize,0,blockSize2x,blockSize));
    //Draw Middle-Left shadow.
    painter.drawPixmap(QRect(0,blockSize,blockSize,destinationHeight-blockSize),
                       shadowSource,
                       QRect(0,blockSize,blockSize,blockSize));
    //Draw Middle-Right shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             blockSize,
                             blockSize,
                             destinationHeight),
                       shadowSource,
                       QRect(blockSize2x,blockSize,blockSize,blockSize));
    //Draw Left-Bottom shadow.
    painter.drawPixmap(QRect(0,destinationHeight,blockSize, blockSize2x),
                       shadowSource,
                       QRect(0,blockSize,blockSize,blockSize2x));
    //Draw Middle-Bottom shadow.
    painter.drawPixmap(QRect(blockSize,
                             destinationHeight+blockSize,
                             destinationWidth,
                             blockSize),
                       shadowSource,
                       QRect(blockSize,blockSize2x,blockSize,blockSize));
    //Draw Right-Buttom shadow.
    painter.drawPixmap(QRect(destinationWidth+blockSize,
                             destinationHeight+blockSize,
                             blockSize,
                             blockSize),
                       shadowSource,
                       QRect(blockSize2x,blockSize2x,blockSize,blockSize));
    painter.end();

    //Link theme manager.
    knTheme->registerWidget(this);
}

QModelIndex KNMusicStoreHomeAlbumView::indexAt(const QPoint &point) const
{
    //Check model is nullptr or not.
    if(listModel()==nullptr)
    {
        //For null model, return null value.
        return QModelIndex();
    }
    //Calculate the column and row.
    int viewX=point.x()+horizontalScrollBar()->value(),
        targetColumn=viewX/AlbumHorizontalSize,
        targetRow=point.y()>AlbumVerticalSize;
    //For a single item, check the target width and column position, make sure
    //that it is in the position.
    if((viewX-targetColumn*AlbumHorizontalSize)>AlbumAreaWidth ||
            point.y()-targetRow*AlbumVerticalSize>AlbumAreaHeight)
    {
        //Outside the part, an invalid index is set.
        return QModelIndex();
    }
    //The item is listed from left to right, from top to bottom.
    return listModel()->index((targetColumn<<1)+targetRow, 0);
}

QRect KNMusicStoreHomeAlbumView::visualRect(const QModelIndex &index) const
{
    //Check the invalid index.
    if(!index.isValid())
    {
        //For null index, it is invalid.
        return QRect();
    }
    //Calculate the real position.
    int xPos=(index.row()>>1)*AlbumHorizontalSize-
            horizontalScrollBar()->value(),
            //Calculate the vertical position.
            yPos=(index.column()&1)*AlbumVerticalSize;
    //All the part of the album is visible.
    return QRect(xPos, yPos, AlbumAreaWidth, AlbumAreaHeight);
}

void KNMusicStoreHomeAlbumView::paintEvent(QPaintEvent *event)
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
    int currentRow=(horizontalScrollBar()->value()/AlbumHorizontalSize)<<1,
        //Calculate the start x.
        currentX=(currentRow>>1)*AlbumHorizontalSize-
            horizontalScrollBar()->value(),
        currentY=PaintTopMargin;
    //Start to paint the album item.
    while(currentRow<homeListModel->rowCount())
    {
        //Draw the pixmap first.
        const KNMusicStoreHomeItem &albumItem=
                homeListModel->homeItem(currentRow);
        //Check the album art is null or not.
        //Draw the pixmap shadow first.
        painter.drawPixmap(currentX-ShadowIncrease, currentY-ShadowIncrease,
                           m_albumShadow);
        //Draw the album art pixmap.
        painter.drawPixmap(currentX, currentY,
                           albumItem.artwork.isNull()?
                               m_noAlbumArtCache:
                               albumItem.artwork);
        //Get the title text.
        QString titleText=fontMetrics().elidedText(albumItem.title,
                                                   Qt::ElideRight,
                                                   AlbumAreaWidth+
                                                   (AlbumAreaWidth>>1));
        //Prepare the title area.
        QRect titleArea=QRect(currentX, currentY+AlbumArtSize+AlbumTitleSpacing,
                              AlbumAreaWidth, AlbumTextAreaHeight);
        //Calculate the real title area.
        titleArea=fontMetrics().boundingRect(titleArea,
                                             Qt::AlignLeft | Qt::AlignTop |
                                             Qt::TextWordWrap,
                                             titleText);
        //Draw the album data.
        painter.drawText(titleArea,
                         Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                         titleText);
        //Change to subtitle opacity.
        painter.setOpacity(0.5);
        //Draw the album text.
        int subtitleHeight=AlbumTextAreaHeight-titleArea.height();
        painter.drawText(QRect(currentX,
                               titleArea.y()+titleArea.height(),
                               AlbumAreaWidth,
                               subtitleHeight),
                         Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                         fontMetrics().elidedText(
                             albumItem.subheading,
                             Qt::ElideRight,
                             subtitleHeight/fontMetrics().height()*
                             AlbumAreaWidth));
        //Reset the opacity.
        painter.setOpacity(1.0);
        //Change the position.
        if(currentY==PaintTopMargin)
        {
            //Increase the Y position only.
            currentY+=AlbumVerticalSize;
        }
        else
        {
            //Reset Y position.
            currentY=PaintTopMargin;
            //Increase X position.
            currentX+=AlbumHorizontalSize;
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
        qreal opacityLeft=(qreal)horizontalValue/(qreal)AlbumHorizontalSize;
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
                            (qreal)AlbumHorizontalSize;
        //Check the maximum, set opacity.
        painter.setOpacity(opacityRight>1.0000?1.0:opacityRight);
        //Draw right shadow.
        painter.drawPixmap(QRect(width()-m_rightShadowCache.width(), 0,
                                 m_rightShadowCache.width(), height()),
                           m_rightShadowCache,
                           QRect(QPoint(0, 0), m_rightShadowCache.size()));
    }
}

void KNMusicStoreHomeAlbumView::updateGeometries()
{
    //Update the range of the horizontal scroll bar.
    horizontalScrollBar()->setRange(0,
                                    (AlbumHorizontalSize<<4)-
                                    AlbumHorizontalSpacing-width());
}
