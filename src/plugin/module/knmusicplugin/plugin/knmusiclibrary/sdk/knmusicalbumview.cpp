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
#include <QScrollBar>
#include <QPainter>
#include <QIcon>

#include "knmusiccategoryproxymodel.h"
#include "knmusicalbummodel.h"

#include "knmusicalbumview.h"

#include <QDebug>

KNMusicAlbumView::KNMusicAlbumView(QWidget *parent) :
    QAbstractItemView(parent)
{
    //Set default scrollbar properties.
    verticalScrollBar()->setRange(0, 0);

    //Set the palette.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             m_outBrightness);
    QPalette pal=palette();
    pal.setColor(QPalette::Base, m_backgroundColor);
    pal.setColor(QPalette::Window, m_backgroundColor);
    pal.setColor(QPalette::Button, QColor(0x30, 0x30, 0x30));
    pal.setColor(QPalette::ButtonText, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Text, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);

    updateParameters();
}

QModelIndex KNMusicAlbumView::indexAt(const QPoint &point) const
{
    //Calculate the point content position and the line of the point.
    int pointContentY=verticalScrollBar()->value()+point.y(),
        itemLine=pointContentY/m_itemSpacingHeight;
    //Check whether mouse click on a row spacing part.
    if(pointContentY-itemLine*m_itemSpacingHeight<m_spacing)
    {
        return QModelIndex();
    }
    //Calculate the column of the position.
    int itemColumn=point.x()/m_itemSpacingWidth,
        pointXInItem=point.x()-itemColumn*m_itemSpacingWidth;
    //Check whether mouse click on a column spacing part.
    if(pointXInItem<m_spacing || pointXInItem>m_spacing+m_itemIconSize)
    {
        return QModelIndex();
    }
    //Calculate the item category index.
    int categoryRow=itemLine*m_maxColumnCount+itemColumn;
    //Check if the category row vaild.
    //We should check the category proxy model, because the point is a display
    //variable.
    return (categoryRow>0 && categoryRow<m_proxyModel->rowCount())?
                m_proxyModel->index(categoryRow, 0, rootIndex()):
                QModelIndex();
}

void KNMusicAlbumView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    //Check the index and the max column count.
    if(!index.isValid() || m_maxColumnCount==0)
    {
        return;
    }
    //Check whether we need to move the vertical scroll bar.
    if(hint==QAbstractItemView::EnsureVisible &&
            rect().contains(visualRect(index), true))
    {
        return;
    }
    //Let the scroll bar move to the prefer position.
    verticalScrollBar()->setValue(indexScrollBarValue(index, hint));
}

QRect KNMusicAlbumView::visualRect(const QModelIndex &index) const
{
    //Get the item content rect.
    QRect indexRect=itemContentRect(index);
    //If the rect is vaild, remove the value of the scroll bar.
    return indexRect.isValid()?
              QRect(indexRect.left()-horizontalScrollBar()->value(),
                    indexRect.top()-verticalScrollBar()->value(),
                    indexRect.width(),
                    indexRect.height()):
              QRect();
}

void KNMusicAlbumView::setModel(QAbstractItemModel *model)
{
    //Save the proxy model and the category model.
    m_proxyModel=static_cast<KNMusicCategoryProxyModel *>(model);
    m_model=static_cast<KNMusicAlbumModel *>(m_proxyModel->sourceModel());
    //Set the model.
    QAbstractItemView::setModel(m_proxyModel);
    //Update the geometries.
    updateGeometries();
}

