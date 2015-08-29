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

#ifndef KNMUSICPLAYLIST_H
#define KNMUSICPLAYLIST_H

#include "knmusicplaylistbase.h"

class QSplitter;
class KNCategoryTab;
class KNEmptyStateWidget;
class KNMusicPlaylistList;
class KNMusicPlaylistViewer;
class KNMusicPlaylistManager;
/*!
 * \brief The KNMusicPlaylist class is the standard playlist realize. It can be
 * treat as a tutorial.
 */
class KNMusicPlaylist : public KNMusicPlaylistBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNMusicPlaylist class.
     * \param parent The parent widget.
     */
    explicit KNMusicPlaylist(QWidget *parent = 0);

    /*!
     * \brief Reimplement from the KNMusicPlaylistBase::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief  Reimplement from the KNMusicPlaylistBase::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief  Reimplement from the KNMusicPlaylistBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionCreatePlaylist();
    void onActionImportPlaylist();
    void onActionRemovePlaylist();
    void onActionExportPlaylist();

private:
    inline QSplitter *generateSplitter();
    KNCategoryTab *m_tab;
    KNEmptyStateWidget *m_container;
    KNMusicPlaylistList *m_playlistList;
    KNMusicPlaylistViewer *m_playlistViewer;
    KNMusicPlaylistManager *m_playlistManager;
};

#endif // KNMUSICPLAYLIST_H
