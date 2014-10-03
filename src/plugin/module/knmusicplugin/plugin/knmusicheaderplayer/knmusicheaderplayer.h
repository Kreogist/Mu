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
class KNMusicParser;
class KNScrollLabel;
class KNProgressSlider;
class KNEditableLabel;
class KNHighlightLabel;
class KNMusicGlobal;
class KNMusicHeaderPlayer : public KNMusicHeaderPlayerBase
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);

signals:

public slots:
    void setBackend(KNMusicBackend *backend);
    void reset();
    void playFile(const QString &filePath);
    void playSection(const QString &filePath,
                     const qint64 &startPosition,
                     const qint64 &duration);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void setAlbumArt(const QPixmap &pixmap);
    void setTitle(const QString &title);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);

private slots:
    void setDuration(const qint64 &duration);
    void onActionProgressPressed();
    void onActionProgressReleased();
    void onActionPositionEdited();
    void onActionLoopStateChanged(const int &state);
    void setPositionText(const qint64 &position);
    void onActionPlayNPauseClicked();
    void onActionVolumeSliderChanged(const qint64 &value);
    void onActionInOutOpacityChange(const QVariant &value);

    void onActionPositionChanged(const qint64 &position);
    void onActionPlayStateChanged(const int &state);

    void setPosition(const qint64 &position);

private:
    void initialAlbumArt();
    void initialLabels();
    void initialProrgess();
    void initialLoopState();
    void initialControlPanel();
    void initialVolume();
    void initialAppendPanel();

    void setPlayIconMode();
    void setPauseIconMode();

    void configureScrollLabel(KNScrollLabel *label);
    void configurePanelAnimation(QPropertyAnimation *animation);
    void configureAppendPanelAnimation(QPropertyAnimation *animation);
    void configureVolumeAnimation(QPropertyAnimation *animation);

    QRect generateOutPosition();
    QRect generateInPosition();

    bool loadFile(const QString &filePath);
    //Public classes.
    KNMusicGlobal *m_musicGlobal;
    KNMusicParser *m_parser;
    KNMusicBackend *m_backend;

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

    //Effects.
    QGraphicsOpacityEffect *m_albumArtEffect,
                           *m_durationEffect,
                           *m_volumeEffect;

    //States
    bool m_isShownPlay=true, m_progressPressed=false;
    int m_albumArtSize=61, m_buttonSize=38;

    //Images
    QPixmap m_playIcon, m_pauseIcon,
            m_noRepeatIcon, m_repeatTrackIcon, m_repeatAllIcon,
            m_muteIcon, m_noMuteIcon,
            m_noAlbumArt;
    QString m_artist, m_album, m_currentFilePath;
};

#endif // KNMUSICHEADERPLAYER_H
