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
    m_main(new KNMusicBackendMpvThread(this)),
    m_preview(new KNMusicBackendMpvThread(this))
{
    //Set threads.
    setMainThread(m_main);
    setPreviewThread(m_preview);
}

int KNMusicBackendMpv::volume() const
{
    ;
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
    ;
}

int KNMusicBackendMpv::volumeLevel() const
{
    return 10;
}

qreal KNMusicBackendMpv::smartVolumeScale() const
{
    return 0.5;
}
