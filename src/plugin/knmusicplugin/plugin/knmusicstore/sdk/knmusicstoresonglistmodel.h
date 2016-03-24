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
#ifndef KNMUSICSTORESONGLISTMODEL_H
#define KNMUSICSTORESONGLISTMODEL_H

#include <QList>

#include "knmusicstoreutil.h"

#include <QAbstractTableModel>

#define SongItemHeight 30

/*!
 * \brief The KNMusicStoreSongListModel class provides a model which could
 * describe a list of songs information for music store only.
 */
class KNMusicStoreSongListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreSongListModel object.
     * \param parent The parent model.
     */
    explicit KNMusicStoreSongListModel(QObject *parent = 0);

    /*!
     * \brief Append a song item to the end of the model.
     * \param item The item of the song.
     */
    void appendItem(const KNMusicStoreUtil::StoreSongItem &item);

    /*!
     * \brief Clear all the data.
     */
    virtual void clear();

    /*!
     * \brief Reimplemented from QAbstractItemModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::columnCount().
     */
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

signals:

public slots:

private slots:
    void retranslate();

private:
    QString m_titles[4];
    QList<KNMusicStoreUtil::StoreSongItem> m_songList;
};

#endif // KNMUSICSTORESONGLISTMODEL_H
