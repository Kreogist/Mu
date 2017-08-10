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

#ifndef KNMUSICALBUMMODEL_H
#define KNMUSICALBUMMODEL_H

#include "knmusiccategorymodelbase.h"

/*!
 * \brief The KNMusicAlbumModel class is the specific category model for albums.
 * It could store all the basic information of an album.
 */
class KNMusicAlbumModel : public KNMusicCategoryModelBase
{
    Q_OBJECT
public:
    enum AlbumCategoryRole
    {
        AlbumArtistRole=CategoryArtworkKeyRole+1
    };

    /*!
     * \brief Construct a KNMusicAlbumModel object.
     * \param parent The parent object.
     */
    explicit KNMusicAlbumModel(QObject *parent = 0);

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
    /*!
     * \brief When an album item is removed from the model, this signal will be
     * emitted.
     * \param removedIndex The removed album item index.
     */
    void albumRemoved(QModelIndex removedIndex);

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

private slots:
    void retranslate();

private:
    struct AlbumItem
    {
        QVariant title;
        QStringList albumArtHash;
        QHash<QString, int> artists;
        QString albumArtist;
        int albumArtistCount, count;
        AlbumItem() :
            title(QVariant()),
            albumArtHash(QStringList()),
            artists(QHash<QString, int>()),
            albumArtist(QString()),
            albumArtistCount(0),
            count(0)
        {
        }
        inline bool operator == (const AlbumItem &value)
        {
            return title==value.title && artists==value.artists;
        }
    };

    inline AlbumItem generateNoCategoryItem();
    inline void appendItem(const AlbumItem &item);
    inline void removeItem(const int &row);
    inline void replaceItem(const int &row, const AlbumItem &item);
    QList<AlbumItem> m_categoryList;
    const QVariant m_nullData;
    QString m_noCategoryText, m_variousArtists;
    QHash<QString, QVariant> *m_hashAlbumArt;
};

#endif // KNMUSICALBUMMODEL_H
