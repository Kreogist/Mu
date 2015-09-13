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

#ifndef KNMUSICPLAYLISTLISTMODEL_H
#define KNMUSICPLAYLISTLISTMODEL_H

#include <QList>
#include <QIcon>

#include <QAbstractListModel>

class KNMusicPlaylistModel;
/*!
 * \brief The KNMusicPlaylistListModel class is a manager of
 * KNMusicPlaylsitModel. It's model itself and it can manage those models with
 * MVC model. It provides such basic functions like append and remove for fast
 * management.
 */
class KNMusicPlaylistListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistListModel object with parent object.
     * \param parent The parent object.
     */
    explicit KNMusicPlaylistListModel(QObject *parent = 0);
    ~KNMusicPlaylistListModel();

    /*!
     * \brief Get a playlist model according to the index.
     * \param index The index of the playlist model.
     * \return If the index is invalid, this will return nullptr. Or else the
     * playlist model pointer.
     */
    KNMusicPlaylistModel *playlist(const QModelIndex &index);

    /*!
     * \brief This is an override function. Get a playlist model according to
     * the row.
     * \param row The row of the playlist.
     * \return If the index is invalid, this will return nullptr. Or else the
     * playlist model pointer.
     */
    inline KNMusicPlaylistModel *playlist(int row)
    {
        return playlist(index(row, 0));
    }

    /*!
     * \brief Find the playlist model inside the playlist list model.
     * \param playlist The playlist model pointer.
     * \return The playlist index. If the playlist model is not exist, return a
     * invalid index.
     */
    QModelIndex playlistIndex(KNMusicPlaylistModel *playlist);

    /*!
     * \brief Append a playlist model at the end of the list.
     * \param model The playlist model which is wait to add.
     * \return The index of the playlist model inside the model. If the model is
     * already in the model, it will return the model index where previously
     * exist, and won't add it again. If you are trying to add a nullptr, it
     * will return a null index and won't add the model into the list.
     */
    QModelIndex append(KNMusicPlaylistModel *model);

    /*!
     * \brief Remove a playlist model from the list.
     * \param model The model pointer.
     * \return If the model never add to the list model, return false. Or else,
     * return the result of delete the model from the list. If the model pointer
     * is nullptr, it will always return a false.
     */
    bool removeModel(KNMusicPlaylistModel *model);

    /*!
     * \brief Insert a playlist model into a specific position.
     * \param row The row where you'd like to insert the model.
     * \param model The model pointer.
     * \return The model index in the list.
     */
    QModelIndex insert(int row, KNMusicPlaylistModel *model);

    /*!
     * \brief Reimplemented from QAbstractListModel::removeRows().
     */
    bool removeRows(int row,
                    int count,
                    const QModelIndex &parent) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::flags().
     */
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::setData().
     */
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /*!
     * \brief Check all the title in the list, if we can find a playlist model
     * which is named as the title, this function will return true.
     * \param title The title text.
     * \return If we can find the title in list, it will return true.
     */
    bool findTitle(const QString &title);

    /*!
     * \brief Reimplemented from QAbstractListModel::mimeTypes().
     */
    QStringList mimeTypes() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::canDropMimeData().
     */
    bool canDropMimeData(const QMimeData *data,
                         Qt::DropAction action,
                         int row,
                         int column,
                         const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::dropMimeData().
     */
    bool dropMimeData(const QMimeData *data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex &parent) Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief When the list is not empty any more, this signal will be emitted.
     */
    void requireShowContent();

    /*!
     * \brief When the list is empty again, this signal will be emitted.
     */
    void requireHideContent();

    /*!
     * \brief When the user dropped the data and the user want to create a new
     * playlist at a specific position, this signal will emitted.
     * \param position The prefer position.
     */
    void requireCreatePlaylist(int position);

public slots:

protected:

private:
    QList<KNMusicPlaylistModel *> m_playlistList;
    QIcon m_icon;
    QVariant m_itemSize;

    static QStringList m_dropMimeTypes;
};

#endif // KNMUSICPLAYLISTLISTMODEL_H
