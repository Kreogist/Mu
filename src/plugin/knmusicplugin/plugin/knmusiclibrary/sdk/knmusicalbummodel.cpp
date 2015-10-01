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
#include "knlocalemanager.h"

#include "knmusicglobal.h"

#include "knmusicalbummodel.h"

#include <QDebug>

KNMusicAlbumModel::KNMusicAlbumModel(QObject *parent) :
    KNMusicCategoryModelBase(parent),
    m_categoryList(QList<AlbumItem>()),
    m_nullData(QVariant()),
    m_noCategoryText(QString()),
    m_variousArtists(QString()),
    m_hashAlbumArt(nullptr)
{
    //Link retranslate signal.
    knI18n->link(this, &KNMusicAlbumModel::retranslate);
    retranslate();
}

void KNMusicAlbumModel::reset()
{
    //Check out the list size.
    if(m_categoryList.isEmpty())
    {
        //We have to insert the no category data to the first.
        generateNoCategoryItem();
        //Mission complete.
        return;
    }
    //Check out the list if the list only contains one item.
    if(m_categoryList.size()==1)
    {
        //Then it should be the no category item.
        //Reset the no category item's data.
        m_categoryList.replace(0, generateNoCategoryItem());
        //Emit data changed signal.
        emit dataChanged(index(0, 0), index(0, 0));
        //Mission complete.
        return;
    }
    //Or else the data needs to be cleared and then updated.
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), 0, m_categoryList.size()-1);
    //Remove all the data.
    m_categoryList.clear();
    //Insert the new no category item data to the item.
    m_categoryList.append(generateNoCategoryItem());
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

QModelIndex KNMusicAlbumModel::categoryIndex(const QVariant &categoryText)
{
    //Find through the list.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we find the text.
        if(m_categoryList.at(i).title==categoryText)
        {
            //Give back the index.
            return index(i, 0);
        }
    }
    //Or else return a null index.
    return QModelIndex();
}

int KNMusicAlbumModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The row count is the size of the category list size.
    return m_categoryList.size();
}

QVariant KNMusicAlbumModel::data(const QModelIndex &index, int role) const
{
    //The invalid index data connot have any valid data.
    if(!index.isValid())
    {
        //Give the null data.
        return m_nullData;
    }
    //Get the category item
    const AlbumItem &item=m_categoryList.at(index.row());
    //Get the data.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        return item.title;
    case Qt::DecorationRole:
        return (m_hashAlbumArt==nullptr || item.albumArtHash.isEmpty())?
                    m_nullData:
                    m_hashAlbumArt->value(item.albumArtHash.first(),
                                          m_nullData);
    case Qt::SizeHintRole:
        return QSize(44, 44);
    case CategorySizeRole:
        return item.count;
    case CategoryArtworkKeyRole:
        return item.albumArtHash.isEmpty()?QString():item.albumArtHash.first();
    case AlbumArtistRole:
        return item.artists.isEmpty()?
                    m_nullData:
                    (item.artists.size()==1?
                         item.artists.keys().first():
                         m_variousArtists);
    default:
        return m_nullData;
    }
}

Qt::ItemFlags KNMusicAlbumModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
}

QVariant KNMusicAlbumModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    //No matter what, just simply return a null variant.
    return m_nullData;
}

QString KNMusicAlbumModel::noCategoryText() const
{
    return m_noCategoryText;
}

bool KNMusicAlbumModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    //Check the valid of the index first.
    if(!index.isValid())
    {
        return false;
    }
    //Get the category item.
    AlbumItem albumItem=m_categoryList.at(index.row());
    //Change the data according to the index.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Change the display text.
        albumItem.title=value;
        //Replace the item.
        m_categoryList.replace(index.row(), albumItem);
        //Emit data changed signal.
        emit dataChanged(index, index, QVector<int>(1, role));
        return true;
    case CategorySizeRole:
        //Change the size.
        albumItem.count=value.toInt();
        //Replace the item.
        m_categoryList.replace(index.row(), albumItem);
        //Emit the data changed signal.
        emit dataChanged(index, index, QVector<int>(1, role));
        return true;
    default:
        return false;
    }
}

int KNMusicAlbumModel::categoryColumn() const
{
    return Album;
}

void KNMusicAlbumModel::setHashAlbumArt(QHash<QString, QVariant> *hashAlbumArt)
{
    m_hashAlbumArt=hashAlbumArt;
}

void KNMusicAlbumModel::setCategoryColumn(int categoryColumn)
{
    //We won't use any other category column, this is album specific.
    Q_UNUSED(categoryColumn)
}

void KNMusicAlbumModel::setNoAlbumArt(const QPixmap &noAlbumArt)
{
    //Ignore the no album art.
    Q_UNUSED(noAlbumArt)
}

