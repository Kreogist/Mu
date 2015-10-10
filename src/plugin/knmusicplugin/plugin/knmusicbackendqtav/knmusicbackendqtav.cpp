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

#include "knmusicbackendqtavthread.h"

#include "knmusicbackendqtav.h"

KNMusicBackendQtAV::KNMusicBackendQtAV(QObject *parent) :
    KNMusicStandardBackend(parent),
    m_main(new KNMusicBackendQtAVThread(this)),
    m_preview(new KNMusicBackendQtAVThread(this))
{
    setMainThread(m_main);
    setPreviewThread(m_preview);
}

int KNMusicBackendQtAV::volume() const
{
    return m_main->volume();
}

int KNMusicBackendQtAV::minimalVolume() const
{
    return 0;
}

int KNMusicBackendQtAV::maximumVolume() const
{
    return 10000;
}

void KNMusicBackendQtAV::setGlobalVolume(const int &volume)
{
    //Set the volume to main thread.
    m_main->setVolume(volume);
}

int KNMusicBackendQtAV::volumeLevel() const
{
    return 100;
}

qreal KNMusicBackendQtAV::smartVolumeScale() const
{
    return 0.5;
}

