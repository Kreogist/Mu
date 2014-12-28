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
#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include <QHash>

#include "knmusictagpraser.h"

namespace KNMusicWMA
{
enum StandardFrameItems
{
    ItemTitle,
    ItemAuthor,
    ItemCopyright,
    ItemDescription,
    ItemRating,
    StandardFrameItemsCount
};
struct KNMusicWMAFrame
{
    QString name;
    QByteArray data;
};
struct WMAPicture
{
    QString mimeType;
    QString description;
    QImage image;
};
}

using namespace KNMusicWMA;

class KNMusicTagWMA : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagWMA(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem);
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem);

signals:

public slots:

private:
    QString frameToText(QByteArray content);
    inline bool isStandardFrame(char *frame)
    {
        return !memcmp(frame, m_standardFrame, 16);
    }

    inline bool isExtendFrame(char *frame)
    {
        return !memcmp(frame, m_extendedFrame, 16);
    }

    bool parseStandardFrame(char *frameStart,
                            quint64 frameSize,
                            QList<KNMusicWMAFrame> &frameList);
    bool parseExtendFrame(char *frameStart,
                          quint64 frameSize,
                          QList<KNMusicWMAFrame> &frameList);
    bool parseImageData(QByteArray imageData, WMAPicture &albumArt);
    inline void writeTagMapToDetailInfo(const QList<KNMusicWMAFrame> &frameList,
                                        KNMusicAnalysisItem &analysisItem);
    unsigned char m_headerMark[17]={0x30, 0x26, 0xB2, 0x75,
                                    0x8E, 0x66, 0xCF, 0x11,
                                    0xA6, 0xD9, 0x00, 0xAA,
                                    0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_standardFrame[17]={0x33, 0x26, 0xB2, 0x75,
                                       0x8E, 0x66, 0xCF, 0x11,
                                       0xA6, 0xD9, 0x00, 0xAA,
                                       0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_extendedFrame[17]={0x40, 0xA4, 0xD0, 0xD2,
                                       0x07, 0xE3, 0xD2, 0x11,
                                       0x97, 0xF0, 0x00, 0xA0,
                                       0xC9, 0x5E, 0xA8, 0x50};

    QString m_standardFrameID[StandardFrameItemsCount];
    QHash<QString, int> m_attributesIndex;
    QTextCodec *m_isoCodec,
               *m_utf16BECodec,
               *m_utf16LECodec,
               *m_utf16Codec,
               *m_utf8Codec,
               *m_localeCodec;
};

#endif // KNMUSICTAGWMA_H
