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
#ifndef KNMUSICCUEPARSER_H
#define KNMUSICCUEPARSER_H

#include <QHash>

#include "knmusiclistparser.h"

class KNMusicCueParser : public KNMusicListParser
{
    Q_OBJECT
public:
    explicit KNMusicCueParser(QObject *parent = 0);
    bool parseList(QFile &listFile,
                   KNMusicListDetailInfo &listDetailInfo);

signals:

public slots:

private:
    void parseCommand(const QString &rawLine,
                      QString &command,
                      QString &commandData);
    void parseMetaCommand(const QString &command,
                          const QString &data,
                          int &commandIndex,
                          QString &metaData, const bool &inTrack);
    qint64 timeTextToPosition(const QString &cueTimeText);
    int commandToIndex(const QString &command, const bool &inTrack);
    QHash<QString, int> m_trackCommandList,
                        m_albumCommandList,
                        m_publicCommandList;
};

#endif // KNMUSICCUEPARSER_H
