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
#include <QThread>

#include "knmusicbackendqtavthread.h"

#include "knmusicbackendqtav.h"

KNMusicBackendQtAV::KNMusicBackendQtAV(QObject *parent) :
    KNMusicStandardBackend(parent)
{
    //Initial the QThread.
    m_mainThread=new QThread(this);
    m_previewThread=new QThread(this);

    //Initial the main and preview thread.
    m_main=new KNMusicBackendQtAVThread;
    m_main->moveToThread(m_mainThread);
    setMainThread(m_main);
    m_preview=new KNMusicBackendQtAVThread;
    m_preview->moveToThread(m_previewThread);
    setPreviewThread(m_preview);

    //Start the thread.
    m_mainThread->start();
    m_previewThread->start();
}

KNMusicBackendQtAV::~KNMusicBackendQtAV()
{
    //Quit the thread.
    m_mainThread->quit();
    m_previewThread->quit();

    //Wait the thread.
    m_mainThread->wait();
    m_previewThread->wait();

    //Delete the main and preview.
    m_main->deleteLater();
    m_preview->deleteLater();
}

bool KNMusicBackendQtAV::available()
{
    //THis is no meaning for QtAV.
    return true;
}

int KNMusicBackendQtAV::volume() const
{
    return m_main->volume();
}

void KNMusicBackendQtAV::loadUrl(const QString &url)
{
    ;
}

int KNMusicBackendQtAV::volumeMinimal()
{
    return 0;
}

int KNMusicBackendQtAV::volumeMaximum()
{
    return 10000;
}

void KNMusicBackendQtAV::changeVolume(const int &volumeSize)
{
    m_main->setVolume(volumeSize);
}
