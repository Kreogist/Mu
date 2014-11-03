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
#ifndef KNMUSICTAGID3V1_H
#define KNMUSICTAGID3V1_H

#include "knmusictagpraser.h"

namespace KNMusicID3v1
{
struct ID3v1Struct
{
    QString tags[5];
    int track=-1;
    int genreIndex=-1;
};
}

using namespace KNMusicID3v1;

class QTextCodec;
class KNMusicTagID3v1 : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagID3v1(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicDetailInfo &detailInfo);
    bool parseAlbumArt(KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    void parseRawData(char *rawTagData, ID3v1Struct &tagData);
    void writeTagDataToDetailInfo(const ID3v1Struct &tagData,
                                  KNMusicDetailInfo &detailInfo);
    void backupByte(char *rawTagData,
                    const int &backupPosition,
                    char &backupPool,
                    int &positionPool,
                    const bool &clearData=false);
    QTextCodec *m_defaultCodec;
};

#endif // KNMUSICTAGID3V1_H
