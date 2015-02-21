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

#include <QList>

#include "knmusiclyricsbase.h"

class QTimeLine;
class KNMusicScrollLyrics : public KNMusicLyricsBase
{
    Q_OBJECT
public:
    explicit KNMusicScrollLyrics(QWidget *parent = 0);
    int spacing() const;
    void setSpacing(int spacing);
    void setLyricsData(const QList<qint64> positions, const QStringList texts);

signals:

public slots:
    void onActionPositionChange(const qint64 &position);
    void onActionLyricsReset();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionLyricsMoved(const int &frame);

private:
    inline void resetLyricsData();
    inline QSize lyricsSize(const QString &lyricsText)
    {
        return fontMetrics().size(Qt::TextExpandTabs, lyricsText);
    }
    inline void startMove(const int &lineDuration,
                          const int &displacement);
    inline int lyricsDuration(const int &index);
    //Lyrics moving animation.
    QTimeLine *m_moveToCurrentLine;

    //Lyrics data.
    QList<qint64> m_positions;
    QStringList m_texts;

    //Lyrics status.
    int m_currentLine=-1, m_lastLine=-1, m_centerOffset=0;

    //UI parameters.
    int m_leftSpacing=15, m_spacing=2;
    QColor m_normalText=QColor(100,100,100),
           m_highlightColor=QColor(0xf7, 0xcf, 0x3d);

    //Lyrics moving const.
    const int m_maximumDuration=200;
};

#endif // KNMUSICSCROLLLYRICS_H
