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

#include "knmusictagparser.h"

class KNMusicTagFLAC : public KNMusicTagParser
{
    Q_OBJECT
public:
    explicit KNMusicTagFLAC(QObject *parent = 0);

    /*!
     * \brief Reimplement from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicTagParser::writeTag.
     */
    bool writeTag(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    struct VorbisFrame
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

    inline void parseVorbisComment(QByteArray &blockData,
                                   QLinkedList<VorbisFrame> &tagMap);
    inline void parsePictureList(const QList<QByteArray> &blocks,
                                 QHash<int, PictureFrame> &imageMap);

    static QHash<QString, int> m_fieldNameIndex;
};

#endif // KNMUSICTAGFLAC_H
