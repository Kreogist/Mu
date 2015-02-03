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
#include "knglobal.h"

#include "knmusicplaylistmodel.h"

KNMusicPlaylistModel::KNMusicPlaylistModel(QObject *parent) :
    KNMusicModel(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Connect language changed request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicPlaylistModel::retranslate);
    //Initial header.
    initialHeader();
}

Qt::ItemFlags KNMusicPlaylistModel::flags(const QModelIndex &index) const
{
    return (index.column()==BlankData)?
                (Qt::ItemIsSelectable |
                 Qt::ItemIsDragEnabled |
                 Qt::ItemIsEnabled |
                 Qt::ItemNeverHasChildren):
                KNMusicModel::flags(index);
}

int KNMusicPlaylistModel::playingItemColumn()
{
    return BlankData;
}

void KNMusicPlaylistModel::retranslate()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<MusicDisplayDataCount;i++)
    {
        header<<(m_musicGlobal->treeViewHeaderText(i));
    }
    setHorizontalHeaderLabels(header);
}

void KNMusicPlaylistModel::initialHeader()
{
    //Using retranslate to update the header text.
    retranslate();
    //Set header size hint.
    setHeaderData(0, Qt::Horizontal, QSize(10,23), Qt::SizeHintRole);
    //Set header alignment
    for(int i=0;i<MusicDataCount;i++)
    {
        setHeaderData(i, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
    }
    //Set sort flag.
    setHeaderSortFlag();
}
