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
#include <QDropEvent>
#include <QMimeData>

#include "kndropproxycontainer.h"

KNDropProxyContainer::KNDropProxyContainer(QWidget *parent) :
    QWidget(parent)
{
    //Enabled drops
    setAcceptDrops(true);
}

void KNDropProxyContainer::dragEnterEvent(QDragEnterEvent *event)
{
    //Check the event, if event contains urls, emit a drag enter signal.
    if(event->mimeData()->hasUrls())
    {
        //Accept the event.
        event->acceptProposedAction();
        //Emit the drag enter event.
        emit dragEntered();
    }
}

void KNDropProxyContainer::dropEvent(QDropEvent *event)
{
    //Emit dropped signal.
    emit dropped();
    //Check the mime data event.
    if(event->mimeData()->hasUrls())
    {
        //Emit the urls dropped signal.
        emit urlsDropped(event->mimeData()->urls());
    }
}

void KNDropProxyContainer::showEvent(QShowEvent *event)
{
    //Before show the container, emit the about to show signal.
    emit aboutToShow();
    //Show the widget.
    QWidget::showEvent(event);
}

void KNDropProxyContainer::hideEvent(QHideEvent *event)
{
    //Before hide the container, emit the about to hide signal.
    emit aboutToHide();
    //Hide the widget.
    QWidget::hideEvent(event);
}
