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

#include <QIcon>

#include "knmusicutil.h"

#include "knmusicheaderplayerbase.h"

using namespace MusicUtil;

class QGraphicsOpacityEffect;
class QParallelAnimationGroup;
class QPropertyAnimation;
class QLabel;
class KNGraphicsGlowEffect;
class KNEditableLabel;
class KNHighLightLabel;
class KNOpacityAnimeButton;
class KNOpacityButton;
class KNScrollLabel;
class KNProgressSlider;
class KNVolumeSlider;
class KNMusicBackend;
class KNMusicNowPlayingBase;
/*!
 * \brief The KNMusicHeaderPlayer class
 */
class KNMusicHeaderPlayer : public KNMusicHeaderPlayerBase
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::activate().
     */
    void activate() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::inactivate().
     */
    void inactivate() Q_DECL_OVERRIDE;

private slots:
    void updatePositionText(const qint64 &position);
    void onActionLoopStateChange(const int &state);
    void onActionMouseInOut(const QVariant &value);
    void onActionPositionEdited();
    void onActionVolumeChanged(const qint64 &value);
    void onActionPlayNPause();

private:
    inline void updateDurationPalette(const int &opacity);
    inline void setPosition(const qint64 &position);
    inline QRect generateOutPosition()
    {
        return QRect(0, -45, width(), 40);
    }
    inline QRect generateInPosition()
    {
        return QRect(0, 5, width(), 40);
    }
    inline QPropertyAnimation *generateAnime(QObject *target);
    inline KNOpacityAnimeButton *generateControlButton(const QString &iconPath);
    inline void startAnime(QParallelAnimationGroup *group,
                           QPropertyAnimation *volume,
                           QPropertyAnimation *control,
                           QPropertyAnimation *append);
    //Widgets
    KNOpacityAnimeButton *m_previous, *m_playNPause, *m_next;
    KNHighLightLabel *m_albumArt;
    KNScrollLabel *m_title, *m_artistAlbum;
    KNProgressSlider *m_progressSlider;
    KNVolumeSlider *m_volumeSlider;
    QLabel *m_duration;
    KNEditableLabel *m_position;
    KNOpacityButton *m_loopState, *m_volumeIndicator;
    QWidget *m_controlPanel, *m_volumePanel, *m_appendPanel;

    //Effects
    QGraphicsOpacityEffect *m_informationEffect;
    KNGraphicsGlowEffect *m_glowEffect;

    //Status.
    bool m_progressPressed;

    //Icons.
    QIcon m_loopStateIcon[LoopCount], m_muteIcon[2];

    //Animations
    QParallelAnimationGroup *m_mouseIn, *m_mouseOut;
    QPropertyAnimation *m_showControl, *m_hideControl,
                           *m_showVolume, *m_hideVolume,
                           *m_showAppend, *m_hideAppend;

    //Plugins.
    KNMusicBackend *m_backend;
    KNMusicNowPlayingBase *m_nowPlaying;
};

#endif // KNMUSICHEADERPLAYER_H
