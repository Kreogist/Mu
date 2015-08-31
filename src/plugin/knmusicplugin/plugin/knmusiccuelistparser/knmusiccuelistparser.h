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

#ifndef KNMUSICCUELISTPARSER_H
#define KNMUSICCUELISTPARSER_H

#include "knmusiclistparser.h"

class KNMusicCueListParser : public KNMusicListParser
{
    Q_OBJECT
public:
    explicit KNMusicCueListParser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicListParser::parseList().
     */
    bool parseList(QFile &listFile,
                   KNMusicListDetailInfo &listDetailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicListParser::writeDetail().
     */
    bool writeDetail(const KNMusicAnalysisItem &analysisItem) Q_DECL_OVERRIDE;

private:
    inline int parseMetaCommand(const QString &command,
                                const QString &data,
                                QString &metaData,
                                const bool &inTrack);

    inline void parseCommand(const QString &rawLine,
                             QString &command,
                             QString &data);

    inline qint64 textToTime(const QString &cueTimeText);

    static QHash<QString, int> m_trackCommandList,
                               m_albumCommandList,
                               m_publicCommandList;
};

#endif // KNMUSICCUELISTPARSER_H
