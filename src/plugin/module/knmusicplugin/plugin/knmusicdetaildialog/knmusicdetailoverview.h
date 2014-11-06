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
#ifndef KNMUSICDETAILOVERVIEW_H
#define KNMUSICDETAILOVERVIEW_H

#include <QWidget>

class QLabel;
class KNFilePathLabel;
class KNMusicParser;
class KNMusicDetailOverview : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicDetailOverview(QWidget *parent = 0);

signals:

public slots:
    void setFilePath(const QString &filePath);
    void retranslate();

private:
    KNMusicParser *m_parser;
    enum DetailInformation
    {
        DetailYear,
        DetailGenre,
        DetailKind,
        DetailSize,
        DetailBitRate,
        DetailSampleRate,
        DetailDateModified,
        DetailInformationCount
    };
    void initialBasicInfoLabel();
    void initialDetailInfoLabel();
    QLabel *m_albumArt, *m_title, *m_artist, *m_album, *m_duration,
           *m_detailInfoCaption[DetailInformationCount],
           *m_detailInfo[DetailInformationCount],
           *m_filePathCaption;
    KNFilePathLabel *m_filePathDataField;
};

#endif // KNMUSICDETAILOVERVIEW_H
