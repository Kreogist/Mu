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
#include <QEventLoop>

#include "knmusicbackendphononthread.h"

#include "knmusicbackendphonon.h"

KNMusicBackendPhonon::KNMusicBackendPhonon(QObject *parent) :
    KNMusicStandardBackend(parent)
{
    m_main=new KNMusicBackendPhononThread(this);
    setMainThread(m_main);
    m_preview=new KNMusicBackendPhononThread(this);
    setPreviewThread(m_preview);
}

KNMusicBackendPhonon::~KNMusicBackendPhonon()
{

}

bool KNMusicBackendPhonon::available()
{
    return true;
}

bool KNMusicBackendPhonon::loadMusic(const QString &filePath)
{
    //Load the file on main thread.
    m_main->loadFromFile(filePath);
    return true;
}

int KNMusicBackendPhonon::volume() const
{
    return m_main->volume();
}

void KNMusicBackendPhonon::loadUrl(const QString &url)
{
    ;
}

int KNMusicBackendPhonon::volumeMinimal()
{
    return 0;
}

int KNMusicBackendPhonon::volumeMaximum()
{
    return 10000;
}

void KNMusicBackendPhonon::changeVolume(const int &volumeSize)
{
    m_main->setVolume(volumeSize);
}

qreal KNMusicBackendPhonon::smartVolumeScale() const
{
    return 0.5;
}
