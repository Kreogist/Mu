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
#ifndef KNMUSICHEADERPLAYER_H
#define KNMUSICHEADERPLAYER_H

#include "knmusicheaderplayerbase.h"

class QLabel;
class QGraphicsOpacityEffect;
class QParallelAnimationGroup;
class QPropertyAnimation;
class KNOpacityButton;
class KNOpacityAnimeButton;
class KNVolumeSlider;
class KNGlobal;
class KNScrollLabel;
class KNProgressSlider;
class KNEditableLabel;
class KNHighlightLabel;
class KNMusicHeaderPlayerAppendMenu;
class KNMusicGlobal;
class KNMusicHeaderPlayer : public KNMusicHeaderPlayerBase
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);
    ~KNMusicHeaderPlayer();
    void setBackend(KNMusicBackend *backend);
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);

signals:

public slots:
    void restoreConfigure();
    void onActionLoopStateChanged(const int &state);
    void reset();
    void play();
    void activatePlayer();
    void inactivatePlayer();

private slots:
    void setAlbumArt(const QPixmap &pixmap);
    void setTitle(const QString &title);
    void setDuration(const qint64 &duration);
    void onActionPlayDragIn(const QStringList &filePaths);
    void onActionProgressPressed();
    void onActionProgressReleased();
    void onActionPositionEdited();
    void onActionPlayNPauseClicked();
    void onActionVolumeSliderChanged(const qint64 &value);
    void onActionInOutOpacityChange(const QVariant &value);
    void onActionShowAppendMenu();
    void onActionAppendMenuActionTriggered(int actionIndex);
    void setPositionText(const qint64 &position);

    void onActionPositionChanged(const qint64 &position);
    void onActionPlayStateChanged(const int &state);

    void setPosition(const qint64 &position);
    void updatePlayerInfo(const KNMusicDetailInfo &detailInfo);

private:
    inline void initialAlbumArt();
    inline void initialLabels();
    inline void initialProrgess();
    inline void initialLoopState();
    inline void initialControlPanel();
    inline void initialVolume();
    inline void initialAppendPanel();
    inline void initialAppendMenu();

    inline void setPlayIconMode();
    inline void setPauseIconMode();
    inline void updateArtistAndAlbum();

    inline void configureScrollLabel(KNScrollLabel *label);
    inline void configurePanelAnimation(QPropertyAnimation *animation);
    inline void configureAppendPanelAnimation(QPropertyAnimation *animation);
    inline void configureVolumeAnimation(QPropertyAnimation *animation);

    inline QRect generateOutPosition();
    inline QRect generateInPosition();

    inline void saveConfigure();

    //Public classes.
    KNMusicGlobal *m_musicGlobal;
    KNMusicBackend *m_backend;
    KNMusicNowPlayingBase *m_nowPlaying;
    KNGlobal *m_global;

    //Animations
    QParallelAnimationGroup *m_mouseIn, *m_mouseOut;
    QPropertyAnimation *m_showPanel, *m_hidePanel,
                       *m_showVolume, *m_hideVolume,
                       *m_showAppendPanel, *m_hideAppendPanel;

    //Widgets.
    QWidget *m_controlPanel, *m_progressPanel, *m_volumePanel, *m_appendPanel;
    KNHighlightLabel *m_albumArt;
    KNScrollLabel *m_title, *m_artistAndAlbum;
    QLabel *m_duration;
    KNProgressSlider *m_progressSlider;
    KNEditableLabel *m_positionDisplay;
    KNOpacityButton *m_loopStatus, *m_volumeIndicator;
    KNOpacityAnimeButton *m_previous, *m_next, *m_playNPause,
                             *m_showMainPlayer, *m_showAppendMenu;
    KNVolumeSlider *m_volumeSlider;
    KNMusicHeaderPlayerAppendMenu *m_appendMenu;

    //Effects.
    QGraphicsOpacityEffect *m_albumArtEffect,
                           *m_durationEffect,
                           *m_volumeEffect;

    //Actions.
    QAction *showIn[4];

    //States
    bool m_isShownPlay=true, m_progressPressed=false, m_appendMenuShown=false;
    int m_albumArtSize=61, m_buttonSize=38, m_loopState=NoRepeat;

    //Images
    QPixmap m_playIcon, m_pauseIcon,
            m_noRepeatIcon, m_repeatTrackIcon, m_repeatAllIcon,
            m_muteIcon, m_noMuteIcon,
            m_noAlbumArt;
    QString m_artist, m_album, m_currentFilePath;
};

#endif // KNMUSICHEADERPLAYER_H
