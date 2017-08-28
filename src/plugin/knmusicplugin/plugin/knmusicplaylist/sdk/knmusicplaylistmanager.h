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

#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QObject>

class QThread;
class KNMusicModel;
class KNMusicPlaylistParser;
class KNMusicPlaylistModel;
class KNMusicPlaylistEngine;
class KNMusicPlaylistListModel;
/*!
 * \brief The KNMusicPlaylistManager class will provides you a higher management
 * API for playlist model management. It can create, remove, save, load, import
 * and export playlist models.
 */
class KNMusicPlaylistManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistManager object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistManager(QObject *parent = 0);
    ~KNMusicPlaylistManager();

    /*!
     * \brief Get the playlist list object pointer. This function should be
     * called when you want to let a list view to display the playlist list
     * model.
     * \return The playlist list model pointer.
     */
    KNMusicPlaylistListModel *playlistList();

    /*!
     * \brief Get the specific playlist model pointer according to the index
     * inside the playlist list.
     * \param index The playlist model index.
     * \return The playlist model pointer.
     */
    KNMusicPlaylistModel *playlist(const QModelIndex &index);

    /*!
     * \brief This is a override function. Get a playlist model according to the
     * identifier of the playlist.
     * \param identifer The identifier of the playlist.
     * \return If the identifer is not exist in the list, return nullptr. Or
     * the pointer to the playlist model.
     */
    KNMusicPlaylistModel *playlist(const QString &identifier);

    /*!
     * \brief Get the playlist directory path. If you haven't set this path
     * before, it will be empty.\n
     * You MUST set the directory path after you initialize the playlist
     * manager.
     * \return The playlist directory path.
     */
    QString playlistDirPath() const;

    /*!
     * \brief Set the playlist directory path.
     * \param playlistDirPath The path of the playlist directory.
     */
    void setPlaylistDirPath(const QString &playlistDirPath);

    /*!
     * \brief Load the playlist from the directory to the list.
     */
    void loadPlaylistList();

    /*!
     * \brief Load a Kreogist Mu standard playlist file.
     * \param filePath The file path of the standard Kreogist Mu playlist.
     * \return If we can load the playlist successfully, return the playlist
     * model pointer.
     */
    KNMusicPlaylistModel *loadPlaylist(const QString &filePath);

    /*!
     * \brief Save a playlist model to a specific file path.
     * \param model The model pointer.
     * \return If the model write to the file path stored in the model
     * successfully, returns true.
     */
    bool savePlaylist(KNMusicPlaylistModel *model);

    /*!
     * \brief Remove one playlist model from the list.
     * \param model The model pointer.
     * \return The result of the model pointer.
     */
    bool removePlaylist(KNMusicPlaylistModel *model);

    /*!
     * \brief Import a playlist model from a specific file.
     * \param filePath The file path.
     * \return The imported model index from the playlist list model.
     */
    QModelIndex importPlaylist(const QString &filePath);

    /*!
     * \brief Import a dozon of playlists for one time.
     * \param filePaths The import file paths.
     * \return If any of the playlist file is import successfully, return the
     * first model index of the playlist. Or else a null model index.
     */
    QModelIndex importPlaylists(const QStringList &filePaths);

    /*!
     * \brief Export a playlist model to a specific file using a specific format
     * via providing index.
     * \param model The model pointer.
     * \param filePath The prefer output file path.
     * \param parserIndex The format index which should be get from the filter.
     * For 0 it would always be the Kreogist Mu format.
     * \return If output successfully, return true.
     */
    bool exportPlaylist(KNMusicPlaylistModel *model,
                        const QString &filePath,
                        const int &parserIndex);

    /*!
     * \brief Get whether the list of playlist is loaded.
     * \return If function loadPlaylistList() has been run once, this will
     * return true. Or else false.
     */
    bool isPlaylistListLoaded() const;

    /*!
     * \brief Create a blank playlist model.
     * \param playlistPosition The prefer position of the playlist.
     * \return The playlist model index inside the playlist list model.
     */
    QModelIndex createPlaylist(int playlistPosition=-1);

    /*!
     * \brief Generate a copy of playlist at a specific index.
     * \param index The source playlist index.
     * \return The copy playlist index.
     */
    QModelIndex copyPlaylist(const QModelIndex &index);

    /*!
     * \brief Add one playlist parser to the playlist parser engine.
     * \param parser The parser object.
     */
    void installPlaylistParser(KNMusicPlaylistParser *parser);

    /*!
     * \brief Get the playlist import filter. It will collect all the filter
     * text from the parsing engine in real-time.
     * \return The filter string list.
     */
    QStringList playlistFilter();

    /*!
     * \brief Start the playlist parsing engine thread.
     */
    void startParseEngine();

signals:
    /*!
     * \brief When the model is not empty anymore, this signal will be emitted.
     */
    void requireShowContent();

    /*!
     * \brief When the model is becoming to empty again, this signal will be
     * emitted.
     */
    void requireHideContent();

    /*!
     * \brief When a new playlist is created, sometimes it's from the list model
     * and we can have to ask the list view to enabled the line edit to rename
     * the new playlist.
     * \param index The new playlist index.
     */
    void requireShowAndRenamePlaylist(const QModelIndex &index);

public slots:

private slots:
    void onActionCreatePlaylist(const int &position);

private:
    inline bool writeModelToFile(KNMusicPlaylistModel *model,
                                 const QString &filePath);
    void savePlaylistList();
    QString generateTitle(const QString &preferName = QString());
    KNMusicPlaylistListModel *m_playlistList;
    QString m_playlistDirPath;
    bool m_isPlaylistListLoaded;
    KNMusicPlaylistEngine *m_playlistEngine;
    QThread *m_workingThread;
};

#endif // KNMUSICPLAYLISTMANAGER_H
