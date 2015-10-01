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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICTAGWAV_H
#define KNMUSICTAGWAV_H

#include "knmusictagid3v2.h"

class KNMusicTagWav : public KNMusicTagId3v2
{
    Q_OBJECT
public:
    explicit KNMusicTagWav(QObject *parent = 0);

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
    struct WAVItem
    {
        QString key;
        QString value;
    };
    inline void parseListChunk(char *rawData,
                               quint32 dataSize,
                               QList<WAVItem> &listData);
    static QHash<QString, int> m_listKeyIndex;
    static char m_id32Chunk[4];
    const char *m_riffHeader, *m_waveHeader, *m_listChunk,
               *m_listInfoHeader;
};

#endif // KNMUSICTAGWAV_H
