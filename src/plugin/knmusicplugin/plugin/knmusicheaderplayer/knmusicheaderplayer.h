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

class QSignalMapper;
class QGraphicsOpacityEffect;
class QParallelAnimationGroup;
class QPropertyAnimation;
class QLabel;
class KNSaoSubMenu;
class KNConfigure;
class KNEditableLabel;
class KNHighLightLabel;
class KNOpacityAnimeButton;
class KNOpacityButton;
class KNScrollLabel;
class KNProgressSlider;
class KNVolumeSlider;
class KNMusicScrollLyrics;
/*!
 * \brief The KNMusicHeaderPlayer class is a default header player implemented
 * from the header player base. This is an example of the header player base.
 */
class KNMusicHeaderPlayer : public KNMusicHeaderPlayerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicHeaderPlayer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicHeaderPlayer(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::lyrics().
     */
    QWidget *lyrics() Q_DECL_OVERRIDE;

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

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::loadConfigure().
     */
    void loadConfigure() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::saveConfigure().
     */
    void saveConfigure() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicHeaderPlayerBase::reset().
     */
    void reset() Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void updatePositionText(const qint64 &position);
    void onActionLoopStateChange(const int &state);
    void onActionMouseInOut(const QVariant &value);
    void onActionPositionEdited();
    void onActionVolumeChanged(const qint64 &value);
    void onActionNowPlayingChanged(const KNMusicAnalysisItem &analysisItem);
    void onLyricsConfigureChanged();
    void showAppendMenu();
    void appendActionTriggered(const int &actionIndex);
    void updateDuration(const qint64 &duration);

private:
    enum SubMenus
    {
        SubMenuRating,
        SubMenuLocate,
        SubMenuCount
    };
    enum MenuActions
    {
        AppendRatingNoStar,
        AppendRatingOneStar,
        AppendRatingTwoStar,
        AppendRatingThreeStar,
        AppendRatingFourStar,
        AppendRatingFiveStar,
        AppendShowInGraphicShell,
        AppendShowDetail,
        AppendDownloadLyrics,
        AppendLocateNowPlaying,
        AppendShowInSongs,
        AppendShowInArtists,
        AppendShowInAlbums,
        AppendShowInGenres,
        MenuActionsCount
    };
    inline int panelYPosition();
    inline void updateDurationPalette(int opacity);
    inline void setPosition(const qint64 &position);
    inline QRect generateOutPosition();
    inline QPropertyAnimation *generateAnime(QObject *target);
    inline KNOpacityAnimeButton *generateControlButton(const QString &iconPath);
    inline KNOpacityAnimeButton *generateAppendButton(const QString &iconPath);
    inline void startAnime(QParallelAnimationGroup *group,
                           QPropertyAnimation *volume,
                           QPropertyAnimation *control,
                           QPropertyAnimation *append);
    inline void setAristAndAlbum(const QString &artist, const QString &album);

    //Widgets
    KNMusicScrollLyrics *m_headerLyrics;
    KNOpacityAnimeButton *m_previous, *m_playNPause, *m_next, *m_showMainPlayer,
                         *m_showMiniPlayer, *m_showAppendMenu;
    KNHighLightLabel *m_albumArt;
    KNScrollLabel *m_title, *m_artistAlbum;
    KNProgressSlider *m_progressSlider;
    KNVolumeSlider *m_volumeSlider;
    QLabel *m_duration;
    KNEditableLabel *m_position;
    KNOpacityButton *m_loopState, *m_volumeIndicator;
    QWidget *m_controlPanel, *m_volumePanel, *m_appendPanel, *m_progressPanel;
    //Append menu.
    KNSaoSubMenu *m_appendMenu;
    QSignalMapper *m_actionTrigger;
    QAction *m_menuActions[MenuActionsCount];
    KNSaoSubMenu *m_subMenus[SubMenuCount];

    //Effects
    QGraphicsOpacityEffect *m_informationEffect;

    //Icons.
    QIcon m_iconLoopState[LoopCount], m_iconMute[2], m_iconPlay, m_iconPause;

    //Animations
    QParallelAnimationGroup *m_mouseIn, *m_mouseOut;
    QPropertyAnimation *m_showControl, *m_hideControl,
                           *m_showVolume, *m_hideVolume,
                           *m_showAppend, *m_hideAppend;

    //Plugins.
    KNMusicBackend *m_backend;
    KNMusicNowPlayingBase *m_nowPlaying;

    //Configures.
    KNConfigure *m_cacheConfigure, *m_musicConfigure,
                *m_lyricsConfigure;

    //Status.
    bool m_appendMenuShown, m_progressPressed;
};

#endif // KNMUSICHEADERPLAYER_H
