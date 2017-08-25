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
#include <QJsonArray>

#include "knconfigure.h"

#include "knmusicmodel.h"

#include "knmusicproxymodel.h"

#include <QDebug>

KNMusicProxyModel::KNMusicProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_categoryColumn(-1)
{
    //Set properties.
    setFilterKeyColumn(-1); //Search for all columns.
    setFilterCaseSensitivity(Qt::CaseInsensitive); //Filter won't care the case.
    setSortCaseSensitivity(Qt::CaseInsensitive); //Sorter will care the case.
}

KNMusicModel *KNMusicProxyModel::musicModel()
{
    //This function is simply recast the source model as a music model.
    return static_cast<KNMusicModel *>(sourceModel());
}

bool KNMusicProxyModel::isSearchMode() const
{
    //Check out the search rule list.
    return !m_searchBlocks.isEmpty();
}

KNMusicDetailInfo KNMusicProxyModel::rowDetailInfo(int row)
{
    //Check the music model. If it's null, return a null detail info.
    if(musicModel()==nullptr)
    {
        //Give back a null detail info.
        return KNMusicDetailInfo();
    }
    //Return the specific detail info.
    return musicModel()->rowDetailInfo(mapToSource(index(row, Name)).row());
}

KNMusicDetailInfo KNMusicProxyModel::rowDetailInfo(const QModelIndex &index)
{
    //Check the music model. If it's null, return a null detail info.
    if(musicModel()==nullptr)
    {
        return KNMusicDetailInfo();
    }
    //Return the specific detail info.
    return musicModel()->rowDetailInfo(mapToSource(index).row());
}

QList<KNMusicSearchBlock> KNMusicProxyModel::searchBlocks() const
{
    //Give back the search block list.
    return m_searchBlocks;
}

void KNMusicProxyModel::clearSearchBlock()
{
    //Clear all the search blocks.
    m_searchBlocks.clear();
    //Set a filter text to update the whole proxy model.
    setFilterFixedString("");
}

void KNMusicProxyModel::setSearchBlocks(
        const QList<KNMusicSearchBlock> &blockList)
{
    //Save the blocks.
    m_searchBlocks=blockList;
    //Set a filter text to update the whole proxy model.
    setFilterFixedString("");
}

