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

#ifndef KNMUSICLYRICSBACKEND_H
#define KNMUSICLYRICSBACKEND_H

#include <QObject>

/*!
 * \brief The KNMusicLyricsBackend class is used to synchronism the lyrics
 * position with the backend. And sync all the data to lyrics base widgets which
 * are linked to this backend via the requireMoveLyrics() signal.\n
 * This is a MVC-liked part for lyrics. Treat this part as a Model-Controller
 * part. It contains the data of the lyrics and provide the state changed data
 * to the view part.
 */
class KNMusicLyricsBackend : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNLMusicLyricsBackend object.
     * \param parent The parent object.
     */
    explicit KNMusicLyricsBackend(QObject *parent = 0);

    /*!
     * \brief lyricsDuration
     * \param index
     * \return
     */
    int lyricsDuration(const int &index);

    QString lyricsText(const int &index);


    bool isEmpty() const;

    int lastLine() const;

signals:
    void requireMoveTo(int lineIndex);
    void lyricsChanged();

public slots:
    void setPosition(const qint64 &position);
    void reset();
    void setLyricsData(const QList<qint64> &positions,
                       const QStringList &texts);

private:
    //Lyrics data.
    QList<qint64> m_positions;
    QStringList m_texts;

    //Lyrics status.
    int m_currentLine, m_lastLine;
};

#endif // KNMUSICLYRICSBACKEND_H
