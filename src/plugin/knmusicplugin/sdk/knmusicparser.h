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

    void installAnalysiser(KNMusicAnalysiser *analysiser);
    void installTagParser(KNMusicTagParser *tagParser);
    void installListParser(KNMusicListParser *listParser);

    void parseFile(const QFileInfo &fileInfo,
                   KNMusicAnalysisItem &analysisItem);
    void parseTrackList(const QString &filePath,
                        QList<KNMusicAnalysisItem> &trackDetailList);
    void parseAlbumArt(KNMusicAnalysisItem &analysisItem);

    bool reanalysisItem(KNMusicAnalysisItem &analysisItem);

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
