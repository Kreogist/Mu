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
#ifndef KNMUSICTAGFLAC_H
#define KNMUSICTAGFLAC_H

#include <QMultiHash>
#include <QLinkedList>

#include "knmusictagpraser.h"

namespace KNMusicFLAC
{
struct FLACMetadataBlock
{
    quint8 blockType=127; //Set the default type to invalid.
    QByteArray blockData;
};
struct VorbisCommentFrame
{
    QString fieldName;
    QString data;
};
struct PictureFrame
{
    QString mimeType;
    QString description;
    QImage image;
};
}

using namespace KNMusicFLAC;

class KNMusicTagFLAC : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagFLAC(QObject *parent = 0);
    bool praseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicDetailInfo &detailInfo);
    bool parseAlbumArt(KNMusicDetailInfo &detailInfo);

signals:

public slots:

private:
    void parseVorbisComment(QByteArray &blockData,
                            QLinkedList<VorbisCommentFrame> &tagMap);
    void parsePictureList(QList<QByteArray> &blocks,
                          QHash<int, PictureFrame> &imageMap);
    void parsePicture(QByteArray &blockData,
                      QHash<int, PictureFrame> &imageMap);
    void writeTagToDetails(QLinkedList<VorbisCommentFrame> &tagMap,
                           KNMusicDetailInfo &detailInfo);
    QHash<QString, int> m_fieldNameIndex;
};

#endif // KNMUSICTAGFLAC_H
