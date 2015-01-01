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
#ifndef KNMUSICTAGAPEV2_H
#define KNMUSICTAGAPEV2_H

#include <QList>

#include "knmusictagpraser.h"

namespace KNMusicAPEv2
{
struct APEHeader
{
    quint32 version;
    quint32 size;
    quint32 itemCount;
    quint32 flags;
};
struct APETagItem
{
    QString key;
    quint32 flag;
    QByteArray value;
};
}

using namespace KNMusicAPEv2;

class KNMusicTagAPEv2 : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagAPEv2(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem);
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem);

signals:

public slots:

private:
    bool checkHeader(const int &position,
                     QDataStream &musicDataStream,
                     APEHeader &header);
    void parseRawData(char *rawData,
                      APEHeader &header,
                      QList<APETagItem> &tagList);
    void writeTagListToAnalysisItem(const QList<APETagItem> &tagList,
                                  KNMusicAnalysisItem &analysisItem);
    const char m_apePreamble[8]={'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X'};
    QHash<QString, int> m_keyIndex;
};

#endif // KNMUSICTAGAPEV2_H
