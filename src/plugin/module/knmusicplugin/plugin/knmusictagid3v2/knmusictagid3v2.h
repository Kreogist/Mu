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
enum ID3v2TextEncoding
{
    EncodeISO,
    EncodeUTF16BELE,
    EncodeUTF16,
    EncodeUTF8
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
struct ID3v2PictureFrame
{
    QString mimeType;
    QString description;
    QImage image;
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
                  KNMusicAnalysisItem &analysisItem);
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem);
    QString frameToText(QByteArray content);
    bool usingDefaultCodec() const;
    void setUsingDefaultCodec(bool usingDefaultCodec);

signals:

public slots:

protected:
    inline bool parseID3v2Header(char *rawHeader,
                                 ID3v2Header &header)
    {
        //Check 'ID3' from the very beginning.
        if(rawHeader[0]!='I' || rawHeader[1]!='D' || rawHeader[2]!='3')
        {
            return false;
        }
        //Get the version of the tag.
        header.major=(quint8)rawHeader[3];
        header.minor=(quint8)rawHeader[3];
        //Get the flag.
        header.flag=(quint8)rawHeader[5];
        //Calculate tag size.
        header.size=(((quint32)rawHeader[6]<<21)&0b00001111111000000000000000000000)+
                (((quint32)rawHeader[7]<<14)&0b00000000000111111100000000000000)+
                (((quint32)rawHeader[8]<<7) &0b00000000000000000011111110000000)+
                ( (quint32)rawHeader[9]     &0b00000000000000000000000001111111);
        return true;
    }
    inline void generateID3v2Property(const quint8 &minor,
                                      ID3v2MinorProperty &property)
    {
        //Because the ID3v2 has so many version, we have to use different calculate
        //function to process these frames.
        switch(minor)
        {
        case 0:
        case 1:
        case 2:
            property.frameIDSize=3;
            property.frameHeaderSize=6;
            property.toSize=KNMusicTagID3v2::minor2Size;
            property.saveFlag=nullptr;
            break;
        case 3:
            property.frameIDSize=4;
            property.frameHeaderSize=10;
            property.toSize=KNMusicTagID3v2::minor3Size;
            property.saveFlag=KNMusicTagID3v2::saveFlag;
            break;
        case 4:
            property.frameIDSize=4;
            property.frameHeaderSize=10;
            property.toSize=KNMusicTagID3v2::minor4Size;
            property.saveFlag=KNMusicTagID3v2::saveFlag;
            break;
        default:
            break;
        }
    }
    bool parseID3v2RawData(char *rawTagData,
                           const ID3v2Header &header,
                           const ID3v2MinorProperty &property,
                           QLinkedList<ID3v2Frame> &frameList);
    void writeID3v2ToDetails(const QLinkedList<ID3v2Frame> &frames,
                             const ID3v2MinorProperty &property,
                             KNMusicAnalysisItem &analysisItem);

private:
    static inline int ratingStars(const quint8 &hex)
    {
        //1-31  = 1 star.
        //32-95 = 2 stars.
        //96-159 = 3 stars.
        //160-223 = 4 stars.
        //224-255 = 5 stars.
        if(hex>0 && hex<32)
        {
            return 1;
        }
        if(hex>31 && hex<96)
        {
            return 2;
        }
        if(hex>95 && hex<160)
        {
            return 3;
        }
        if(hex>159 && hex<224)
        {
            return 4;
        }
        if(hex>223)
        {
            return 5;
        }
        return 0;
    }
    static inline quint32 minor2Size(char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<16)&0b00000000111111110000000000000000)+
               (((quint32)rawTagData[1]<<8) &0b00000000000000001111111100000000)+
               ( (quint32)rawTagData[2]     &0b00000000000000000000000011111111);
    }

    static inline quint32 minor3Size(char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<24)&0b11111111000000000000000000000000)+
               (((quint32)rawTagData[1]<<16)&0b00000000111111110000000000000000)+
               (((quint32)rawTagData[2]<<8 )&0b00000000000000001111111100000000)+
               ( (quint32)rawTagData[3]     &0b00000000000000000000000011111111);
    }

    static inline quint32 minor4Size(char *rawTagData)
    {
        return (((quint32)rawTagData[0]<<21)&0b00001111111000000000000000000000)+
               (((quint32)rawTagData[1]<<14)&0b00000000000111111100000000000000)+
               (((quint32)rawTagData[2]<<7 )&0b00000000000000000011111110000000)+
               ( (quint32)rawTagData[3]     &0b00000000000000000000000001111111);
    }

    static inline void saveFlag(char *rawTagData, ID3v2Frame &frameData)
    {
        frameData.flags[0]=rawTagData[8];
        frameData.flags[1]=rawTagData[9];
    }

    inline void parseAPICImageData(QByteArray imageData,
                                   QHash<int, ID3v2PictureFrame> &imageMap);
    inline void parsePICImageData(QByteArray imageData,
                                  QHash<int, ID3v2PictureFrame> &imageMap);
    QHash<QString, int> m_frameIDIndex;
    KNMusicGlobal *m_musicGlobal;

    QByteArray m_unsynchronisationRaw, m_unsynchronisationTo;
    QTextCodec *m_isoCodec,
               *m_utf16BECodec,
               *m_utf16LECodec,
               *m_utf16Codec,
               *m_utf8Codec,
               *m_localeCodec;
    bool m_usingDefaultCodec=true;
};

#endif // KNMUSICTAGID3V2_H