void KNMusicAlbumView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Get the option view item.
    QStyleOptionViewItem option=viewOptions();
    //Check if we need to paint the background.
    if(autoFillBackground())
    {
        painter.fillRect(rect(), option.palette.base());
    }

    //Update the parameters of the view first.
    updateParameters();
    //Check the model first.
    if(m_proxyModel==nullptr)
    {
        return;
    }
    //Get the row count.
    int albumCount=m_proxyModel->rowCount();
    //Calculate the line count.
    m_lineCount=(albumCount+m_maxColumnCount-1)/m_maxColumnCount;
    //Check the album count first.
    if(albumCount==0)
    {
        return;
    }
    int currentColumn=0,
        currentLeft=m_spacing,
        currentLine=verticalScrollBar()->value()/m_itemSpacingHeight, //Skip the before.
        currentTop=m_spacing+currentLine*m_itemSpacingHeight,
        currentRow=currentLine*m_maxColumnCount,
        heightSurplus=height()+m_itemSpacingHeight;
    //Change the origin of coordinate.
    painter.translate(0, -verticalScrollBar()->value());
    //Draw all the albums.
    while(currentRow < albumCount && heightSurplus > 0)
    {
        //Get the source index of the item.
        QModelIndex sourceIndex=m_proxyModel->index(currentRow, 0);
        //If the source index is not the current index, then draw the album.
        if(sourceIndex!=m_selectedIndex)
        {
            paintAlbum(painter,
                       QRect(currentLeft,
                             currentTop,
                             m_itemWidth,
                             m_itemHeight),
                       sourceIndex);
        }
        //Add current row and column.
        currentRow++;
        currentColumn++;
        //Check if we need to move to next row.
        if(currentColumn>=m_maxColumnCount)
        {
            //Add one line.
            currentLine++;
            //Reset the column.
            currentColumn=0;
            //Change the position.
            currentLeft=m_spacing;
            currentTop+=m_itemSpacingHeight;
            heightSurplus-=m_itemSpacingHeight;
        }
        else
        {
            //Move to next column position.
            currentLeft+=m_itemSpacingWidth;
        }
    }
    //Update the scroll bar value.
    updateGeometries();
}

void KNMusicAlbumView::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QAbstractItemView::resizeEvent(event);
}

QModelIndex KNMusicAlbumView::moveCursor(QAbstractItemView::CursorAction cursorAction,
                                         Qt::KeyboardModifiers modifiers)
{
    QModelIndex current=currentIndex(), movedIndex;
    switch (cursorAction)
    {
    case QAbstractItemView::MoveUp:
        movedIndex=m_proxyModel->index(current.row()-m_maxColumnCount, 0);
        break;
    case QAbstractItemView::MoveDown:
        movedIndex=m_proxyModel->index(current.row()+m_maxColumnCount, 0);
        break;
    case QAbstractItemView::MoveLeft:
        movedIndex=m_proxyModel->index(current.row()-1, 0);
        break;
    case QAbstractItemView::MoveRight:
        movedIndex=m_proxyModel->index(current.row()+1, 0);
        break;
    case QAbstractItemView::MoveHome:
        movedIndex=m_proxyModel->index(0, 0);
        break;
    case QAbstractItemView::MoveEnd:
        movedIndex=m_proxyModel->index(m_model->rowCount()-1, 0);
        break;
    default:
        break;
    }
    viewport()->update();
    return current;
}

int KNMusicAlbumView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int KNMusicAlbumView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool KNMusicAlbumView::isIndexHidden(const QModelIndex &index) const
{
    //!FIXME: No album process.
    return false;
}

void KNMusicAlbumView::setSelection(const QRect &rect,
                                    QItemSelectionModel::SelectionFlags command)
{
    selectionModel()->select(indexAt(QPoint(rect.x(),rect.y())),
                             command);
}

QRegion KNMusicAlbumView::visualRegionForSelection(const QItemSelection &selection) const
{
    //If there's no selection, of course nothing.
    if(selection.size()==0)
    {
        return QRect();
    }
    //Add the only select item to the region, return the region.
    QItemSelectionRange range=selection.at(0);
    QRegion region;
    region+=visualRect(indexAt(QPoint(range.top(), range.left())));
    return region;
}

void KNMusicAlbumView::updateGeometries()
{
    //Update the range of the vertical scroll bar.
    verticalScrollBar()->setRange(0,
                                  qMax(0,
                                       m_lineCount*m_itemSpacingHeight+m_spacing-height()));
    //Update the page and single step.
    verticalScrollBar()->setPageStep(m_itemSpacingHeight>>1);
    verticalScrollBar()->setSingleStep(m_itemSpacingHeight>>1);
}

