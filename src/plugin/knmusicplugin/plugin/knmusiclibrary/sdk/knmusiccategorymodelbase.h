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

#ifndef KNMUSICCATEGORYMODELBASE_H
#define KNMUSICCATEGORYMODELBASE_H

#include "knmusicutil.h"

#include <QAbstractListModel>

using namespace MusicUtil;

class KNMusicCategoryModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CategoryRole
    {
        CategorySizeRole=Qt::UserRole,
        CategoryArtworkKeyRole
    };
    /*!
     * \brief Construct a KNMusicCategoryModelBase object.
     * \param parent The parent object.
     */
    KNMusicCategoryModelBase(QObject *parent = 0):QAbstractListModel(parent){}

    /*!
     * \brief Reset the category model. This will add the no category item to
     * the model.
     */
    virtual void reset()=0;

    /*!
     * \brief Get the index of a specific category index.
     * \param categoryText The text data of the category.
     * \return The index of the category text from the model.
     */
    virtual QModelIndex categoryIndex(const QVariant &categoryText)=0;

    /*!
     * \brief Get the no category text.
     * \return The no category text.
     */
    virtual QString noCategoryText() const=0;

    /*!
     * \brief Get the category column.
     * \return The category column number.
     */
    virtual int categoryColumn() const=0;

    /*!
     * \brief Set the library hash album art for the category model.
     * \param hashAlbumArt The album art hash list.
     */
    virtual void setHashAlbumArt(QHash<QString, QVariant> *hashAlbumArt)=0;

signals:
    /*!
     * \brief When the album art of the index is updated, this signal will be
     * emitted.
     * \param index The specific index.
     */
    void albumArtUpdate(QModelIndex index);

public slots:
    /*!
     * \brief Set the category column number, it will be used to category the
     * detail info.
     * \param categoryColumn The category column.
     */
    virtual void setCategoryColumn(int categoryColumn)=0;

    /*!
     * \brief Set the no album art icon.
     * \param noAlbumArt The no album art icon.
     */
    virtual void setNoAlbumArt(const QPixmap &noAlbumArt)=0;

    /*!
     * \brief Set no category text of the first data.
     * \param noCategoryText
     */
    virtual void setNoCategoryText(const QString &noCategoryText)=0;

    /*!
     * \brief When a new detail info is adding to the library, called this
     * function to add it to cateogry model.
     * \param detailInfo The detail info added to library model.
     */
    virtual void onCategoryAdd(const KNMusicDetailInfo &detailInfo)=0;

    /*!
     * \brief When a detail info is removing from the library, called this
     * function right before it's being removed to remove it from the category
     * model.
     * \param detailInfo The detail info which is going to be removed.
     */
    virtual void onCategoryRemove(const KNMusicDetailInfo &detailInfo)=0;

    /*!
     * \brief When a detail info is being updated, called this function when it
     * is being update to keep the category model updated.
     * \param before The detail info before updating.
     * \param after The latest detail info.
     */
    virtual void onCategoryUpdate(const KNMusicDetailInfo &before,
                                  const KNMusicDetailInfo &after)=0;

    /*!
     * \brief When a detail info was being update because of the updating of the
     * album art key hash, call this function to update the category album art
     * in the category model.\n
     * This function will be used only when the image hash key of the previous
     * detail info is empty.
     * \param detailInfo The detail info with new category album art.
     */
    virtual void onCategoryAlbumArtUpdate(const KNMusicDetailInfo &detailInfo)
    =0;

    /*!
     * \brief When the image hash list was built complete, called this function
     * to update the category model.
     */
    virtual void onActionImageRecoverComplete()=0;
};

#endif // KNMUSICCATEGORYMODELBASE_H