void KNMusicAlbumModel::setNoCategoryText(const QString &noCategoryText)
{
    //Save the text first.
    m_noCategoryText=noCategoryText;
    //Check the list.
    if(m_categoryList.isEmpty())
    {
        //The category list is not initialized.
        //We have to insert the no category data to the first.
        appendItem(generateNoCategoryItem());
        //Mission complete.
        return;
    }
    //Get the first item in the list.
    AlbumItem noCategory=m_categoryList.at(0);
    //Change the text.
    noCategory.title=m_noCategoryText;
    //Replace the item.
    m_categoryList.replace(0, noCategory);
    //Emit the data changed signal.
    emit dataChanged(index(0, 0),
                     index(0, 0),
                     QVector<int>(1, Qt::DisplayRole));
}

void KNMusicAlbumModel::onCategoryAdd(const KNMusicDetailInfo &detailInfo)
{
    //Get the category text.
    QVariant titleText=detailInfo.textLists[Album];
    QString artistText=detailInfo.textLists[Artist].toString();
    //Check if the category is blank.
    if(titleText.toString().isEmpty())
    {
        //Get the category list data.
        AlbumItem item=m_categoryList.at(0);
        //Increase the count.
        ++(item.count);
        //Add count of no category item.
        replaceItem(0, item);
        //Mission complete.
        return;
    }
    //Find the category text.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we could find the item.
        if(m_categoryList.at(i).title==titleText &&
                m_categoryList.at(i).artists.contains(artistText))
        {
            //Get the category list data.
            AlbumItem item=m_categoryList.at(i);
            //Increase the count.
            ++(item.count);
            //Check out the cover image hash.
            if(!detailInfo.coverImageHash.isEmpty())
            {
                //Add the hash string to the category.
                item.albumArtHash.append(detailInfo.coverImageHash);
            }
            //Add the artist to the list.
            item.artists.insert(artistText,
                                item.artists.value(artistText, 0)+1);
            //Replace the item.
            replaceItem(i, item);
            //Mission complete.
            return;
        }
    }
    //We need to generate a new item for it.
    AlbumItem item;
    item.title=titleText;
    item.count=1;
    //Check out the hash key.
    if(!detailInfo.coverImageHash.isEmpty())
    {
        //Add the cover image hash to the category.
        item.albumArtHash.append(detailInfo.coverImageHash);
    }
    //Add the artist to the list.
    item.artists.insert(artistText, 1);
    //Append the item.
    appendItem(item);
}

void KNMusicAlbumModel::onCategoryRemove(const KNMusicDetailInfo &detailInfo)
{
    //Get the category text.
    QVariant titleText=detailInfo.textLists[Album];
    QString artistText=detailInfo.textLists[Artist].toString();
    //Check if the category is blank.
    if(titleText.toString().isEmpty())
    {
        //Reduce count of the no category item.
        //Get the category list data.
        AlbumItem item=m_categoryList.at(0);
        //Increase the item counter.
        --(item.count);
        //Check out the item count.
        if(item.count==0)
        {
            //Emit the album removed signal.
            emit albumRemoved(index(0));
        }
        //Replace the original item.
        m_categoryList.replace(0, item);
        //Emit the data changed signal.
        emit dataChanged(index(0), index(0), QVector<int>(1, CategorySizeRole));
        //Mission complete.
        return;
    }
    //Find the category text.
    for(int i=0, bound=m_categoryList.size(); i<bound; ++i)
    {
        //If we could find the item.
        if(m_categoryList.at(i).title==titleText)
        {
            //Check out the counter.
            if(m_categoryList.at(i).count==1)
            {
                //Emit the album removed signal.
                emit albumRemoved(index(i));
                //This is the last item, we will remove this category.
                removeItem(i);
                //Finished.
                return;
            }
            //Get the category list data.
            AlbumItem item=m_categoryList.at(i);
            //Decrease the count.
            --(item.count);
            //Remove the artist from the list.
            //Check the song count.
            int artistSongCount=item.artists.value(artistText);
            //If this the last one
            if(artistSongCount<=1)
            {
                //Remove the artist from the has list.
                item.artists.remove(artistText);
            }
            else
            {
                //Reduce the artist count.
                item.artists.insert(artistText,
                                    item.artists.value(artistText, 0)-1);
            }
            //Replace the item.
            replaceItem(i, item);
            //Mission complete.
            return;
        }
    }
    //If we can get here, there's something wrong, ignore it.
}

