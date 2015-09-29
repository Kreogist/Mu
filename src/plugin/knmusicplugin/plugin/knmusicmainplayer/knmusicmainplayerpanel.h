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

#ifndef KNMUSICMAINPLAYERPANEL_H
#define KNMUSICMAINPLAYERPANEL_H

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QBoxLayout;
class KNOpacityAnimeButton;
class KNLoopScrollLabel;
class KNHighLightLabel;
/*!
 * \brief The KNMusicMainPlayerPanel class
 */
class KNMusicMainPlayerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicMainPlayerPanel(QWidget *parent = 0);

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
    void setAnalysisItem(const KNMusicAnalysisItem &item);
    void updatePanelFont(const QFont &labelFont);

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
    inline void setAristAndAlbum(const QString &artist, const QString &album);
    KNOpacityAnimeButton *m_gotoIcons[GotoButtonCount];
    KNLoopScrollLabel *m_titleLabel, *m_artistAlbumLabel;
    QBoxLayout *m_buttonLayout;
    QWidget *m_detailPanel;
    KNHighLightLabel *m_albumArt;
};

#endif // KNMUSICMAINPLAYERPANEL_H
