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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREALBUMMODEL_H
#define KNMUSICSTOREALBUMMODEL_H

#include "knmusicstoreutil.h"

#include <QAbstractTableModel>

using namespace MusicStoreUtil;

/*!
 * \brief The KNMusicStoreAlbumModel class provides the model for an album. It
 * manages a list of KNMusicStoreSongInfo. This model provides many functions to
 * instead of using setData function.
 */
class KNMusicStoreAlbumModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum AlbumModelColumns
    {
        AlbumModelIndex,
        AlbumModelName,
        AlbumModelArtist,
        AlbumModelDuration,
        AlbumModelColumnCount
    };

    /*!
     * \brief Construct a KNMusicStoreAlbumModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreAlbumModel(QObject *parent = 0);

    /*!
     * \brief Clear all the content of the song list.
     */
    void clear();

    /*!
     * \brief Set the size of the model. Before reserve the model, it must be an
     * empty model.
     * \param songCount The number of the new list.
     */
    void reserve(int songCount=0);

    /*!
     * \brief Replace one item in the list.
     * \param i The item index.
     * \param songInfo The new song information.
     */
    void replace(int i, const KNMusicStoreSongInfo &songInfo);

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::columnCount().
     */
    int columnCount(const QModelIndex &parent=QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::headerData().
     */
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief When the row count changed, this signal will be emitted.
     * \param rowCount The new row count.
     */
    void rowCountChanged(int rowCount);

public slots:

private slots:
    void retranslate();

private:
    QList<KNMusicStoreSongInfo> m_songInfoList;
    QString m_headerText[AlbumModelColumnCount];
};

#endif // KNMUSICSTOREALBUMMODEL_H
