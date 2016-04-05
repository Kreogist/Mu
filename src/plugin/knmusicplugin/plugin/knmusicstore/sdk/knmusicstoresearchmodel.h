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
#ifndef KNMUSICSTORESEARCHMODEL_H
#define KNMUSICSTORESEARCHMODEL_H

#include <QJsonArray>

#include <QAbstractTableModel>

#define SearchItemHeight 30

/*!
 * \brief The KNMusicStoreSearchSongModel class provides the model which could
 * display the search result of a music store backend in a standard treeview
 * widget.
 */
class KNMusicStoreSearchModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreSearchSongModel object.
     * \param parent The parent pointer.
     */
    explicit KNMusicStoreSearchModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;

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
     * \brief Clear the search result model data.
     */
    void clear();

    /*!
     * \brief Get the id of one search resuilt by providing its item row.
     * \param row The item row of the search result.
     * \return The item search id.
     */
    QVariant searchResultId(int row) const;

    /*!
     * \brief Get the total search count of the model.
     * \return The total items of the search result.
     */
    int totalCount() const;

signals:

public slots:
    /*!
     * \brief Set the column title field name list.
     * \param columnFieldNames The field name string list.
     */
    void setColumnFieldNames(const QStringList &columnFieldNames);

    /*!
     * \brief Set the search result data array to the model.
     * \param searchResult The search result data.
     */
    void setSearchResult(const QList<QStringList> &searchResult);

    /*!
     * \brief Set the search result item id for search result data.
     * \param searchResultId The id list.
     */
    void setSearchResultId(const QList<QVariant> &searchResultId);

    /*!
     * \brief Set the total search item count.
     * \param totalCount Total search item count.
     */
    void setTotalCount(int totalCount);

private:
    QList<QStringList> m_searchResult;
    QList<QVariant> m_searchResultId;
    QStringList m_columnFieldNames;
    int m_totalCount;
};

#endif // KNMUSICSTORESEARCHMODEL_H
