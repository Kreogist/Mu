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
#include "knlocalemanager.h"
#include "kndarkwaitingwheel.h"

#include "knmusicstorebackendmanager.h"
#include "knmusicstorealbummodel.h"

#include "knmusicstoreglobal.h"

KNMusicStoreGlobal *KNMusicStoreGlobal::m_instance=nullptr;

KNMusicStoreGlobal::~KNMusicStoreGlobal()
{
    //Check the widget parent.
    if(m_connectStateWheel->parent()==nullptr)
    {
        //Remove the widget when no one manage it.
        m_connectStateWheel->deleteLater();
    }
}

KNMusicStoreGlobal *KNMusicStoreGlobal::instance()
{
    return m_instance;
}

void KNMusicStoreGlobal::initial(QObject *parent)
{
    //Check the instance.
    if(m_instance==nullptr)
    {
        //Construct the object.
        m_instance=new KNMusicStoreGlobal(parent);
    }
}

void KNMusicStoreGlobal::retranslate()
{
    ;
}

KNMusicStoreGlobal::KNMusicStoreGlobal(QObject *parent) :
    QObject(parent),
    m_connectStateWheel(new KNDarkWaitingWheel()),
    m_albumModel(new KNMusicStoreAlbumModel(this))
{
    //Initial the backend manager.
    KNMusicStoreBackendManager::initial(this);

    //Update the wheel widget.
    m_connectStateWheel->hide();

    //Link the retranslator.
    knI18n->link(this, &KNMusicStoreGlobal::retranslate);
    retranslate();
}

KNMusicStoreAlbumModel *KNMusicStoreGlobal::albumModel() const
{
    return m_albumModel;
}

QWidget *KNMusicStoreGlobal::connectStateWheel()
{
    return static_cast<QWidget *>(m_connectStateWheel);
}

void KNMusicStoreGlobal::addConnectionCounter(int counter)
{
    //Set the widget to be the visit one.
    m_connectStateWheel->show();
    //Start ticking.
    m_connectStateWheel->startTick();
    //Add number to semaphore.
    m_connectSemaphore.release(counter);
}

void KNMusicStoreGlobal::reduceConnectionCounter(int counter)
{
    //Reduce the number.
    m_connectSemaphore.acquire(counter);
    //Check the value.
    if(m_connectSemaphore.available()==0)
    {
        //Hide the wheel.
        m_connectStateWheel->hide();
        //Stop ticking.
        m_connectStateWheel->stopTick();
    }
}
