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

#ifndef KNMUSICCATEGORYMODEL_H
#define KNMUSICCATEGORYMODEL_H

#include <QList>

#include "knmusiccategorymodelbase.h"

class KNMusicCategoryModel : public KNMusicCategoryModelBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicCategoryModel with a given parent object.
     * \param parent The parent object pointer.
     */
    explicit KNMusicCategoryModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::categoryIndex().
     */
    QModelIndex categoryIndex(const QVariant &categoryText) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::flags().
     */
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::noCategoryText().
     */
    QString noCategoryText() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::setData().
     */
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::categoryColumn().
     */
    int categoryColumn() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::setHashAlbumArt().
     */
    void setHashAlbumArt(QHash<QString, QVariant> *hashAlbumArt)
    Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::setCategoryColumn().
     */
    void setCategoryColumn(int categoryColumn) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::setNoAlbumArt().
     */
    void setNoAlbumArt(const QPixmap &noAlbumArt) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::setNoCategoryText().
     */
    void setNoCategoryText(const QString &noCategoryText) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::onCategoryAdd().
     */
    void onCategoryAdd(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::onCategoryRemove().
     */
    void onCategoryRemove(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryModelBase::onCategoryUpdate().
     */
    void onCategoryUpdate(const KNMusicDetailInfo &before,
                          const KNMusicDetailInfo &after) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMusicCategoryModelBase::onCategoryAlbumArtUpdate().
     */
    void onCategoryAlbumArtUpdate(const KNMusicDetailInfo &detailInfo)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from
     * KNMusicCategoryModelBase::onActionImageRecoverComplete().
     */
    void onActionImageRecoverComplete() Q_DECL_OVERRIDE;

protected:
    struct CategoryItem
    {
        QVariant displayText;
        QStringList albumArtHash;
        int count;
        CategoryItem() :
            displayText(QVariant()),
            albumArtHash(QStringList()),
            count(0)
        {
        }
        inline bool operator == (const CategoryItem &value)
        {
            return displayText==value.displayText;
        }
    };
    QVariant noAlbumArt() const;
    CategoryItem itemAt(const int &row) const;
    CategoryItem generateNoCategoryItem();
    void appendItem(const CategoryItem &item);
    void removeItem(const int &row);
    void replaceItem(const int &row, const CategoryItem &item);
    void reduceCount(const int &row);

private:
    inline void saveNoAlbumArt(const QPixmap &noAlbumArt);
    QList<CategoryItem> m_categoryList;
    QVariant m_noAlbumArt;
    QString m_noCategoryText;
    QHash<QString, QVariant> *m_hashAlbumArt;
    int m_categoryColumn;
};

#endif // KNMUSICCATEGORYMODEL_H
