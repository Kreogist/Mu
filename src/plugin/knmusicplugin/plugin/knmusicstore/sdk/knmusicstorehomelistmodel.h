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

#ifndef KNMUSICSTOREHOMELISTMODEL_H
#define KNMUSICSTOREHOMELISTMODEL_H

#include "knmusicstoreutil.h"

#include <QAbstractListModel>

using namespace MusicStoreUtil;

/*!
 * \brief The KNMusicStoreHomeListModel class provides the item list used in
 * music store home page.
 */
class KNMusicStoreHomeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreHomeListModel object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreHomeListModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Get the album art directly from the list.
     * \param row The item row.
     * \return The pixmap data.
     */
    QPixmap albumArt(int row) const;

    /*!
     * \brief Get the album title text from the list.
     * \param row The item row.
     * \return The title text.
     */
    QString albumTitle(int row) const;

    /*!
     * \brief Append one music item to the model.
     * \param homeItem The music item.
     */
    void appendRow(const KNMusicStoreHomeItem &homeItem);

signals:

public slots:
    /*!
     * \brief Clear all the data of the model.
     */
    void reset();

private:
    QList<KNMusicStoreHomeItem> m_itemList;
};

#endif // KNMUSICSTOREHOMELISTMODEL_H
