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

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomealbumview.h"

#define AlbumArtSize            125
#define AlbumHorizontalSpacing  20
#define AlbumHorizontalSize     145
#define AlbumVerticalSize       191
#define AlbumViewHeight         390
#define AlbumAreaHeight         172
#define AlbumAreaWidth          125
#define AlbumTitleSpacing       10

KNMusicStoreHomeAlbumView::KNMusicStoreHomeAlbumView(QWidget *parent) :
    QAbstractItemView(parent),
    m_noAlbumArtCache(QPixmap(":/plugin/music/public/noalbum.png")),
    m_listModel(nullptr)
{
    //Set properties.
    setFixedHeight(AlbumViewHeight);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Initial the cache.
    m_noAlbumArtCache=m_noAlbumArtCache.scaled(AlbumArtSize, AlbumArtSize,
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
}

QModelIndex KNMusicStoreHomeAlbumView::indexAt(const QPoint &point) const
{
    //Check model is nullptr or not.
    if(!m_listModel)
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
    if((!model()) || viewX > AlbumAreaWidth || point.y() > AlbumAreaHeight)
    {
        //Outside the part, an invalid index is set.
        return QModelIndex();
    }
    //The item is listed from left to right, from top to bottom.
    return m_listModel->index(targetColumn * 2 + targetRow, 0);
}

void KNMusicStoreHomeAlbumView::scrollTo(const QModelIndex &index,
                                         QAbstractItemView::ScrollHint hint)
{
    Q_UNUSED(index)
    Q_UNUSED(hint)
    //Ignore the scroll to event.
}

QRect KNMusicStoreHomeAlbumView::visualRect(const QModelIndex &index) const
{
    //Calculate the real position.
    int xPos=(index.row()>>1)*AlbumHorizontalSize-
            horizontalScrollBar()->value();
    //Check the validation of the position.
    if(xPos<-AlbumAreaWidth || xPos>width())
    {
        //Nothing is visible.
        return QRect();
    }
    //Calculate the vertical position.
    int yPos=(index.column()&1)*AlbumVerticalSize;
    //Part or all the part of the album is visible.
    if(xPos<0)
    {
        //Only a part of album is visible, it is at the left side of the widget.
        return QRect(0, yPos, AlbumAreaWidth+xPos, AlbumAreaHeight);
    }
    if(xPos>width()-AlbumAreaWidth)
    {
        //Only a part of album is visible, it is at the right side of the
        //widget.
        return QRect(xPos, yPos, width()-xPos, AlbumAreaHeight);
    }
    //All the part of the album is visible.
    return QRect(xPos, yPos, AlbumAreaWidth, AlbumAreaHeight);
}

void KNMusicStoreHomeAlbumView::setModel(QAbstractItemModel *model)
{
    //Set the model to view.
    QAbstractItemView::setModel(model);
    //This model must be a list model.
    m_listModel=static_cast<KNMusicStoreHomeListModel *>(model);
}

QModelIndex KNMusicStoreHomeAlbumView::moveCursor(
        QAbstractItemView::CursorAction cursorAction,
        Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(cursorAction)
    Q_UNUSED(modifiers)
    //Ignore the move cursor.
    return QModelIndex();
}

void KNMusicStoreHomeAlbumView::setSelection(
        const QRect &rect,
        QItemSelectionModel::SelectionFlags command)
{
    Q_UNUSED(rect)
    Q_UNUSED(command)
}

void KNMusicStoreHomeAlbumView::paintEvent(QPaintEvent *event)
{
    //Check the model pointer, if the model pointer is null, ignore the paint
    //event.
    if(!m_listModel)
    {
        //Ignore to paint null model.
        return;
    }
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Calculate the start index.
    int currentRow=horizontalScrollBar()->value()/AlbumHorizontalSize,
        //Calculate the start x.
        currentX=currentRow*AlbumHorizontalSize-horizontalScrollBar()->value(),
        currentY=0;
    //Start to paint the album item.
    while(currentRow<m_listModel->rowCount())
    {
        //Draw the pixmap first.
        QPixmap albumArt=m_listModel->albumArt(currentRow);
        //Check the album art is null or not.
        //Draw the no album pixmap.
        painter.drawPixmap(currentX, currentY,
                           albumArt.isNull()?m_noAlbumArtCache:albumArt);
        //Draw the album data.
//        painter.drawText(QRect(currentX, currentY+AlbumTitleSpacing,
//                               AlbumAreaWidth, fontMetrics().height()<<1),
//                         m_listModel);
        //Change the position.
        if(currentY==0)
        {
            //Increase the Y position only.
            currentY=AlbumVerticalSize;
        }
        else
        {
            //Reset Y position.
            currentY=0;
            //Increase X position.
            currentX+=AlbumHorizontalSize;
        }
        //Increase the row.
        ++currentRow;
    }
}

int KNMusicStoreHomeAlbumView::horizontalOffset() const
{
    //The horizontal offset is the scroll bar value.
    return horizontalScrollBar()->value();
}

int KNMusicStoreHomeAlbumView::verticalOffset() const
{
    //The vertical offset is the scroll bar value. However, it is const 0.
    return 0;
}

bool KNMusicStoreHomeAlbumView::isIndexHidden(const QModelIndex &index) const
{
    //Treat all the index visible.
    Q_UNUSED(index)
    return false;
}

QRegion KNMusicStoreHomeAlbumView::visualRegionForSelection(
        const QItemSelection &selection) const
{
    Q_UNUSED(selection)
    return QRegion();
}

void KNMusicStoreHomeAlbumView::updateGeometries()
{
    //Update the range of the horizontal scroll bar.
    horizontalScrollBar()->setRange(0,
                                    (AlbumHorizontalSize<<4)-
                                    AlbumHorizontalSpacing-width());
}
