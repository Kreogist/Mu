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
#include "knmusicplaylistnotifications.h"

KNMusicPlaylistFailedLoaded::KNMusicPlaylistFailedLoaded() :
    KNNotificationData(parent),
    m_failedCounter(0)
{
}

QString KNMusicPlaylistFailedLoaded::title() const
{
    //Return the failed loaded data.
    return QObject::tr("%1 playlists cannot be loaded.").arg(
                QString::number(failedCounter));
}

QString KNMusicPlaylistFailedLoaded::content() const
{
    //Return the failed loaded content.
    return QObject::tr("Those playlists may be moved, deleted or renamed.");
}

void KNMusicPlaylistFailedLoaded::setFailedCounter(int failedCounter)
{
    //Set the failed counter data.
    m_failedCounter=failedCounter;
}
