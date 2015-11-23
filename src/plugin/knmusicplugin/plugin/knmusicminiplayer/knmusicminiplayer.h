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

#ifndef KNMUSICMINIPLAYER_H
#define KNMUSICMINIPLAYER_H

#include <QIcon>

#include "knmusicminiplayerbase.h"

class QTimeLine;
class KNProgressSlider;
class KNImageLabel;
class KNEditableLabel;
class KNOpacityAnimeButton;
class KNMusicHScrollLyrics;
/*!
 * \brief The KNMusicMiniPlayer class
 */
class KNMusicMiniPlayer : public KNMusicMiniPlayerBase
{
    Q_OBJECT
public:
    explicit KNMusicMiniPlayer(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::setBackend().
     */
    void setBackend(KNMusicBackend *backend) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::setNowPlaying().
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying) Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicMiniPlayerBase::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void onActionMouseInOut(int frame);

private:
    inline KNOpacityAnimeButton *generateButton(
            const QString &iconPath=QString());
    inline void startAnime(int targetFrame);
    inline void setPosition(const qint64 &position);
    QIcon m_muteIcon[2], m_iconPause, m_iconPlay;
    QPoint m_pressedPoint, m_originalPos;
    QWidget *m_container;
    KNImageLabel *m_icon;
    KNEditableLabel *m_position;
    KNOpacityAnimeButton *m_previous, *m_playNPause, *m_next, *m_mute,
                         *m_restore, *m_close;
    KNProgressSlider *m_progressSlider;
    KNMusicHScrollLyrics *m_lyrics;
    QTimeLine *m_moving;

    KNMusicBackend *m_backend;

    bool m_progressPressed, m_pressed;
};

#endif // KNMUSICMINIPLAYER_H
