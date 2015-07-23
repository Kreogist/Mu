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

#include "knmusicplaylistmodel.h"

KNMusicPlaylistModel::KNMusicPlaylistModel(QObject *parent) :
    KNMusicModel(parent),
    m_title(QString()),
    m_filePath(QString()),
    m_built(false)
{

}

QString KNMusicPlaylistModel::title() const
{
    return m_title;
}

void KNMusicPlaylistModel::setTitle(const QString &title)
{
    m_title = title;
}

QString KNMusicPlaylistModel::filePath() const
{
    return m_filePath;
}

void KNMusicPlaylistModel::setFilePath(const QString &filePath)
{
    m_filePath = filePath;
}

bool KNMusicPlaylistModel::built() const
{
    return m_built;
}

void KNMusicPlaylistModel::buildModel()
{
    //Check if we have been build this model before.
    if(m_built)
    {
        return;
    }
    //Set the build flag.
    m_built=true;
}
