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
#ifndef KNMUSICLRCPARSER_H
#define KNMUSICLRCPARSER_H

#include <QRegularExpression>
#include <QMap>
#include <QList>

#include <QObject>

namespace KNMusicLRC
{
enum Properties
{
    Title,
    Artist,
    Album,
    LyricAuthor
};
struct LRCFrame
{
    qint64 position;
    QString text;
};
}

using namespace KNMusicLRC;

class KNMusicLRCParser : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLRCParser(QObject *parent = 0);
    QList<qint64> positions() const;

signals:

public slots:
    void parseFile(const QString &filePath,
                   QMap<int, QString> &properties,
                   QList<qint64> &positions,
                   QStringList &lyricsText);

private:
    void parseFrame(const QString &frame,
                    const QString &lineData,
                    QList<LRCFrame> &lyricsData,
                    QMap<int, QString> &properties);
    static bool frameLessThan(const LRCFrame &frameLeft,
                              const LRCFrame &frameRight);
    QRegularExpression m_frameCatch;
    QStringList m_headerText;
    QTextCodec *m_utf8Codec, *m_localeCodec;
};

#endif // KNMUSICLRCPARSER_H
