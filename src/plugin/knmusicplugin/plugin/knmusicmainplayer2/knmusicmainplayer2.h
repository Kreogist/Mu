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
#ifndef KNMUSICMAINPLAYER2_H
#define KNMUSICMAINPLAYER2_H

#include "knmusicutil.h"

#include "knmusicmainplayerbase.h"

using namespace MusicUtil;

class KNOpacityAnimeButton;
class KNMusicMainPlayerControl;
class KNMusicMainPlayerContent;
/*!
 * \brief The KNMusicMainPlayer class is a default realize of the main music
 * player. It will use all the standard SDK file to build the main music player.
 * You can treat this as an example of the main player.
 */
class KNMusicMainPlayer2 : public KNMusicMainPlayerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayer widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayer2(QWidget *parent = nullptr);

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
    /*!
     * \brief Reimplemented from KNMusicPlayerBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicPlayerBase::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onNowPlayingChanged(const KNMusicAnalysisItem &analysisItem);

private:
    inline void scaleBackground();
    QPixmap m_background, m_originalBackground;
    KNMusicMainPlayerControl *m_controlPanel;
    KNMusicMainPlayerContent *m_contentContainer;
    bool m_isHorizontal;
};

#endif // KNMUSICMAINPLAYER2_H
