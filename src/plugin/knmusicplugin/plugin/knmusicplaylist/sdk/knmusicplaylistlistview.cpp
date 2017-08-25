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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>

#include "kndpimanager.h"

#include "knmusicmodel.h"

#include "knmusicplaylistlistdelegate.h"
#include "knmusicplaylistlistmodel.h"

#include "knmusicplaylistlistview.h"

#include <QDebug>

#define DropSideMargin 5

KNMusicPlaylistListView::KNMusicPlaylistListView(QWidget *parent) :
    KNMusicCategoryListViewBase(parent),
    m_dropSideMargin(knDpi->width(DropSideMargin))
{
    //Enabled the drag and drop function.
    enabledDragDrop();
    //Set properties.
    setIconSize(knDpi->size(40, 40));
    setDragDropMode(QAbstractItemView::DropOnly);
    //Initial the delegate.
    setItemDelegate(new KNMusicPlaylistListDelegate(this));
}

QString KNMusicPlaylistListView::currentPlaylistTitle() const
{
    //If current playlist is valid(according to the currentIndex()), then get
    //the data of the display role, and transfer it to string.
    return currentIndex().isValid()?
                currentIndex().data(Qt::DisplayRole).toString():
                QString();
}

void KNMusicPlaylistListView::dragEnterEvent(QDragEnterEvent *event)
{
    //Check the model.
    if(model()==nullptr)
    {
        //Ignore the drag enter event.
        event->ignore();
        return;
    }
    //Accept all the other event.
    event->accept();
    //Set the state.
    setState(DraggingState);
}

void KNMusicPlaylistListView::dragMoveEvent(QDragMoveEvent *event)
{
    //Ignore the event by default.
    event->ignore();
    //Check the model.
    if(model()==nullptr)
    {
        //Ignore the drag event when the music model is null.
        return;
    }
    //Check mime type.
    const QMimeData *dragMimeData=event->mimeData();
    if(dragMimeData->hasUrls() ||
            dragMimeData->hasFormat(ModelMimeType))
    {
        event->accept();
    }
    //Check the event state.
    if(!event->isAccepted())
    {
        //Ignore the cannot accepted event.
        return;
    }

    //Get the current position index.
    QModelIndex dropIndex=indexAt(event->pos());
    //Check whether the index is valid or not.
    if(dropIndex.isValid())
    {
        //Set the row to the delegate.
        KNMusicPlaylistListDelegate::setHoverRow(dropIndex.row());
        //Get the drop position.
        QAbstractItemView::DropIndicatorPosition dropIndicatorPosition=
                dropPosition(event->pos(), visualRect(dropIndex));
        //Calculate the drop rect according to the drop indicator position.
        switch(dropIndicatorPosition)
        {
        case QAbstractItemView::OnItem:
            KNMusicPlaylistListDelegate::setIndicator(
                        KNMusicPlaylistListDelegate::OnItem);
            break;
        case QAbstractItemView::AboveItem:
            KNMusicPlaylistListDelegate::setIndicator(
                        KNMusicPlaylistListDelegate::AboveItem);
            break;
        case QAbstractItemView::BelowItem:
            KNMusicPlaylistListDelegate::setIndicator(
                        KNMusicPlaylistListDelegate::BelowItem);
            break;
        default:
            //It shouldn't come to here, we write this is to avoid the
            //enumeration not handled in switch warning.
            break;
        }
    }
    else
    {
        //Set it to the end of the whole model.
        //Get the last index rect.
        //Set the row to the delegate.
        KNMusicPlaylistListDelegate::setHoverRow(model()->rowCount()-1);
        //Update the indicator.
        KNMusicPlaylistListDelegate::setIndicator(
                    KNMusicPlaylistListDelegate::BelowItem);
    }
    //Update the widget.
    viewport()->update();
}

