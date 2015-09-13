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

#include "knmusicplaylistlistdelegate.h"
#include "knmusicmodel.h"

#include "knmusicplaylistlistview.h"

#include <QDebug>

#define DropSideMargin 5

KNMusicPlaylistListView::KNMusicPlaylistListView(QWidget *parent) :
    KNMusicCategoryListViewBase(parent)
{
    //Enabled the drag and drop function.
    enabledDragDrop();
    //Set properties.
    setIconSize(QSize(40, 40));
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
    if(pos.y() - rect.top() < DropSideMargin)
    {
        //At the above part.
        return QAbstractItemView::AboveItem;
    }
    if(rect.bottom() - pos.y() < DropSideMargin)
    {
        //At the bottom part.
        return QAbstractItemView::BelowItem;
    }
    //It should be right on the item.
    return QAbstractItemView::OnItem;
}

void KNMusicPlaylistListView::dropEvent(QDropEvent *event)
{
    //Clear the state.
    setState(NoState);
    //Clear the dragging hover rect.
    //Reset the row to the delegate.
    KNMusicPlaylistListDelegate::setHoverRow(-1);
    //Update the indicator.
    KNMusicPlaylistListDelegate::setIndicator(
                KNMusicPlaylistListDelegate::OutOfItem);
    //Do the original drop event.
    KNMusicCategoryListViewBase::dropEvent(event);
}
