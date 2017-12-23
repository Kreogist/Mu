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
#ifndef KNMUSICMAINPLAYERCONTENT_H
#define KNMUSICMAINPLAYERCONTENT_H

#include <QHash>

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QBoxLayout;
class QParallelAnimationGroup;
class QPropertyAnimation;
class KNHighLightLabel;
class KNMusicScrollLyrics;
class KNLoopScrollLabel;
class KNOpacityAnimeButton;
/*!
 * \brief The KNMusicMainPlayerContent class provides the container for all the
 * widgets. It manages the animation for all the moving widgets.\n
 * This container maintains the DFA for all the states. Switch the states
 * between these widgets.
 */
class KNMusicMainPlayerContent : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMainPlayerContent widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMainPlayerContent(QWidget *parent = nullptr);

signals:
    /*!
     * \brief Ask to show the current song in 'Songs' tab of the music library
     * tab.
     */
    void requireShowInSongs();

    /*!
     * \brief Ask to show the current song in 'Artists' tab of the music library
     * tab.
     */
    void requireShowInArtists();

    /*!
     * \brief Ask to show the current song in 'Albums' tab of the music library
     * tab.
     */
    void requireShowInAlbums();

    /*!
     * \brief Ask to show the current song in 'Genres' tab of the music library
     * tab.
     */
    void requireShowInGenres();

public slots:
    /*!
     * \brief Enable the horizontal layout or not.
     * \param isHorizontal If the content is in horizontal mode, set to true.
     */
    void setHorizontal(bool isHorizontal);

    /*!
     * \brief Set the analysis item to display.
     * \param item The analysis item of the display song.
     */
    void setAnalysisItem(const KNMusicAnalysisItem &item);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeChanged();

private:
    enum GotoButtons
    {
        GotoSong,
        GotoArtist,
        GotoAlbum,
        GotoGenre,
        GotoButtonCount
    };
    enum PanelState
    {
        StateSongInfo,
        StateLyrics,
        StatePlayList
    };
    inline void addAnimation(QWidget *widget);
    inline void showWidget(QWidget *widget, QPoint pos,
                           bool force=true, bool fromLeft=false);
    inline void applyPosition(bool force=true, bool fromLeft=false);
    KNOpacityAnimeButton *m_gotoIcons[GotoButtonCount];
    QHash<QWidget *, QPropertyAnimation *> m_animeMap;
    KNHighLightLabel *m_albumArt;
    KNLoopScrollLabel *m_titleLabel, *m_artistLabel, *m_albumLabel;
    KNMusicScrollLyrics *m_lyricsPanel;
    QWidget *m_playlistPanel, *m_infoContainer;
    QBoxLayout *m_gotoLayout;
    QParallelAnimationGroup *m_animationGroup;
    int m_state;
    bool m_isHorizontal;
};

#endif // KNMUSICMAINPLAYERCONTENT_H
