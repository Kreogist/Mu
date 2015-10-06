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

#ifndef KNMUSICPROXYMODEL_H
#define KNMUSICPROXYMODEL_H

#include "knmusicutil.h"

#include <QSortFilterProxyModel>

using namespace MusicUtil;

class KNMusicModel;
class KNMusicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicProxyModel(QObject *parent = 0);
    KNMusicModel *musicModel();
    bool isSearchMode() const;

    KNMusicDetailInfo rowDetailInfo(int row);

    KNMusicDetailInfo rowDetailInfo(const QModelIndex &row);

    QList<KNMusicSearchBlock> searchBlocks() const;

    void clearSearchBlock();

    inline QString textData(int row, int column) const
    {
        return data(index(row, column), Qt::DisplayRole).toString();
    }

    inline QVariant propertyData(int row, int role) const
    {
        return data(index(row, 0), role);
    }

    inline void mapRowListToSource(QList<int> &rowList)
    {
        //Translate all the items in the row list.
        for(auto i=rowList.begin(); i!=rowList.end(); ++i)
        {
            //Use mapToSource to map all the row list.
            (*i)=mapToSource(index(*i, Name)).row();
        }
    }

    int categoryColumn() const;

    QString categoryContent() const;

signals:

public slots:
    /*!
     * \brief Set the search block list to filter the music model.
     * \param blockList The search block list.
     */
    void setSearchBlocks(const QList<KNMusicSearchBlock> &blockList);

    /*!
     * \brief setCategoryContent
     * \param categoryContent
     */
    void setCategoryContent(const QString &categoryContent);

    /*!
     * \brief setCategoryColumn
     * \param categoryColumn
     */
    void setCategoryColumn(int categoryColumn);

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
    inline bool checkRule(QAbstractItemModel *model,
                                const int &row,
                                const KNMusicSearchBlock &block) const;
    QList<KNMusicSearchBlock> m_searchBlocks;
    int m_categoryColumn;
    QString m_categoryContent;
};

#endif // KNMUSICPROXYMODEL_H
