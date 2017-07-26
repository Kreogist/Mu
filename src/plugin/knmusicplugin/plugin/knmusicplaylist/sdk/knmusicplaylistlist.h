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

#ifndef KNMUSICPLAYLISTLIST_H
#define KNMUSICPLAYLISTLIST_H

#include <QWidget>

class KNAnimationMenu;
class KNLinearSenseWidget;
class KNOpacityAnimeButton;
class KNMusicPlaylistListView;
class KNMusicPlaylistListModel;
/*!
 * \brief The KNMusicPlaylistList class provides the a list of all the playlists
 * that in a model. The widget has a list view for displaying all the playlists
 * and a embedded editor for adding, removing and editing the playlists.
 */
class KNMusicPlaylistList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistList widget.
     * \param parent The parent widget.
     */
    explicit KNMusicPlaylistList(QWidget *parent = 0);

    /*!
     * \brief Set the playlist list model to the list.
     * \param model The model pointer.
     */
    void setPlaylistList(KNMusicPlaylistListModel *model);

    /*!
     * \brief Start to rename one of the playlist item in the list.
     * \param index The playlist index.
     */
    void renamePlaylist(const QModelIndex &index);

    /*!
     * \brief Get the current selected index.
     * \return The selected playlist index in the model.
     */
    QModelIndex currentIndex() const;

signals:
    /*!
     * \brief Require for create a new playlist.
     */
    void requireCreatePlaylist();

    /*!
     * \brief Require to show the import playlists dialog.
     */
    void requireImportPlaylists();

    /*!
     * \brief Require to export the current selected playlist.
     */
    void requireExportPlaylist();

    /*!
     * \brief Require to copy the current playlist.
     */
    void requireCopyPlaylist();

    /*!
     * \brief Require to remove the current playlist.
     */
    void requireRemovePlaylist();

    /*!
     * \brief Require to show a specific playlist in the model.
     * \param index The index of the playlist.
     */
    void requireShowPlaylist(const QModelIndex &index);

public slots:
    /*!
     * \brief Show the specific playlist.
     * \param index The index of the playlist.
     */
    void showPlaylist(const QModelIndex &index);

private slots:
    void retranslate();

private:
    inline KNAnimationMenu *generateEditorMenu();
    inline void configureEditor();
    enum EditorButton
    {
        Add,
        RemoveCurrent,
        Configure,
        EditorButtonCount
    };
    KNOpacityAnimeButton *m_buttons[EditorButtonCount];
    enum AddMenuAction
    {
        AddPlaylist,
        ImportPlaylist,
        AddMenuActionCount
    };
    enum ConfigureMenuAction
    {
        ExportPlaylist,
        CopyPlaylist,
        ConfigureMenuActionCount
    };
    QAction *m_addActions[AddMenuActionCount],
            *m_configureAction[ConfigureMenuActionCount];

    KNMusicPlaylistListView *m_playlistListView;
    KNLinearSenseWidget *m_playlistListEditor;
};

#endif // KNMUSICPLAYLISTLIST_H
