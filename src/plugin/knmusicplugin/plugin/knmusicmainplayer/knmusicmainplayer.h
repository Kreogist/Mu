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

#ifndef KNMUSICMAINPLAYER_H
#define KNMUSICMAINPLAYER_H

#include <QIcon>

#include "knmusicglobal.h"

#include "knmusicmainplayerbase.h"

using namespace MusicUtil;

class QBoxLayout;
class QLabel;
class KNEditableLabel;
class KNOpacityAnimeButton;
class KNProgressSlider;
class KNGlassAnimeButton;
class KNMusicScrollLyrics;
class KNMusicMainPlayerPanel;
/*!
 * \brief The KNMusicMainPlayer class is a default realize of the main music
 * player. It will use all the standard SDK file to build the main music player.
 * You can treat this as an example of the main player.
 */
class KNMusicMainPlayer : public KNMusicMainPlayerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayer(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicPlayerBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlayerBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionAnalysisItemChanged(const KNMusicAnalysisItem &item);
    void onActionPlayNPauseClicked();
    void updatePositionText(const qint64 &position);
    void updateDuration(const qint64 &duration);

private:
    enum ControlButtons
    {
        ButtonPrev,
        ButtonPlayNPause,
        ButtonNext,
        ControlButtonsCount
    };
    enum VolumeSizes
    {
        NoVolume,
        Volume1,
        Volume2,
        Volume3,
        VolumeSizeCount
    };
    inline void setPosition(const qint64 &position);
    //Resources.
    QIcon m_playIcon, m_pauseIcon, m_loopStateIcon[LoopCount],
          m_volumeSizeIcon[VolumeSizeCount];

    //Layouts.
    QBoxLayout *m_contentLayout;

    //Backends.
    KNMusicBackend *m_backend;

    //Global Controls/Panels.
    KNOpacityAnimeButton *m_hideMainPlayer;
    KNMusicMainPlayerPanel *m_detailInfoPanel;
    KNMusicScrollLyrics *m_lyricsPanel;
    QWidget *m_playlistPanel;
    QWidget *m_controlPanel;

    //Controls.
    KNProgressSlider *m_progressSlider;
    QLabel *m_duration;
    KNEditableLabel *m_position;
    KNOpacityAnimeButton *m_loopMode;
    KNGlassAnimeButton *m_controlButtons[ControlButtonsCount];

    //Status.
    bool m_progressPressed;
};

#endif // KNMUSICMAINPLAYER_H
