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

#ifndef KNMUSICTAGAPEV2_H
#define KNMUSICTAGAPEV2_H

#include "knmusictagparser.h"

/*!
 * \brief The KNMusicTagApev2 class is used to parse APEv1 and APEv2 tag. It can
 * automatically detect the APEv1 and APEv2.
 */
class KNMusicTagApev2 : public KNMusicTagParser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagApev2 class.
     * \param parent The parent object.
     */
    explicit KNMusicTagApev2(QObject *parent = 0);

    /*!
     * \brief Reimplement from KNMusicTagParser::parseTag.
     */
    bool parseTag(QFile &musicFile,
                  QDataStream &musicDataStream,
                  KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicTagParser::writeTag.
     */
    bool writeTag(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from KNMusicTagParser::parseAlbumArt.
     */
    bool parseAlbumArt(KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;
signals:

public slots:

private:
    struct APEHeader
    {
        quint32 version;
        quint32 size;
        quint32 itemCount;
        quint32 flags;
    };

    inline bool checkHeader(const int &position,
                            QDataStream &dataStream,
                            APEHeader &header);

    inline quint32 dataToSize(char *data)
    {
        return (((quint32)data[3]<<24) & 0xFF000000)+
               (((quint32)data[2]<<16) & 0x00FF0000)+
               (((quint32)data[1] <<8) & 0x0000FF00)+
               ( (quint32)data[0]      & 0x000000FF);
    }

    const char *m_apePreamble;
};

#endif // KNMUSICTAGAPEV2_H
