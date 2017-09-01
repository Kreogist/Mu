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

class KNConfigure;
class KNMusicModel;
/*!
 * \brief The KNMusicProxyModel class provides the filter and sort proxy model
 * for music models.\n
 * This model provides the filter and sort function for a music model. It is
 * used in all the music treeview widget for data sorting and filtering. It also
 * provides the ability to use one colume as category colume. This would allow
 * to use this proxy model as the model part of artist view or genre view.
 */
class KNMusicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicProxyModel object with parent object.
     * \param parent The parent object.
     */
    explicit KNMusicProxyModel(QObject *parent = 0);

    /*!
     * \brief Get the source model as a music model.
     * \return The source music model pointer. If you have never set a source
     * model, it will be nullptr.
     */
    KNMusicModel *musicModel();

    /*!
     * \brief Get whether the proxy model is displaying the search result.
     * \return If the proxy model has the search filter, return true.
     */
    bool isSearchMode() const;

    /*!
     * \brief Get a music detail info of a specific row.
     * \param row The row of the index.
     * \return The detail info structure of the row.
     */
    KNMusicDetailInfo rowDetailInfo(int row);

    /*!
     * \brief Get a music detail info of a specific row from its model index.
     * \param row The model index of the row.
     * \return The detail info structure of the row.
     */
    KNMusicDetailInfo rowDetailInfo(const QModelIndex &row);

    /*!
     * \brief Get the current set search blocks.
     * \return The set search blocks data.
     */
    QList<KNMusicSearchBlock> searchBlocks() const;

    /*!
     * \brief Remove the search blocks of the current proxy model.
     */
    void clearSearchBlock();

    /*!
     * \brief This is a function which allows you to get data much eariler. Get
     * the display text data from a specific position of the model.
     * \param row The proxy row of the position.
     * \param column The proxy column of the position.
     * \return The text data at that position.
     */
    inline QString textData(int row, int column) const
    {
        return data(index(row, column), Qt::DisplayRole).toString();
    }

    /*!
     * \brief This is a function which allows you to get data much eariler. Get
     * the data at a role from a specific row of the model.
     * \param row The row of the proxy model.
     * \param role The role of the data.
     * \return The role data of the row.
     */
    inline QVariant propertyData(int row, int role) const
    {
        return data(index(row, 0), role);
    }

    /*!
     * \brief Map a list of proxy model rows to the source model.
     * \param rowList The proxy model row list.
     */
    inline void mapRowListToSource(QList<int> &rowList)
    {
        //Translate all the items in the row list.
        for(auto i=rowList.begin(); i!=rowList.end(); ++i)
        {
            //Use mapToSource to map all the row list.
            (*i)=mapToSource(index(*i, Name)).row();
        }
    }

    /*!
     * \brief Get the column of the category.
     * \return The category column index of the model.
     */
    int categoryColumn() const;

    /*!
     * \brief Get the category filter content. This will ask the proxy model to
     * filter the category data.
     * \return The cateogry content data of the value.
     */
    QString categoryContent() const;

    /*!
     * \brief Get the identifier of the proxy model.
     * \return The proxy model identifier text.
     */
    QString identifier() const;

    /*!
     * \brief Set the identifier text data.
     * \param identifier The identifier text.
     */
    void setIdentifier(const QString &identifier);

    /*!
     * \brief Get the proxy state data of the model to JSON object.
     */
    QJsonObject proxyState();

    /*!
     * \brief Load the state of the model from JSON object.
     * \param proxyParameters The proxy data JSON object.
     */
    void loadProxyState(const QJsonObject &proxyParameters);

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
                                int row,
                                const KNMusicSearchBlock &block) const;
    QList<KNMusicSearchBlock> m_searchBlocks;
    QString m_categoryContent, m_identifier;
    int m_categoryColumn;
};

#endif // KNMUSICPROXYMODEL_H
