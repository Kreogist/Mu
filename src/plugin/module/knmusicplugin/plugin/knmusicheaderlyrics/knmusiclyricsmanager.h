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
#ifndef KNMUSICLYRICSMANAGER_H
#define KNMUSICLYRICSMANAGER_H

#include <QFile>
#include <QMap>
#include <QStringList>

#include <QObject>

namespace KNMusicLyrics
{
enum SearchPolicy
{
    SameNameInLyricsDir,
    RelateNameInLyricsDir,
    SameNameInMusicDir,
    RelateNameInMusicDir
};
}

using namespace KNMusicLyrics;

class KNMusicLRCParser;
class KNMusicLyricsManager : public QObject
{
    Q_OBJECT
public:
    static KNMusicLyricsManager *instance();
    int lines() const;
    qint64 positionAt(const int &index) const;
    QString lyricsAt(const int &index) const;

signals:

public slots:
    void loadLyricsForFile(const QString &filePath);

private:
    void clear();
    bool findLyricsForFile(const QString &filePath);
    bool checkLyricsFile(const QString &lyricsPath);
    static KNMusicLyricsManager *m_instance;
    explicit KNMusicLyricsManager(QObject *parent = 0);

    KNMusicLRCParser *m_lrcParser;
    QString m_currentLyricsPath, m_lyricsFolderPath;
    QFile m_lyricsFile;
    QList<int> m_policyList;

    QList<qint64> m_positions;
    QStringList m_lyricsText;
    QMap<int, QString> m_lyricsProperty;
};

#endif // KNMUSICLYRICSMANAGER_H
