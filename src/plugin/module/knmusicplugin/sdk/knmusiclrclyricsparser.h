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
#ifndef KNMUSICLRCLYRICSPARSER_H
#define KNMUSICLRCLYRICSPARSER_H

#include <QList>
#include <QRegularExpression>

#include <QObject>

namespace KNMusicLRCLyrics
{
struct LyricsLine
{
    qint64 position;
    QString text;
};
}

using namespace KNMusicLRCLyrics;

class KNMusicLRCLyricsParser : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLRCLyricsParser(QObject *parent = 0);
    ~KNMusicLRCLyricsParser();
    bool parseFile(const QString &filePath,
                   QList<qint64> &positionList,
                   QStringList &textList);
    bool parseData(const QString &lyricsTextData,
                   QList<qint64> &positionList,
                   QStringList &textList);

signals:

public slots:

private:
    void parseFrames(QString frame,
                     const QString &text,
                     QList<LyricsLine> &positionList);
    static bool frameLessThan(const LyricsLine &frameLeft,
                              const LyricsLine &frameRight);
    QRegularExpression m_frameCatchRegExp, m_noneNumberRegExp;
    QTextCodec *m_utf8Codec, *m_localeCodec;
};

#endif // KNMUSICLRCLYRICSPARSER_H
