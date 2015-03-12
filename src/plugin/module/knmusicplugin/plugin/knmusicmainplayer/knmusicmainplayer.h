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
#ifndef KNMUSICMAINPLAYER_H
#define KNMUSICMAINPLAYER_H

#include "knmusicmainplayerbase.h"

class QLabel;
class QBoxLayout;
class QFormLayout;
class KNLabelButton;
class KNEditableLabel;
class KNOpacityAnimeButton;
class KNGlassAnimeButton;
class KNProgressSlider;
class KNVolumeSlider;
class KNMusicMainLyrics;
class KNMusicMainPlayer : public KNMusicMainPlayerBase
{
public:
    explicit KNMusicMainPlayer(QWidget *parent = 0);
    ~KNMusicMainPlayer();
    QWidget *banner();
    void setBackend(KNMusicBackend *backend);
    void setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer);
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);

public slots:
    void onActionAnalysisItemUpdate();
    void onActionLoopStateChanged(const int &state);

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    void resizeEvent(QResizeEvent *event);

protected slots:
    void onActionRequireShowIn(const int &label);

private slots:
    void retranslate();
    void onActionProgressPressed();
    void onActionProgressReleased();
    void onActionHideMainPlayer();
    void onActionPlayNPauseClicked();
    void onActionPositionEdited();

    void onActionPositionChanged(const qint64 &position);
    void onActionDurationChanged(const qint64 &duration);
    void onActionPlayStateChanged(const int &state);
    void onActionVolumeChanged(const int &volumeSize);

    void setPositionText(const qint64 &position);

    void setPosition(const qint64 &position);

private:
    inline void setPlayIconMode();
    inline void setPauseIconMode();

    enum InformationElements
    {
        ElementTitle,
        ElementArtist,
        ElementAlbum,
        ElementGenre,
        InformationElementsCount
    };

    KNOpacityAnimeButton *m_hideMainPlayer;

    inline void initialAlbumArt();
    //Album art widgets.
    QLabel *m_albumArtLabel;
    QPixmap m_albumArt;
    int m_pixmapSize=0;

    //Information layouts.
    inline void initialInformationPanel();
    inline void updateInformationPanel();
    QBoxLayout *m_mainLayout;
    QFormLayout *m_infoPanelLayout;
    int m_maxElementWidth=0;
    KNLabelButton *m_informationElementIcons[InformationElementsCount],
                  *m_informationElements[InformationElementsCount];

    //Lyrics panel.
    inline void initialLyricsPanel();
    KNMusicMainLyrics *m_mainLyrics;

    //Banner.
    inline void initialBanner();
    QWidget *m_banner;

    inline void initialPlaylistPanel();

    //Control panel.
    inline void initialControlPanel();
    QWidget *m_controlWidget;
    KNProgressSlider *m_progress;
    KNEditableLabel *m_position;
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

    KNGlassAnimeButton *m_controlButtons[ControlButtonsCount];
    QLabel *m_duration;
    QBoxLayout *m_buttonLeftLayout, *m_buttonRightLayout;
    KNLabelButton *m_volumeIcon;
    KNVolumeSlider *m_volumeSlider;
    QPixmap m_playIcon, m_pauseIcon, m_loopStateIcon[LoopCount],
            m_volumeSizeIcon[VolumeSizeCount];
    //Loop mode button.
    KNOpacityAnimeButton *m_loopMode;
    //Status.
    bool m_isShownPlay=true, m_progressPressed=false;

    inline void setEliedLabelText(QLabel *label,
                                  const QString &text,
                                  const int &width);

    //Backend.
    KNMusicBackend *m_backend=nullptr;
    int m_firstStageVolume=0, m_secondStageVolume=0;
    //Header player.
    KNMusicHeaderPlayerBase *m_headerPlayer=nullptr;
    KNMusicNowPlayingBase *m_nowPlaying=nullptr;
};

#endif // KNMUSICMAINPLAYER_H