void KNMusicAlbumModel::onCategoryUpdate(const KNMusicDetailInfo &before,
                                         const KNMusicDetailInfo &after)
{
    //Check out the category item have ever changed.
    if(before.textLists[Album]==after.textLists[Album])
    {
        //We won't need to add or reduce count, but we have to check two things:
        // 1. Whether the cover image needs to be updated.
        // 2. Whether the artist needs to be updated.
        bool coverImageUpdate=after.coverImageHash!=before.coverImageHash,
             artistUpdate=after.textLists[Artist]!=before.textLists[Artist];
        //If any thing we need te bo updated, go through and update the item.
        if(coverImageUpdate || artistUpdate)
        {
            //Get the category text.
            QVariant albumText=after.textLists[Album];
            //Find the category item.
            for(int i=0; i<m_categoryList.size(); ++i)
            {
                //If we could find the item.
                if(m_categoryList.at(i).title==albumText)
                {
                    //Get the category item.
                    AlbumItem item=m_categoryList.at(i);
                    //If the cover image update flag is on,
                    if(coverImageUpdate)
                    {
                        //Remove the original cover image hash from the item.
                        item.albumArtHash.removeOne(before.coverImageHash);
                        //Insert the new cover image.
                        item.albumArtHash.append(after.coverImageHash);
                    }
                    //If the artist update flag is on,
                    if(artistUpdate)
                    {
                        //Get the previous artist.
                        QString artistText=before.textLists[Artist].toString();
                        //Remove the previous artist from the list.
                        //Check the song count.
                        int artistSongCount=item.artists.value(artistText);
                        //If this the last one
                        if(artistSongCount<=1)
                        {
                            //Remove the artist from the has list.
                            item.artists.remove(artistText);
                        }
                        else
                        {
                            //Reduce the artist count.
                            item.artists.insert(artistText,
                                                item.artists.value(artistText,
                                                                   0)-1);
                        }
                        //Get the after artist.
                        artistText=after.textLists[Artist].toString();
                        //Add the artist to the list.
                        item.artists.insert(artistText,
                                            item.artists.value(artistText,
                                                               0)+1);
                    }
                    //Replace the one.
                    replaceItem(i, item);
                    //Mission complete.
                    return;
                }
            }
        }
        //Nothing changed, we will do nothing.
        //Mission complete.
        return;
    }
    //Or else, we have to remove the previous data.
    onCategoryRemove(before);
    //And then add the after category.
    onCategoryAdd(after);
}

void KNMusicAlbumModel::onCategoryAlbumArtUpdate(
        const KNMusicDetailInfo &detailInfo)
{
    //Check out the artwork key hash first.
    if(detailInfo.coverImageHash.isEmpty())
    {
        return;
    }
    //Get the album text.
    QVariant albumTitle=detailInfo.textLists[Album];
    //Check if album title is blank.
    if(albumTitle.toString().isEmpty())
    {
        //Mission complete, no artist don't need anything.
        return;
    }
    //Find the category text.
    for(int i=0; i<m_categoryList.size(); ++i)
    {
        //If we could find the item.
        if(m_categoryList.at(i).title==albumTitle)
        {
            //Get the category list data.
            AlbumItem item=m_categoryList.at(i);
            //Add the hash string to the category.
            item.albumArtHash.append(detailInfo.coverImageHash);
            //Replace the item.
            replaceItem(i, item);
            //Emit cover image update signal.
            emit albumArtUpdate(index(i));
            //Mission complete.
            return;
        }
    }
}

void KNMusicAlbumModel::onActionImageRecoverComplete()
{
    //Well, with the brand new instructures, we won't need to save or change
    //anything.
    //The only thing we have to do is emit a data changed signal.
    emit dataChanged(index(0),
                     index(rowCount()-1),
                     QVector<int>(1, Qt::DecorationRole));
}

void KNMusicAlbumModel::retranslate()
{
    //Update the text.
    m_variousArtists=tr("Various Artists");
}

inline KNMusicAlbumModel::AlbumItem KNMusicAlbumModel::generateNoCategoryItem()
{
    //Generate the no category item.
    AlbumItem noAlbum;
    //Set no album title.
    noAlbum.title=m_noCategoryText;
    //Give back the no category item.
    return noAlbum;
}

inline void KNMusicAlbumModel::appendItem(const AlbumItem &item)
{
    //Follow the documentation, we have to do this.
    beginInsertRows(QModelIndex(),
                    m_categoryList.size(),
                    m_categoryList.size());
    //Append this data at the end of the list.
    m_categoryList.append(item);
    //End the insertation.
    endInsertRows();
}

inline void KNMusicAlbumModel::removeItem(const int &row)
{
    //As the documentation said, called this function first.
    beginRemoveRows(QModelIndex(), row, row);
    //Remove the data in the category list.
    m_categoryList.removeAt(row);
    //As the documentation said, called this after remove rows.
    endRemoveRows();
}

inline void KNMusicAlbumModel::replaceItem(const int &row,
                                           const AlbumItem &item)
{
    //Replace the original item.
    m_categoryList.replace(row, item);
    //Emit the data changed signal.
    emit dataChanged(index(row), index(row));
}