bool KNMusicProxyModel::lessThan(const QModelIndex &source_left,
                                 const QModelIndex &source_right) const
{
    //Check out the column.
    switch(source_left.column())
    {
    //For the time role, we have to get the total duration of the row.
    case Time:
    {
        //Get the duration role.
        qlonglong &&leftData=source_left.data(DurationRole).toLongLong(),
                  &&rightData=source_right.data(DurationRole).toLongLong();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    case Size:
    {
        //Get the file size role.
        qlonglong &&leftData=source_left.data(FileSizeRole).toLongLong(),
                &&rightData=source_right.data(FileSizeRole).toLongLong();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    case DiscNumber:
    case DiscCount:
    case TrackNumber:
    case TrackCount:
    {
        //Get the display role, translate it into an integer.
        int &&leftData=source_left.data(Qt::DisplayRole).toString().toInt(),
            &&rightData=source_right.data(Qt::DisplayRole).toString().toInt();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    case DateAdded:
    {
        //Get the date added role, translate it into date time.
        QDateTime &&leftData=source_left.data(DateAddedRole).toDateTime(),
                  &&rightData=source_right.data(DateAddedRole).toDateTime();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    case DateModified:
    {
        //Get the date modified role, translate it into date time.
        QDateTime &&leftData=source_left.data(DateModifiedRole).toDateTime(),
                  &&rightData=source_right.data(DateModifiedRole).toDateTime();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    case LastPlayed:
    {
        //Get the date last played role, translate it into date time.
        QDateTime &&leftData=source_left.data(DateLastPlayedRole).toDateTime(),
                 &&rightData=source_right.data(DateLastPlayedRole).toDateTime();
        //Compare the size, if they are the same, then the adding order is the
        //right order.
        return (leftData == rightData) ?
                    (source_left.row() < source_right.row()) :
                    (leftData < rightData);
    }
    }
    //Get the display role, translate it into text.
    QString &&leftData=source_left.data(Qt::DisplayRole).toString(),
            &&rightData=source_right.data(Qt::DisplayRole).toString();
    //Compare the size, if they are the same, then the adding order is the
    //right order.
    return (leftData == rightData) ?
                (source_left.row() < source_right.row()) :
                (leftData < rightData);
}

bool KNMusicProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    //Get the source model.
    QAbstractItemModel *model=sourceModel();
    //Check the validation of category column.
    if(m_categoryColumn!=-1)
    {
        //Check out the category column data.
        if(model->index(source_row,
                        m_categoryColumn).data(Qt::DisplayRole).toString() !=
                m_categoryContent)
        {
            //Abandon the data which didn't match the category content.
            return false;
        }
    }
    //Check the model is null or not, treat the null tree view as accept all
    //rows. Check the block list is empty or not.
    if(m_searchBlocks.isEmpty() || model==nullptr)
    {
        //All the row will become accept if there's no blocks filter.
        return true;
    }
    //Check if the row comply with the limits.
    for(auto i : m_searchBlocks)
    {
        //Check the the rule, if the row can not match one rule, then it cannot
        //be accept.
        if(!checkRule(model, source_row, i))
        {
            return false;
        }
    }
    return true;
}

inline bool KNMusicProxyModel::checkRule(QAbstractItemModel *model,
                                         int row,
                                         const KNMusicSearchBlock &block) const
{
    //Check if this block a common search block,
    if(block.index==-1)
    {
        //Translate the format of the search block data to QString.
        QString &&ruleText=block.value.toString();
        //Search the text in all column.
        for(int i=0; i<MusicColumnCount; i++)
        {
            //Search the rule text in the whole data.
            if(model->index(row, i).data(Qt::DisplayRole).toString().
                    contains(ruleText, Qt::CaseInsensitive))
            {
                //Once match, then it should be true.
                return true;
            }
        }
        return false;
    }
    //Or else, check the specific column.
    if(block.isColumn)
    {
        //Check the block index, for most of the blocks, simply match the string
        //in the text.
        switch(block.index)
        {
        default:
            return model->data(model->index(row, block.index), Qt::DisplayRole)
                    .toString().contains(block.value.toString(),
                                         Qt::CaseInsensitive);
        }
    }
    //So now it should be a property of the data.
    //Get the data from the Name column.
    QVariant &&propertyData=model->data(model->index(row, 0), block.index);
    //Check the data.
    switch(block.index)
    {
    default:
        return propertyData.toString().contains(block.value.toString(),
                                                Qt::CaseInsensitive);
    }
}

QString KNMusicProxyModel::identifier() const
{
    return m_identifier;
}

void KNMusicProxyModel::setIdentifier(const QString &identifier)
{
    m_identifier = identifier;
}

void KNMusicProxyModel::saveState(KNConfigure *configure)
{
    //Save all the configure data.
    configure->setData("ProxyModel", m_identifier);
    //Then save the proxy model data.
    QJsonObject proxyParameters;
    proxyParameters.insert("Type", "CustomObject");
    // Categroy column.
    proxyParameters.insert("ProxyCategoryColumn", m_categoryColumn);
    // Categroy column content.
    proxyParameters.insert("ProxyCategoryContent", m_categoryContent);
    // Search content.
    QJsonArray searchBlocks;
    for(auto i : m_searchBlocks)
    {
        //Construct the block data.
        QJsonObject blockData;
        //Save the block data.
        blockData.insert("Index", i.index);
        blockData.insert("Column", i.isColumn);
        blockData.insert("Value", i.value.toString());
        //Append the block data.
        searchBlocks.append(blockData);
    }
    proxyParameters.insert("ProxySearchBlocks", searchBlocks);
    configure->setData("ProxyModelData", proxyParameters);
}

void KNMusicProxyModel::loadState(KNConfigure *configure)
{
    ;
}

QString KNMusicProxyModel::categoryContent() const
{
    //Get the content of the category.
    return m_categoryContent;
}

void KNMusicProxyModel::setCategoryContent(const QString &categoryContent)
{
    //Save the category content.
    m_categoryContent = categoryContent;
    //Set a filter text to update the whole proxy model.
    setFilterFixedString("");
}

int KNMusicProxyModel::categoryColumn() const
{
    return m_categoryColumn;
}

void KNMusicProxyModel::setCategoryColumn(int categoryColumn)
{
    //Save the categroy column.
    m_categoryColumn = categoryColumn;
    //Set a filter text to update the whole proxy model.
    setFilterFixedString("");
}
