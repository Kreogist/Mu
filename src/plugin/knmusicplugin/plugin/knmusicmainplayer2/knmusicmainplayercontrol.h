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
#ifndef KNMUSICMAINPLAYERCONTROL_H
#define KNMUSICMAINPLAYERCONTROL_H

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QLabel;
class QGraphicsOpacityEffect;
class QTimeLine;
class KNOpacityButton;
class KNVolumeSlider;
class KNProgressSlider;
class KNEditableLabel;
class KNOpacityAnimeButton;
class KNMusicBackend;
class KNMusicNowPlayingBase;
/*!
 * \brief The KNMusicMainPlayerControl class provides the widget for control the
 * current playing song and playing sequence.
 */
class KNMusicMainPlayerControl : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayerControl widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayerControl(QWidget *parent = nullptr);

    /*!
     * \brief Set the playing backend to the main player control.
     * \param backend The music player backend.
     */
    void setBackend(KNMusicBackend *backend);

    /*!
     * \brief Set the now playing to the main player control.
     * \param nowPlaying The now playing object.
     */
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);

    /*!
     * \brief Get the hide main player button.
     * \return The button pointer.
     */
    KNOpacityAnimeButton *hideMainPlayer() const;

signals:
    /*!
     * \brief Ask the main player content to change the panel.
     * \param panel The panel index.
     */
    void requireSwitchPanel(int panel);

public slots:
    /*!
     * \brief When the main player panel content changed, this signal should be
     * called.
     * \param panelState The panel state.
     */
    void onPanelStateChange(int panelState);

protected:
    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onVolumeChanged(int volumeSize);
    void updateDuration(const qint64 &duration);
    void updatePositionText(const qint64 &position);

    void onLoopStateChanged(int state);
    void onActionPositionEdited();

    void onSwitchButtonPressed();

private:
    enum VolumeSizes
    {
        NoVolume,
        Volume1,
        Volume2,
        Volume3,
        VolumeSizeCount
    };
    inline void opacityMoveIn();
    inline void opacityMoveOut();
    inline void setPosition(const qint64 &position);
    inline KNOpacityAnimeButton *generateButton(
            const QString &imagePath=QString());
    inline void changeOpacity(int endFrame);
    QPixmap m_playIcon, m_pauseIcon, m_loopStateIcon[LoopCount],
            m_volumeSizeIcon[VolumeSizeCount],
            m_mainPlayerStates[MainPlayerStateCount];
    KNMusicBackend *m_backend;
    QTimeLine *m_changeOpacity;
    QGraphicsOpacityEffect *m_opacity, *m_buttonOpacity;
    KNEditableLabel *m_position;
    QLabel *m_duration;
    KNProgressSlider *m_progressSlider;
    KNOpacityAnimeButton *m_loopMode, *m_previous, *m_playNPause, *m_next,
                         *m_panelSwitch, *m_hideMainPlayer;
    KNOpacityButton *m_volumeIcon;
    KNVolumeSlider *m_volumeSlider;
    int m_currentOpacity, m_firstStageVolume, m_secondStageVolume, m_panelState;
    bool m_progressPressed;
};

#endif // KNMUSICMAINPLAYERCONTROL_H
