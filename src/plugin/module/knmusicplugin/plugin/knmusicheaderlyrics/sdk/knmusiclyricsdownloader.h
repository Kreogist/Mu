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
#ifndef KNMUSICLYRICSDOWNLOADER_H
#define KNMUSICLYRICSDOWNLOADER_H

#include "knmusicglobal.h"

#include <QObject>

using namespace KNMusic;

class QTimer;
class QNetworkReply;
class QNetworkAccessManager;
class KNMusicLyricsDownloader : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloader(QObject *parent = 0);
    virtual QString downloadLyrics(const KNMusicDetailInfo &detailInfo)=0;

signals:

public slots:

protected:
    inline QString processKeywords(QString str)
    {
        //Clear some no used words. I don't know how these regexp works.
        //Magic, don't touch.
        str=str.toLower();
        str.replace('.', " ");
        str.replace(QRegExp("\\'|·|\\$|\\&|–"), " ");
        str.replace(QRegExp("\\(.*?\\)|\\[.*?]|{.*?}|\\uff08.*?\\uff09"), " ");
        str.replace(QRegExp("[-/:-@[-`{-~]+"), " ");
        return str;
    }
    void get(const QString &url, QByteArray &responseData);
    QString writeLyricsFile(const KNMusicDetailInfo &detailInfo,
                            const QString &content);

private:
    QNetworkAccessManager *m_networkManager;
    QTimer *m_timeout;
};

#endif // KNMUSICLYRICSDOWNLOADER_H
