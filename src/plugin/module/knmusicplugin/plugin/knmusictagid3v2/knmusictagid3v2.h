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
#ifndef KNMUSICTAGID3V2_H
#define KNMUSICTAGID3V2_H

#include <QLinkedList>
#include <QHash>

#include "knmusictagpraser.h"

namespace KNMusicID3v2
{
enum ID3v2HeaderFlag
{
    Unsynchronisation=0b10000000,
    ExtendedHeader=0b01000000,
    ExperimentalIndicator=0b00100000
};
enum ID3v2FrameFlag
{
    FrameDataLengthIndicator=0b00000001,
    FrameUnsynchronisation=0b00000010
};

struct ID3v2Header
{
    quint8 major=0;
    quint8 minor=0;
    quint32 size=0;
    quint8 flag=0;
};
struct ID3v2Frame
{
    char frameID[5]={0};
    char *start;
    quint32 size=0;
    char flags[2]={0};
};
typedef quint32 (*FrameSizeCalculator)(char *);
typedef void (*FlagSaver)(char *, ID3v2Frame &);
struct ID3v2MinorProperty
{
    int frameIDSize;
    int frameHeaderSize;
    FrameSizeCalculator toSize=nullptr;
    FlagSaver saveFlag=nullptr;
};
}

using namespace KNMusicID3v2;

class QTextCodec;
class KNMusicTagID3v2 : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagID3v2(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicDetailInfo &detailInfo);
    bool parseAlbumArt(KNMusicDetailInfo &detailInfo);
    static quint32 minor2Size(char *rawTagData);
    static quint32 minor3Size(char *rawTagData);
    static quint32 minor4Size(char *rawTagData);
    static void saveFlag(char *rawTagData, ID3v2Frame &frameData);
    QString frameToText(QByteArray content);

signals:

public slots:

private:
    bool parseHeader(char *rawHeader,
                     ID3v2Header &header);
    bool parseRawData(char *rawTagData,
                      const ID3v2Header &header,
                      const ID3v2MinorProperty &property,
                      QLinkedList<ID3v2Frame> &frameList);
    void generateProperty(const quint8 &minor,
                          ID3v2MinorProperty &property);
    void writeFramesToDetails(const QLinkedList<ID3v2Frame> &frames,
                              const ID3v2MinorProperty &property,
                              KNMusicDetailInfo &detailInfo);
    QHash<QString, int> m_frameIDIndex;
    KNMusicGlobal *m_musicGlobal;

    QTextCodec *m_isoCodec,
               *m_utf16BECodec,
               *m_utf16LECodec,
               *m_utf16Codec,
               *m_utf8Codec,
               *m_localeCodec;
};

#endif // KNMUSICTAGID3V2_H
