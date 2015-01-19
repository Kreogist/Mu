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
#include "knmusicvlcglobal.h"
#include "knmusicbackendvlcthread.h"

#include "knmusicbackendvlc.h"

KNMusicBackendVLC::KNMusicBackendVLC(QObject *parent) :
    KNMusicStandardBackend(parent)
{
    //Initial VLC first. But VLC should only initial once.
    //We are going to initial the vlc at the time initial the VLC global.
    m_vlcGlobal=KNMusicVLCGlobal::instance();
    //Initial the main and preview thread.
    m_main=new KNMusicBackendVLCThread(this);
    setMainThread(m_main);

    m_preview=new KNMusicBackendVLCThread(this);
    setPreviewThread(m_preview);
}

KNMusicBackendVLC::~KNMusicBackendVLC()
{
    //Release the global.
    delete m_vlcGlobal;
}

bool KNMusicBackendVLC::available()
{
    //!FIXME: I forget why there's a function named this...
    return true;
}

int KNMusicBackendVLC::volume() const
{
    return m_main->volume();
}

void KNMusicBackendVLC::loadUrl(const QString &url)
{
    ;
}

int KNMusicBackendVLC::volumeMinimal()
{
    return 0;
}

int KNMusicBackendVLC::volumeMaximum()
{
    return 100;
}

void KNMusicBackendVLC::changeVolume(const int &volumeSize)
{
    m_main->setVolume(volumeSize);
}

qreal KNMusicBackendVLC::smartVolumeScale() const
{
    return 0.5;
}
