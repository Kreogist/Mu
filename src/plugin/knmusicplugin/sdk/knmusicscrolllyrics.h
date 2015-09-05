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

#ifndef KNMUSICSCROLLLYRICS_H
#define KNMUSICSCROLLLYRICS_H

#include "knmusiclyricsbase.h"

class QTimeLine;
/*!
 * \brief The KNMusicScrollLyrics class provides a lyrics display in vertical
 * mode, and it can scroll to the specific line if you give the widget the
 * position.
 */
class KNMusicScrollLyrics : public KNMusicLyricsBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicScrollLyrics widget with given parent.
     * \param parent The parent widget of the lyrics view.
     */
    explicit KNMusicScrollLyrics(QWidget *parent = 0);

    /*!
     * \brief backend
     * \return
     */
    KNMusicLyricsBackend *backend() Q_DECL_OVERRIDE;


signals:

public slots:
    /*!
     * \brief setBackend
     * \param backend
     */
    void setBackend(KNMusicLyricsBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief reset
     */
    void reset();

    /*!
     * \brief moveToLine
     * \param lineIndex
     */
    void moveToLine(const int &lineIndex) Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionLyricsMoved(const int &frame);

private:

    inline QRect lyricsRect(const QString &lyricsText)
    {
        return fontMetrics().boundingRect(m_leftSpacing,
                                          0,
                                          width()-m_leftSpacing,
                                          height(),
                                          Qt::TextWordWrap,
                                          lyricsText);
    }

    //Backend.
    KNMusicLyricsBackend *m_backend;
    //UI parameters.
    QTimeLine *m_moveToCurrentLine;
    //Lyrics status.
    int m_currentLine, m_centerOffset;
    //UI parameters.
    int m_leftSpacing, m_spacing;
};

#endif // KNMUSICSCROLLLYRICS_H
