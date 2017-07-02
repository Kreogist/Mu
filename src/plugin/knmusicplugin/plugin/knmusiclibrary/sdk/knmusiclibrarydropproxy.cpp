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
#include <QDropEvent>
#include <QMimeData>

#include "knmusicmodel.h"

#include "knmusiclibrarydropproxy.h"

KNMusicLibraryDropProxy::KNMusicLibraryDropProxy(QWidget *parent) :
    KNDropProxyContainer(parent)
{
}

void KNMusicLibraryDropProxy::dropEvent(QDropEvent *event)
{
    //Get the mime data pointer from the event.
    const QMimeData *mimeData=event->mimeData();
    //Check the event data.
    if(mimeData->hasFormat(ModelMimeType))
    {
        //Check the pointer.
        if(mimeData->data(ModelMimeType).toLongLong()==m_libraryModelPointer)
        {
            //Ignore the drop event.
            event->ignore();
            return;
        }
    }
    //Do original check.
    KNDropProxyContainer::dropEvent(event);
}

void KNMusicLibraryDropProxy::setLibraryModelPointer(const qint64 &model)
{
    //Save the model pointer.
    m_libraryModelPointer = model;
}
