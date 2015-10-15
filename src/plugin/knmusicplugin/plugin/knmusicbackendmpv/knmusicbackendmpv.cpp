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
#include <mpv/client.h>

#include "knmusicbackendmpvthread.h"

#include "knmusicbackendmpv.h"

KNMusicBackendMpv::KNMusicBackendMpv(QObject *parent) :
    KNMusicStandardBackend(parent),
    m_main(new KNMusicBackendMpvThread()),
    m_preview(new KNMusicBackendMpvThread())
{
    //Move to work thread.
    m_main->moveToThread(&m_mainThread);
    m_preview->moveToThread(&m_previewThread);
    //Set threads.
    setMainThread(m_main);
    setPreviewThread(m_preview);
    //Start thread.
    m_mainThread.start();
    m_previewThread.start();
}

KNMusicBackendMpv::~KNMusicBackendMpv()
{
    //Quit the working threads.
    m_mainThread.quit();
    m_previewThread.quit();
    //Wait for quitting.
    m_mainThread.wait();
    m_previewThread.wait();
    //Recover memory.
    m_main->deleteLater();
    m_preview->deleteLater();
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
    return 100;
}

void KNMusicBackendMpv::setGlobalVolume(const int &volume)
{
    //Set the volume size to main thread.
    m_main->setVolume(volume);
    //Emit the volume changed signal.
    emit volumeChanged(m_main->volume());
}

int KNMusicBackendMpv::volumeLevel() const
{
    return 10;
}

qreal KNMusicBackendMpv::smartVolumeScale() const
{
    return 0.5;
}
