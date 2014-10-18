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
#include "knmusicsingleplaylistmodel.h"

KNMusicSinglePlaylistModel::KNMusicSinglePlaylistModel(QObject *parent) :
    KNMusicModel(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Initial header.
    initialHeader();
}

void KNMusicSinglePlaylistModel::initialHeader()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<MusicDisplayDataCount;i++)
    {
        header<<(m_musicGlobal->treeViewHeaderText(i));
    }
    setHorizontalHeaderLabels(header);
    //Set header size hint.
    setHeaderData(0, Qt::Horizontal, QSize(10,23), Qt::SizeHintRole);
    //Set header alignment
    for(int i=0;i<MusicDataCount;i++)
    {
        setHeaderData(i, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
    }
    //Set special header data, e.g.: Sort flag.
    setHeaderData(Time, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(Size, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(TrackNumber, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(Time, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(DiscNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(DiscCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(TrackNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(TrackCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(Size, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(BitRate, Qt::Horizontal, 3, Qt::UserRole);
    setHeaderData(DateAdded, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(DateModified, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(LastPlayed, Qt::Horizontal, 4, Qt::UserRole);
}