void KNMusicAlbumView::paintAlbum(QPainter &painter,
                                  const QRect &rect,
                                  const QModelIndex &index)
{
    //Ensure the index is vaild.
    if(!index.isValid())
    {
        return;
    }
    //Draw the album art first.
    QIcon currentIcon=
            m_proxyModel->data(index, Qt::DecorationRole).value<QIcon>();
    painter.drawPixmap(QRect(rect.x()+1,
                              rect.y()-1,
                              m_itemIconSize-2,
                              m_itemIconSize-2),
                        currentIcon.pixmap(m_itemIconSize, m_itemIconSize));
    //Get the option view item.
    QStyleOptionViewItem option=viewOptions();
    //Set the pen as the text color.
    QColor textColor=option.palette.color(QPalette::Text);
    painter.setPen(textColor);
    //Draw the album text.
    //Draw the album name first.
    painter.drawText(rect.x(),
                     rect.y()+m_itemIconSize,
                     m_itemIconSize,
                     fontMetrics().height(),
                     Qt::AlignLeft,
                     fontMetrics().elidedText(index.data(Qt::DisplayRole).toString(),
                                              Qt::ElideRight,
                                              m_itemIconSize));
    //Draw the album artist name.
    //Get the album list.
    QHash<QString, QVariant> artistList=index.data(CategoryArtistList).toHash();
    if(!artistList.isEmpty())
    {
        //Set color.
        textColor.setAlpha(textColor.alpha()>>1);
        painter.setPen(textColor);
        //Draw the text.
        painter.drawText(rect.x(),
                         rect.y()+m_itemIconSize+fontMetrics().height(),
                         m_itemIconSize,
                         fontMetrics().height(),
                         Qt::AlignLeft,
                         fontMetrics().elidedText(artistList.size()==1?
                                                      artistList.keys().first():
                                                      tr("Various Artists"),
                                                  Qt::ElideRight,
                                                  m_itemIconSize));
    }
}

int KNMusicAlbumView::indexScrollBarValue(const QModelIndex &index,
                                          QAbstractItemView::ScrollHint hint)
{
    //Get the top of index position, set it to scroll value.
    int topPositionValue=index.row()/m_maxColumnCount*m_itemSpacingHeight;
    //Change the item content Y according to the hint.
    switch(hint)
    {
    case QAbstractItemView::PositionAtTop:
        //No need to change.
        return topPositionValue;
    case QAbstractItemView::PositionAtCenter:
        //Reduce a half of the viewer height to move up.
        return topPositionValue-((height()-m_itemSpacingHeight)>>1);
    case QAbstractItemView::PositionAtBottom:
        //Reduce the whole viewer height to move up.
        return topPositionValue-height()+m_itemSpacingHeight;
        break;
    case QAbstractItemView::EnsureVisible:
        //Now, the index item must be a unvisible one in the viewer.
        //We have already has the top position, calculate the bottom position,
        //and calculate the distence of the current vertical scroll bar's value
        //to these two position.
        int bottomPositionValue=topPositionValue-height()+m_itemSpacingHeight;
        //If to the bottom is lesser than to top, change the value.
        return (qAbs(verticalScrollBar()->value()-bottomPositionValue)<
                qAbs(verticalScrollBar()->value()-topPositionValue))?
                    bottomPositionValue:
                    topPositionValue;
    }
}

QRect KNMusicAlbumView::itemContentRect(const QModelIndex &index) const
{
    //Check the index first.
    if(!index.isValid() || m_maxColumnCount==0)
    {
        return QRect();
    }
    //Calculate the item line.
    int itemIndex=index.row(),
        itemLine=itemIndex/m_maxColumnCount;
    //Calculate the rect.
    return QRect((itemIndex-itemLine*m_maxColumnCount)*m_itemSpacingWidth,
                 itemLine*m_itemSpacingHeight,
                 m_itemWidth,
                 m_itemHeight);
}

void KNMusicAlbumView::updateParameters()
{
    //Get the usable width.
    int usableWidth=width()-m_spacing,
        minimalWidth=m_itemMinimalWidth+m_spacing;
    //When the usable width is lesser than the minimal width,
    //Force set the max column count to 1.
    if(usableWidth<minimalWidth)
    {
        m_maxColumnCount=1;
        m_itemWidth=width();
    }
    else
    {
        //Calculate how many column we can put in one row.
        m_maxColumnCount=usableWidth/minimalWidth;
        //Calculate the real item width, it's must be larger than minimal width.
        m_itemWidth=usableWidth/m_maxColumnCount-m_spacing;
    }
    //The icon size should be the item width.
    m_itemIconSize=m_itemWidth;
    //The height of the item should be a item icon size and two text lines.
    m_itemHeight=m_itemIconSize+(fontMetrics().height()<<1);
    //Calcualte the spacing item width and height.
    m_itemSpacingHeight=m_spacing+m_itemHeight;
    m_itemSpacingWidth=m_spacing+m_itemWidth;
}
