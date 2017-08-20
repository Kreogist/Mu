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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICHSCROLLLYRICS_H
#define KNMUSICHSCROLLLYRICS_H

#include "knmusiclyricsbase.h"

class QTimeLine;
class KNMusicBackend;
/*!
 * \brief The KNMusicHScrollLyrics class provides a lyrics display in horizontal
 * mode, and it can scroll to the specific line if you give the widget the
 * position.
 */
class KNMusicHScrollLyrics : public KNMusicLyricsBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicScrollLyrics widget with given parent.
     * \param parent The parent widget of the lyrics view.
     */
    explicit KNMusicHScrollLyrics(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLyricsBase::backend.
     */
    KNMusicLyricsBackend *backend() Q_DECL_OVERRIDE;

    /*!
     * \brief Get the spacing of the lyrics.
     * \return The lyrics line spacing.
     */
    int spacing() const;

    /*!
     * \brief Get the alignment of the lyrics text.
     * \return The whole alignment of the text.
     */
    Qt::Alignment alignment() const;

    /*!
     * \brief Get the place holder text. When there's no lyrics, this text will
     * be displayed.
     * \return The place holder text.
     */
    QString placeHolderText() const;

signals:

public slots:
    /*!
     * \brief Set the place holder text.
     * \param placeHolderText The text will be displayed.
     */
    void setPlaceHolderText(const QString &placeHolderText);

    /*!
     * \brief Set the spacing between two lyrics lines.
     * \param spacing The spacing of two lines. This value should be positive.
     * If the number is negative, it will be set to 0.
     */
    void setSpacing(int spacing);

    /*!
     * \brief Reimplemented from KNMusicLyricsBase::setBackend.
     */
    void setBackend(KNMusicLyricsBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Clear the scroll lyrics widget data. Reset the timer.
     */
    void reset();

    /*!
     * \brief Change the alignment of all the lyrics text.
     * \param alignment The alignment of text.
     */
    void setAlignment(Qt::Alignment alignment);

    /*!
     * \brief Reimplemented from KNMusicLyricsBase::moveToLine().
     */
    void moveToLine(int lineIndex,
                    const qint64 &position) Q_DECL_OVERRIDE;

    /*!
     * \brief Pause/Resume the time line if possible.
     * \param pause Pause or unpause the time line.
     */
    void setPause(bool pause);

protected:
    /*!
     * \brief Reimplemented from KNMusicLyricsBase::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionLyricsMoved(int frame);

private:
    //Backend.
    KNMusicLyricsBackend *m_backend;
    //UI parameters.
    QString m_placeHolderText;
    QTimeLine *m_moveToCurrentLine;
    //Lyrics status.
    int m_currentLine, m_centerOffset;
    //UI parameters.
    int m_spacing, m_minimumDuration;
    //Alignment flags.
    Qt::Alignment m_alignment;
};

#endif // KNMUSICHSCROLLLYRICS_H
