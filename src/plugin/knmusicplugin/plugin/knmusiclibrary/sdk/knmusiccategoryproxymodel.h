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

#ifndef KNMUSICCATEGORYPROXYMODEL_H
#define KNMUSICCATEGORYPROXYMODEL_H

#include "knmusicutil.h"

#include <QSortFilterProxyModel>

using namespace MusicUtil;

/*!
 * \brief The KNMusicCategoryProxyModel class provides the sort, filter and
 * search function to a category model.
 */
class KNMusicCategoryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construxt a KNMusicCategoryProxyModel object.
     * \param parent The parent object.
     */
    explicit KNMusicCategoryProxyModel(QObject *parent = 0);

    /*!
     * \brief Get the proxy category index from the text.
     * \param categoryText The category text.
     * \return The proxy category item index.
     */
    QModelIndex categoryIndex(const QVariant &categoryText);

    /*!
     * \brief Get the search item blocks.
     * \return The search item block list.
     */
    QList<KNMusicSearchBlock> searchBlocks() const;

signals:

public slots:
    /*!
     * \brief Set the search block list to filter the music model.
     * \param blockList The search block list.
     */
    void setSearchBlocks(const QList<KNMusicSearchBlock> &blockList);

    /*!
     * \brief Remove all the search blocks in the list.
     */
    void clearSearchBlocks();

protected:
    /*!
     * \brief Reimplemented from QSortFilterProxyModel::lessThan().
     */
    bool lessThan(const QModelIndex &source_left,
                  const QModelIndex &source_right) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QSortFilterProxyModel::filterAcceptsRow().
     */
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const
    Q_DECL_OVERRIDE;

private:
    QList<KNMusicSearchBlock> m_searchBlocks;
};

#endif // KNMUSICCATEGORYPROXYMODEL_H
