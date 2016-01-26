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

#include <QStringList>

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
     * \brief Get the duration of a specific line of lyrics.
     * \param index The lyrics line index.
     * \return The lyrics line duration.
     */
    int lyricsDuration(const int &index);

    /*!
     * \brief Get the lyrics position.
     * \param index The lyrics line.
     * \return The lyrics display position.
     */
    qint64 lyricsPosition(const int &index);

    /*!
     * \brief Get the text of the lyrics at a specific line.
     * \param index The lyrics line index.
     * \return The lyrics text.
     */
    QString lyricsText(const int &index);

    /*!
     * \brief Get whether the backend is empty. That means it holds nothing.
     * \return If there's no file loaded to the backend, return true.
     */
    bool isEmpty() const;

    /*!
     * \brief Get the index of the last line.
     * \return The last line index.
     */
    int lastLine() const;

signals:
    /*!
     * \brief When the position changed, and the line of the lyrics changed,
     * this signal will be emitted.
     * \param lineIndex The current lyrics line.
     * \param position The position of current line.
     */
    void requireMoveTo(int lineIndex, qint64 position);

    /*!
     * \brief If the lyrics changed, that means a new lyrics is loaded, or the
     * backend is reset, this signal will be emitted.
     */
    void lyricsChanged();

    /*!
     * \brief When the view of the backend should be update itself, this signal
     * will be emitted.
     */
    void updateWidget();

public slots:
    /*!
     * \brief Set the position of the song, and backend will automaticially
     * locate the position to the specific line.
     * \param position The position of the song.
     */
    void setPosition(const qint64 &position);

    /*!
     * \brief Clear all the data of the lyrics backend.
     */
    void reset();

    /*!
     * \brief Set the lyrics data to the backend.
     * \param positions The lyrics time position lists.
     * \param texts The text lists.
     */
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
