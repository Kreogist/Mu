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
#include "knmusicbackendmpvthread.h"

#include "knmusicbackendmpv.h"

KNMusicBackendMpv::KNMusicBackendMpv(QObject *parent) :
    KNMusicStandardBackend(parent),
    m_main(nullptr),
    m_preview(nullptr)
{
    //Initial the main and preview of the threads.
    m_main=new KNMusicBackendMpvThread;
    m_preview=new KNMusicBackendMpvThread;
    //Set the main and preview threads.
    setMainThread(m_main);
    setPreviewThread(m_preview);
    //Link the main thread success signal to backend signal.
    connect(m_main, &KNMusicBackendMpvThread::loadSuccess,
            this, &KNMusicBackendMpv::loadSuccess);
}

KNMusicBackendMpv::~KNMusicBackendMpv()
{
    m_main->deleteLater();
    m_preview->deleteLater();
}


bool KNMusicBackendMpv::loadMusic(const QString &filePath,
                                  const qint64 &start,
                                  const qint64 &duration)
{
    //Check thread first, if thread exist, load the music.
    if(!m_main || !m_main->loadFile(filePath))
    {
        //If there's no thread or failed to load, load music will always be
        //false.
        return false;
    }
    //Set the section for thread.
    m_main->setPlaySection(start, duration);
    //Load the music compelte.
    return true;
}

int KNMusicBackendMpv::volume() const
{
    return m_main->volume();
}

int KNMusicBackendMpv::minimalVolume() const
{
    return 0;
}

int KNMusicBackendMpv::maximumVolume() const
{
    return 10000;
}

void KNMusicBackendMpv::setGlobalVolume(const int &volume)
{
    //Set to the main thread.
    m_main->setVolume(volume);
    //Emit the volume changed signal.
    emit volumeChanged(volume);
}

int KNMusicBackendMpv::volumeLevel() const
{
    return 1000;
}

qreal KNMusicBackendMpv::smartVolumeScale() const
{
    return 0.1;
}
