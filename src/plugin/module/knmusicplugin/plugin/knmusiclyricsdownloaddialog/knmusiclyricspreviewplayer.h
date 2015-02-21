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
#ifndef KNMUSICLYRICSPREVIEWPLAYER_H
#define KNMUSICLYRICSPREVIEWPLAYER_H

#include <QWidget>

class KNConnectionHandler;
class KNOpacityButton;
class KNProgressSlider;
class KNMusicBackend;
class KNMusicLyricsManager;
class KNMusicLRCLyricsParser;
class KNMusicScrollLyrics;
class KNMusicLyricsPreviewPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsPreviewPlayer(QWidget *parent = 0);
    ~KNMusicLyricsPreviewPlayer();
    void setBackend(KNMusicBackend *backend);

    void linkBackend();
    void resetPreviewPlayer();

    void setLyrics(const QString &lyricsText);
    void clearLyrics();

signals:

public slots:

private slots:
    void onActionPlayNPauseClick();
    void onActionProgressPressed();
    void onActionProgressReleased();
    void onActionPreviewStatusChange(const int &state);
    void onActionPreviewPositionChanged(const qint64 &position);
    void onActionPreviewDurationChanged(const qint64 &duration);

private:
    inline void initialPlayerControls();
    KNMusicLyricsManager *m_lyricsManager;
    KNMusicLRCLyricsParser *m_parser;
    KNOpacityButton *m_playNPause;
    KNProgressSlider *m_progress;
    QPixmap m_playIcon, m_pauseIcon;
    KNMusicBackend *m_backend;
    KNConnectionHandler *m_backendLinks;
    KNMusicScrollLyrics *m_lyricsDisplay;

    bool m_isButtonPlay=true, m_progressPressed=false;
};

#endif // KNMUSICLYRICSPREVIEWPLAYER_H
