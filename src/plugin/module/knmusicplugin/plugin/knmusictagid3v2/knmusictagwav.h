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
#ifndef KNMUSICTAGWAV_H
#define KNMUSICTAGWAV_H

#include "knmusictagid3v2.h"

namespace KNMusicWAV
{
struct WAVItem
{
    QString key;
    QString value;
};
}

using namespace KNMusicWAV;

class KNMusicTagWAV : public KNMusicTagID3v2
{
    Q_OBJECT
public:
    explicit KNMusicTagWAV(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicDetailInfo &detailInfo);
    bool parseAlbumArt(KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    inline void parseListChunk(char *rawData,
                               quint32 dataSize,
                               QList<WAVItem> &listData);
    inline void writeListDataToDetailInfo(const QList<WAVItem> &listData,
                                          KNMusicDetailInfo &detailInfo);
    const char m_riffHeader[4]={'R', 'I', 'F', 'F'},
               m_waveHeader[4]={'W', 'A', 'V', 'E'},
               m_listChunk[4]={'L', 'I', 'S', 'T'},
               m_listInfoHeader[4]={'I', 'N', 'F', 'O'},
               m_id32Chunk[4]={'i', 'd', '3', 0x20};
    QHash<QString, int> m_listKeyIndex;
};

#endif // KNMUSICTAGWAV_H