void KNMusicPlaylistListView::dragLeaveEvent(QDragLeaveEvent *)
{
    //Clear the state.
    setState(NoState);
    //Stop auto scroll.
    stopAutoScroll();
    //Clear the dragging hover rect.
    //Reset the row to the delegate.
    KNMusicPlaylistListDelegate::setHoverRow(-1);
    //Update the indicator.
    KNMusicPlaylistListDelegate::setIndicator(
                KNMusicPlaylistListDelegate::OutOfItem);
    //Update the viewport.
    viewport()->update();
}

QAbstractItemView::DropIndicatorPosition KNMusicPlaylistListView::dropPosition(
        const QPoint &pos,
        const QRect &rect) const
{
    //Check the pos to the rect.
    if(pos.y() - rect.top() < m_dropSideMargin)
    {
        //At the above part.
        return QAbstractItemView::AboveItem;
    }
    if(rect.bottom() - pos.y() < m_dropSideMargin)
    {
        //At the bottom part.
        return QAbstractItemView::BelowItem;
    }
    //It should be right on the item.
    return QAbstractItemView::OnItem;
}

inline bool KNMusicPlaylistListView::dropOn(QDropEvent *event,
                                            int &dropRow,
                                            QModelIndex &dropIndex)
{
    //Check the event.
    if(event->isAccepted())
    {
        //Actually I don't know why we have to do it.
        //If you understand this, or you are a developer of Qt, please mail me
        //at tomguts@126.com to tell me why it works like this.
        return false;
    }

    //Check the position of the event.
    if(!viewport()->rect().contains(event->pos()))
    {
        //For the position out of the rect, return false.
        return false;
    }

    //Get the index at the Y position of the pos.
    dropIndex=indexAt(QPoint(0, event->pos().y()));
    //Check the drop index.
    if(dropIndex.isValid())
    {
        //Get the indicator position.
        QAbstractItemView::DropIndicatorPosition indicatorPosition=
                dropPosition(event->pos(), visualRect(dropIndex));
        //Check drop on the top or the bottom part of the index.
        switch (indicatorPosition)
        {
        case QAbstractItemView::AboveItem:
            //Set the row.
            dropRow=dropIndex.row();
            //Clear up the model index.
            dropIndex=QModelIndex();
            break;
        case QAbstractItemView::OnItem:
            //Reset the row.
            dropRow=-1;
            break;
        case QAbstractItemView::BelowItem:
            //Set the row.
            dropRow=dropIndex.row()+1;
            //Clear up the model index.
            dropIndex=QModelIndex();
            break;
        default:
            break;
        }
    }
    else
    {
        //Check the model is null or not.
        if(model()==nullptr)
        {
            dropRow=-1;
        }
        else
        {
            //Treat the bottom area as the last row.
            dropRow=model()->rowCount();
        }
    }
    return true;
}

void KNMusicPlaylistListView::dropEvent(QDropEvent *event)
{
    //Clear the state.
    setState(NoState);
    //Check the model.
    if(model()==nullptr)
    {
        //Ignore the drop event when the music model is null.
        event->ignore();
        return;
    }
    //Clear the dragging hover rect.
    //Reset the row to the delegate.
    KNMusicPlaylistListDelegate::setHoverRow(-1);
    //Update the indicator.
    KNMusicPlaylistListDelegate::setIndicator(
                KNMusicPlaylistListDelegate::OutOfItem);
    //Drop the data.
    int row=-1;
    QModelIndex droppedIndex;
    //Get the drop row.
    if(dropOn(event, row, droppedIndex))
    {
        //Check the music model, and do the drop mime data to the proxy model.
        if(model()!=nullptr &&
                model()->dropMimeData(event->mimeData(),
                                      Qt::CopyAction,
                                      row,
                                      Name,
                                      droppedIndex))
        {
            //Accept the drop event.
            event->accept();
        }
    }
    //Stop auto scroll.
    stopAutoScroll();
    //Set state.
    setState(NoState);
    //Update view port.
    viewport()->update();
}

void KNMusicPlaylistListView::showEvent(QShowEvent *event)
{
    //Emit initial signal.
    emit requireInitialPlaylist();
    //Show the widget.
    KNMusicCategoryListViewBase::showEvent(event);
}
