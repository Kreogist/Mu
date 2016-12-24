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

#include "knmusiclrcparser.h"

#include "knmusicstorebackendmanager.h"
#include "knmusicstorealbummodel.h"

#include "knmusicstoreglobal.h"

#include <QDebug>

KNMusicStoreGlobal *KNMusicStoreGlobal::m_instance=nullptr;

KNMusicStoreGlobal::~KNMusicStoreGlobal()
{
    //Check the widget parent.
    if(m_connectStateWheel->parent()==nullptr)
    {
        //Remove the widget when no one manage it.
        m_connectStateWheel->deleteLater();
    }
    //Quit the working thread.
    m_backendThread.quit();
    m_backendThread.wait();
    //Remove the backend manager.
    knMusicStoreBackendManager->deleteLater();
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
    m_connectSemaphore(0),
    m_connectStateWheel(new KNDarkWaitingWheel()),
    m_albumModel(new KNMusicStoreAlbumModel(this)),
    m_lrcParser(new KNMusicLrcParser(this))
{
    //Initial the backend manager.
    KNMusicStoreBackendManager::initial(&m_backendThread);
    //Link the backend manager.
    connect(knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::requireAddConnectionCount,
            this, &KNMusicStoreGlobal::addConnectionCounter,
            Qt::QueuedConnection);
    connect(knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::requireReduceConnectionCount,
            this, &KNMusicStoreGlobal::reduceConnectionCounter,
            Qt::QueuedConnection);
    connect(knMusicStoreBackendManager,
            &KNMusicStoreBackendManager::requireResetConnectionCount,
            this, &KNMusicStoreGlobal::resetConnectionCounter,
            Qt::QueuedConnection);
    //Start the working thread.
    m_backendThread.start();

    //Update the wheel widget.
    m_connectStateWheel->hide();

    //Link the retranslator.
    knI18n->link(this, &KNMusicStoreGlobal::retranslate);
    retranslate();
}

KNMusicLrcParser *KNMusicStoreGlobal::lrcParser() const
{
    return m_lrcParser;
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
    //Lock the semaphore.
    m_connectLock.lock();
    //Set the widget to be the visit one.
    m_connectStateWheel->show();
    //Check the wheel working state.
    if(!m_connectStateWheel->isTick())
    {
        //Start ticking.
        m_connectStateWheel->startTick();
    }
    //Add number to semaphore.
    m_connectSemaphore+=counter;
    //Unlock.
    m_connectLock.unlock();
}

void KNMusicStoreGlobal::resetConnectionCounter()
{
    //Reduce the data of itself.
    reduceConnectionCounter(m_connectSemaphore);
}

void KNMusicStoreGlobal::reduceConnectionCounter(int counter)
{
    //Lock the semaphore.
    m_connectLock.lock();
    //Reduce the number.
    m_connectSemaphore-=counter;
    //Check the value.
    if(m_connectSemaphore<=0)
    {
        //Reset to 0, minus could be a bug.
        m_connectSemaphore=0;
        //Hide the wheel.
        m_connectStateWheel->hide();
        //Stop ticking.
        m_connectStateWheel->stopTick();
    }
    //Release the lock.
    m_connectLock.unlock();
}
