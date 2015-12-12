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

#ifndef KNMUSICPARSER_H
#define KNMUSICPARSER_H

#include <QList>
#include <QLinkedList>
#include <QFileInfo>
#include <QObject>

#include "knmusicutil.h"

using namespace MusicUtil;

class KNMusicAnalysiser;
class KNMusicTagParser;
class KNMusicListParser;
/*!
 * \brief The KNMusicParser class is the parser class which provides a unique
 * and simple way to analysis and parse the tag of the music file. It can also
 * analaysis the track list.
 */
class KNMusicParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicParser class.
     * \param parent The parent object.
     */
    explicit KNMusicParser(QObject *parent = 0);

    /*!
     * \brief Install a music file analysiser to the parser.
     * \param analysiser The analysiser pointer.
     */
    void installAnalysiser(KNMusicAnalysiser *analysiser);

    /*!
     * \brief Install a tag parser to the parser.
     * \param tagParser The tag parser pointer.
     */
    void installTagParser(KNMusicTagParser *tagParser);

    /*!
     * \brief Install a list parser to the parser.
     * \param listParser The list parser pointer.
     */
    void installListParser(KNMusicListParser *listParser);

    /*!
     * \brief Parse a music file to a analysis item. This won't parse the album
     * art data. If you want to parse that data, called parseAlbumArt() after.
     * \param fileInfo The QFileInfo class of the music file.
     * \param analysisItem The output analysis item.
     */
    void parseFile(const QFileInfo &fileInfo,
                   KNMusicAnalysisItem &analysisItem);

    /*!
     * \brief Parse a track list file.
     * \param filePath The track list file path.
     * \param trackItemList The track analysis item list.
     */
    void parseTrackList(const QString &filePath,
                        QList<KNMusicAnalysisItem> &trackItemList);

    /*!
     * \brief Using the tag parser to parse the previous album art data in the
     * item.
     * \param analysisItem The item which should be parsed by parseFile() or get
     * from parseTrackList().
     */
    void parseAlbumArt(KNMusicAnalysisItem &analysisItem);

    /*!
     * \brief Analysis an item again. Update the data in the item.
     * \param analysisItem An old analysis item. The file path should be correct
     * to find the file.
     * \return Re-analysis it successful, it will return true.
     */
    bool reanalysisItem(KNMusicAnalysisItem &analysisItem);

    /*!
     * \brief Write analysis item to file path.
     * \param analysisItem The analysis item.
     */
    bool writeAnalysisItem(const KNMusicAnalysisItem &analysisItem);

private:
    inline bool findImageFile(const QString &baseName,
                              KNMusicAnalysisItem &item);
    inline bool checkImageFile(const QString &filePath,
                               KNMusicAnalysisItem &item);
    QList<QString> m_imageTypes;
    QLinkedList<KNMusicAnalysiser *> m_analysisers;
    QLinkedList<KNMusicTagParser *> m_tagParsers;
    QLinkedList<KNMusicListParser *> m_listParsers;
};

#endif // KNMUSICPARSER_H
