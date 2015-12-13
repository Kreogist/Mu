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

/*!
 * \brief The KNMusicTagFlac class provides the tag parser of the metadata of
 * the FLAC format file.\n
 * This is a read only tag parser, you cannot use the writeTag() function to
 * write any data to a FLAC file.
 */
class KNMusicTagFlac : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagFlac object with a given parent.
     * \param parent The parent object.
     */
    explicit KNMusicTagFlac(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicTagParser::tagParserName().
     */
    QString tagParserName() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeTag.
     */
    bool writeTag(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writable().
     */
    bool writable() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicTagParser::writeCoverImage().
     */
    bool writeCoverImage() const Q_DECL_OVERRIDE;

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
