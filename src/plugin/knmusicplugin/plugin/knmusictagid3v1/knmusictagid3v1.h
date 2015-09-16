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

#include "knmusictagparser.h"

/*!
 * \brief The KNMusicTagId3v1 class is used to parse ID3v1 tag. It can
 * automatically detect the ID3v1 and ID3v1.1.
 */
class KNMusicTagId3v1 : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagId3v1 class.
     * \param parent The parent object.
     */
    explicit KNMusicTagId3v1(QObject *parent = 0);

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
    struct ID3v1Struct
    {
        QString tags[5];
        int track;
        int genreIndex;
        ID3v1Struct() :
            track(-1),
            genreIndex(-1)
        {
        }
    };

    inline void parseRawData(char *rawTagData, ID3v1Struct &tagData);

    inline void writeToDetailInfo(const ID3v1Struct &tagData,
                                  KNMusicDetailInfo &detailInfo);

    inline QString standardizeText(const QString &text)
    {
        return text.simplified().remove(QChar('\0'));
    }

    inline void setRawTagData(char *rawTagData,
                              const QVariant &text,
                              const int &length);

    inline void backupByte(char *rawTagData,
                           const int &backupPosition,
                           char &backupPool,
                           int &positionPool,
                           const bool &clearData=false)
    {
        //Check the position backup pool.
        //If it's invalid(-1), restore the data.
        if(positionPool!=-1)
        {
            rawTagData[positionPool]=backupPool;
        }
        //Do the backup.
        backupPool=rawTagData[backupPosition];
        positionPool=backupPosition;
        //If ask to clear the data, clear it.
        if(clearData)
        {
            rawTagData[backupPosition]=0;
        }
    }

    QTextCodec *m_codec;
};

#endif // KNMUSICTAGID3V1_H
