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

#ifndef KNMUSICTAGPARSER_H
#define KNMUSICTAGPARSER_H

#include <QFile>
#include <QDataStream>
#include <QObject>

#include "knmusicutil.h"

using namespace MusicUtil;

/*!
 * \brief The KNMusicTagParser is a interface of parser. The parser will use all
 * tag parser to parse the file. The parser can read and write for one kind of
 * tag.\n
 * Notice: The parseTag() function should be optimized for speed.\n
 */
class KNMusicTagParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTagParser class.
     * \param parent The parent object. It should be the parser.
     */
    KNMusicTagParser(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Parse the tag in the file. If we cannot find the tag in the file,
     * it should return false immediately.
     * \param musicFile The target music file.
     * \param musicDataStream The data stream of the music file.
     * \param analysisItem The file data item.
     * \return If the parser can find the tag and parse it successfully, return
     * true.
     */
    virtual bool parseTag(QFile &musicFile,
                          QDataStream &musicDataStream,
                          KNMusicAnalysisItem &analysisItem)=0;

    /*!
     * \brief Write the information of tag to the file.
     * \param analysisItem The information of the file.
     * \return If the parser write the tag successfully, then return true.
     */
    virtual bool writeTag(const KNMusicAnalysisItem &analysisItem)=0;

    /*!
     * \brief Parse the album art data from an analysised item.
     * \param analysisItem A analysised item with album art data.
     * \return If the parser can parse the analysised item, return true.
     */
    virtual bool parseAlbumArt(KNMusicAnalysisItem &analysisItem)=0;

protected:
    /*!
     * \brief Set the text data is a simple string setter. If the new string is
     * empty, it will not replaced the original text.
     * \param destination The destination string.
     * \param source The new data string.
     */
    inline void setTextData(QVariant &destination, const QString &source)
    {
        if(!source.isEmpty())
        {
            destination=QVariant(source);
        }
    }
};

#endif // KNMUSICTAGPARSER_H
