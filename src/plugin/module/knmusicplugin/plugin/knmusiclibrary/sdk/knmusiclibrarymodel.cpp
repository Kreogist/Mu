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
#include <QThread>

#include "knhashpixmaplist.h"
#include "knjsondatabase.h"
#include "knglobal.h"

#include "knmusicmodelassist.h"
#include "knmusiclibraryanalysisextend.h"
#include "knmusiclibraryimagemanager.h"

#include "knmusiclibrarymodel.h"

#include <QDebug>

KNMusicLibraryModel::KNMusicLibraryModel(QObject *parent) :
    KNMusicModel(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Initial header.
    initialHeader();
    //Initial the pixmap list.
    m_coverImageList=new KNHashPixmapList(this);
    //Reset the analysis extend.
    m_analysisExtend=new KNMusicLibraryAnalysisExtend;
    m_analysisExtend->setCoverImageList(m_coverImageList);
    connect(m_analysisExtend, &KNMusicLibraryAnalysisExtend::requireUpdateImage,
            this, &KNMusicLibraryModel::updateCoverImage);
    connect(m_analysisExtend, &KNMusicLibraryAnalysisExtend::requireAppendLibraryRow,
            this, &KNMusicLibraryModel::appendLibraryMusicRow);
    setAnalysisExtend(m_analysisExtend);

    //Connect language changed request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicLibraryModel::retranslate);
}

Qt::DropActions KNMusicLibraryModel::supportedDropActions() const
{
    return Qt::IgnoreAction;
}

Qt::ItemFlags KNMusicLibraryModel::flags(const QModelIndex &index) const
{
    return (index.column()==BlankData)?
                (Qt::ItemIsSelectable |
                 Qt::ItemIsDragEnabled |
                 Qt::ItemIsEnabled |
                 Qt::ItemNeverHasChildren):
                KNMusicModel::flags(index);
}

QPixmap KNMusicLibraryModel::artwork(const QString &key)
{
    return m_coverImageList->pixmap(key);
}

int KNMusicLibraryModel::rowFromFilePath(const QString &filePath)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    FilePathRole,
                                    filePath,
                                    1,
                                    Qt::MatchFixedString);
    //If we can't find it, return -1.
    if(fileCheck.isEmpty())
    {
        return -1;
    }
    return fileCheck.first().row();
}

int KNMusicLibraryModel::rowFromDetailInfo(const KNMusicDetailInfo &detailInfo)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    FilePathRole,
                                    detailInfo.filePath,
                                    -1,
                                    Qt::MatchFixedString);
    //If we can't find it, return -1.
    if(fileCheck.isEmpty())
    {
        return -1;
    }
    //Check the start position is the same as the detail info.
    for(QModelIndexList::iterator i=fileCheck.begin();
        i!=fileCheck.end();
        ++i)
    {
        if(rowProperty((*i).row(), StartPositionRole).toLongLong()==
                detailInfo.startPosition)
        {
            return (*i).row();
        }
    }
    //If we are here, means find nothing.
    return -1;
}

int KNMusicLibraryModel::playingItemColumn()
{
    return BlankData;
}

bool KNMusicLibraryModel::dropMimeData(const QMimeData *data,
                                       Qt::DropAction action,
                                       int row,
                                       int column,
                                       const QModelIndex &parent)
{
    Q_UNUSED(data)
    Q_UNUSED(action)
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(parent)
    return false;
}

void KNMusicLibraryModel::setRowProperty(const int &row,
                                         const int &propertyRole,
                                         const QVariant &value)
{
    //Update the row property.
    KNMusicModel::setRowProperty(row, propertyRole, value);
    //Update the row in database.
    QJsonArray itemDataArray=m_database->at(row).toArray(),
               propertyArray=itemDataArray.at(1).toArray();
    //We only need to update the property rows.
    propertyArray.replace(PropertyFilePath,
                          rowProperty(row, FilePathRole).toString()); //PropertyFilePath
    propertyArray.replace(PropertyFileName,
                          rowProperty(row, FileNameRole).toString()); //PropertyFileName
    propertyArray.replace(PropertyCoverImageHash,
                          rowProperty(row, ArtworkKeyRole).toString()); //PropertyCoverImageHash
    propertyArray.replace(PropertyTrackFilePath,
                          rowProperty(row, TrackFileRole).toString()); //PropertyTrackFilePath
    propertyArray.replace(PropertyTrackIndex,
                          rowProperty(row, TrackIndexRole).toInt()); //PropertyTrackIndex
    propertyArray.replace(PropertyStartPosition,
                          QString::number(rowProperty(row, StartPositionRole).toLongLong())); //PropertyStartPosition;
    itemDataArray.replace(1, propertyArray);
    m_database->replace(row, itemDataArray);
}

void KNMusicLibraryModel::setItemText(const int &row,
                                      const int &column,
                                      const QString &text)
{
    //Set the item text.
    KNMusicModel::setItemText(row, column, text);
    //Update the row in database.
    QJsonArray itemDataArray=m_database->at(row).toArray(),
               textInformationArray=itemDataArray.at(0).toArray();
    textInformationArray.replace(column, text);
    itemDataArray.replace(0, textInformationArray);
    m_database->replace(row, itemDataArray);
}

void KNMusicLibraryModel::installCategoryModel(KNMusicCategoryModel *model)
{
    m_categoryModels.append(model);
}

void KNMusicLibraryModel::retranslate()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<MusicDisplayDataCount;i++)
    {
        header<<(m_musicGlobal->treeViewHeaderText(i));
    }
    setHorizontalHeaderLabels(header);
}

void KNMusicLibraryModel::addFiles(const QStringList &fileList)
{
    //Only analysis the file that we don't contain.
    QStringList zippedFileList;
    for(QStringList::const_iterator i=fileList.begin();
        i!=fileList.end();
        ++i)
    {
        //Check if we can find the file in the library.
        QModelIndexList fileCheck=match(index(0,0),
                                        FilePathRole,
                                        (*i),
                                        1,
                                        Qt::MatchFixedString);
        //If we can't find it, check in the filelist.
        if(fileCheck.isEmpty())
        {
            if(!zippedFileList.contains((*i)))
            {
                zippedFileList.append((*i));
            }
        }
    }
    //Ask to analysis the zipped file list.
    KNMusicModel::addFiles(zippedFileList);
}

void KNMusicLibraryModel::appendMusicRow(const QList<QStandardItem *> &musicRow)
{
    //Add the row to model.
    KNMusicModel::appendMusicRow(musicRow);
    //Add the row to database, generate the data list array.
    QJsonArray textInformationArray, propertyArray, itemDataArray;
    int i;
    for(i=0; i<MusicDataCount; i++)
    {
        textInformationArray.append(musicRow.at(i)->data(Qt::DisplayRole).toString());
    }
    QStandardItem *propertyItem=musicRow.at(0);
    propertyArray.append(propertyItem->data(FilePathRole).toString()); //PropertyFilePath
    propertyArray.append(propertyItem->data(FileNameRole).toString()); //PropertyFileName
    propertyArray.append(propertyItem->data(ArtworkKeyRole).toString()); //PropertyCoverImageHash
    propertyArray.append(musicRow.at(BitRate)->data(Qt::UserRole).toInt()); //PropertyBitRate
    propertyArray.append(musicRow.at(Rating)->data(Qt::DisplayRole).toInt()); //PropertyRating
    propertyArray.append(musicRow.at(SampleRate)->data(Qt::UserRole).toInt()); //PropertySampleRating
    propertyArray.append(QString::number(musicRow.at(Size)->data(Qt::UserRole).toLongLong())); //PropertySize
    propertyArray.append(QString::number(musicRow.at(Time)->data(Qt::UserRole).toLongLong())); //PropertyDuration
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicRow.at(DateAdded)->data(Qt::UserRole))); //PropertyDateAdded
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicRow.at(DateModified)->data(Qt::UserRole))); //PropertyDateModified
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(musicRow.at(LastPlayed)->data(Qt::UserRole))); //PropertyLastPlayed
    propertyArray.append(propertyItem->data(TrackFileRole).toString()); //PropertyTrackFilePath
    propertyArray.append(propertyItem->data(TrackIndexRole).toInt()); //PropertyTrackIndex
    propertyArray.append(QString::number(propertyItem->data(StartPositionRole).toLongLong())); //PropertyStartPosition
    itemDataArray.append(textInformationArray);
    itemDataArray.append(propertyArray);
    m_database->append(itemDataArray);
    //Add the row data to category models.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryAdded(musicRow);
    }
}

void KNMusicLibraryModel::updateMusicRow(const int &row,
                                         const KNMusicDetailInfo &detailInfo)
{
    //Do row udpates operate.
    KNMusicModel::updateMusicRow(row, detailInfo);
    //Update the row data in database, we need to generate the new information from the row.
    QJsonArray textInformationArray, propertyArray, itemDataArray;
    int i;
    for(i=0; i<MusicDataCount; i++)
    {
        textInformationArray.append(itemText(row, i));
    }
    propertyArray.append(rowProperty(row, FilePathRole).toString()); //PropertyFilePath
    propertyArray.append(rowProperty(row, FileNameRole).toString()); //PropertyFileName
    propertyArray.append(rowProperty(row, ArtworkKeyRole).toString()); //PropertyCoverImageHash
    propertyArray.append(roleData(row, BitRate, Qt::UserRole).toInt()); //PropertyBitRate
    propertyArray.append(roleData(row, Rating, Qt::DisplayRole).toInt()); //PropertyRating
    propertyArray.append(roleData(row, SampleRate, Qt::UserRole).toInt()); //PropertySampleRating
    propertyArray.append(QString::number(roleData(row, Size, Qt::UserRole).toLongLong())); //PropertySize
    propertyArray.append(QString::number(roleData(row, Time, Qt::UserRole).toLongLong())); //PropertyDuration
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(roleData(row, DateAdded, Qt::UserRole))); //PropertyDateAdded
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(roleData(row, DateModified, Qt::UserRole))); //PropertyDateModified
    propertyArray.append(KNMusicModelAssist::dateTimeToDataString(roleData(row, LastPlayed, Qt::UserRole))); //PropertyLastPlayed
    propertyArray.append(rowProperty(row, TrackFileRole).toString()); //PropertyTrackFilePath
    propertyArray.append(rowProperty(row, TrackIndexRole).toInt()); //PropertyTrackIndex
    propertyArray.append(QString::number(rowProperty(row, StartPositionRole).toLongLong())); //PropertyStartPosition
    itemDataArray.append(textInformationArray);
    itemDataArray.append(propertyArray);
    m_database->replace(row, itemDataArray);
}

void KNMusicLibraryModel::updateCoverImage(const int &row,
                                           const KNMusicAnalysisItem &analysisItem)
{
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Ask to update the image key in the database.
    QJsonArray itemDataArray=m_database->at(row).toArray(),
               propertyArray=itemDataArray.at(1).toArray();
    propertyArray.replace(PropertyCoverImageHash, detailInfo.coverImageHash);
    itemDataArray.replace(1, propertyArray);
    m_database->replace(row, itemDataArray);
    //Set the artwork key for the model, we have already save the data.
    KNMusicModel::setRowProperty(row, ArtworkKeyRole, detailInfo.coverImageHash);
    //Get the cover image.
    QPixmap coverImagePixmap=QPixmap::fromImage(analysisItem.coverImage);
    //Ask category models to update the cover image.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCoverImageUpdate(detailInfo.textLists[(*i)->categoryIndex()],
                                 detailInfo.coverImageHash,
                                 coverImagePixmap);
    }
}

void KNMusicLibraryModel::removeMusicRow(const int &row)
{
    //Remove the row from the database.
    m_database->removeAt(row);
    //Quick generate the row, this shouldn't so slow.
    QList<QStandardItem *> currentRow;
    for(int i=0; i<columnCount(); i++)
    {
        currentRow.append(item(row, i));
    }
    //Ask category model to remove this row.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryRemoved(currentRow);
    }
    //Save the album artwork key.
    QString currentArtworkKey=rowProperty(row, ArtworkKeyRole).toString();
    //Remove the row.
    KNMusicModel::removeMusicRow(row);
    //Check the artwork key after remove the row.
    //If no one use this artwork key any more, remove the artwork.
    if(match(index(0,0),
             ArtworkKeyRole,
             currentArtworkKey,
             1,
             Qt::MatchFixedString | Qt::MatchCaseSensitive).isEmpty())
    {
        //Remove the image from the disk and hash list.
        m_imageManager->removeImage(currentArtworkKey);
        m_coverImageList->removeImage(currentArtworkKey);
        //Ask category model to remove the image and replace it.
        for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
            i!=m_categoryModels.end();
            ++i)
        {
            //If the category model is asking to update album art, update it.
            if((*i)->updateAlbumArt())
            {
                QModelIndexList keyRemovedIndex=
                        (*i)->match((*i)->index(0,0),
                                    CategoryArtworkKeyRole,
                                    currentArtworkKey,
                                    -1,
                                    Qt::MatchFixedString);
                //Replace all the artworks.
                for(QModelIndexList::iterator j=keyRemovedIndex.begin();
                    j!=keyRemovedIndex.end();
                    ++j)
                {
                    //Search the category text in model.
                    QModelIndexList categoryList=match(index(0, (*i)->categoryIndex()),
                                                       Qt::DisplayRole,
                                                       (*i)->data(*j).toString(),
                                                       -1,
                                                       Qt::MatchExactly);
                    //Try to find the text.
                    for(QModelIndexList::iterator k=categoryList.begin();
                        k!=categoryList.end();
                        ++k)
                    {
                        //Get the artwork from the row property.
                        QString artworkKey=rowProperty((*k).row(), ArtworkKeyRole).toString();
                        //Check the artwork key.
                        if(!artworkKey.isEmpty())
                        {
                            (*i)->changeAlbumArt((*j),
                                                 artworkKey,
                                                 QIcon(artwork(artworkKey)));
                            break;
                        }
                    }
                }
            }
        }
    }
    //Check row count before remove row.
    if(rowCount()==0)
    {
        emit libraryEmpty();
    }
}

void KNMusicLibraryModel::appendLibraryMusicRow(const QList<QStandardItem *> &musicRow,
                                                const KNMusicAnalysisItem &analysisItem)
{
    //Append the music row first.
    appendMusicRow(musicRow);
    //Ask to analysis album art.
    m_analysisExtend->onActionAnalysisAlbumArt(musicRow.at(Name),
                                               analysisItem);
    //Check row count before add the row.
    if(rowCount()==1)
    {
        emit libraryNotEmpty();
    }
}

void KNMusicLibraryModel::recoverMusicRow(const QList<QStandardItem *> &musicRow)
{
    //Add the row to model.
    KNMusicModel::appendMusicRow(musicRow);
    //Add the row data to category models.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryRecover(musicRow);
    }
    //Check row count before add the row.
    if(rowCount()==1)
    {
        emit libraryNotEmpty();
    }
}

void KNMusicLibraryModel::imageRecoverComplete()
{
    //Ask category models to update images.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onImageRecoverComplete(m_coverImageList);
    }
}

inline void KNMusicLibraryModel::initialHeader()
{
    //Using retranslate to update the header text.
    retranslate();
    //Set header size hint.
    setHeaderData(0, Qt::Horizontal, QSize(10,23), Qt::SizeHintRole);
    //Set header alignment
    for(int i=0;i<MusicDataCount;i++)
    {
        setHeaderData(i, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
    }
    //Set special header data, e.g.: Sort flag.
    setHeaderData(Time, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(Size, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(TrackNumber, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    //Set sort flag.
    setHeaderSortFlag();
}

KNMusicLibraryImageManager *KNMusicLibraryModel::imageManager() const
{
    return m_imageManager;
}

void KNMusicLibraryModel::setImageManager(KNMusicLibraryImageManager *imageManager)
{
    m_imageManager=imageManager;
    //Set the hash list.
    m_imageManager->setPixmapList(m_coverImageList);
    //Link request.
    connect(m_imageManager, &KNMusicLibraryImageManager::recoverComplete,
            this, &KNMusicLibraryModel::imageRecoverComplete);
}

void KNMusicLibraryModel::recoverModel()
{
    //Read the database information.
    m_database->read();
    int j;
    //Recover the model for all the array data.
    for(QJsonArray::iterator i=m_database->begin();
        i!=m_database->end();
        i++)
    {
        //Generate the detail information.
        KNMusicDetailInfo currentDetail;
        //Get the data array, ignore the unavailable data.
        QJsonArray itemDataArray=(*i).toArray();
        if(itemDataArray.size()!=2)
        {
            continue;
        }
        //Get the information and property array.
        QJsonArray textInformationArray=itemDataArray.at(0).toArray(),
                   propertyArray=itemDataArray.at(1).toArray();
        //Load the data to the current detail.
        if(textInformationArray.size()!=MusicDataCount)
        {
            continue;
        }
        //Read the text list data.
        for(j=0; j<MusicDataCount; j++)
        {
            currentDetail.textLists[j]=textInformationArray.at(j).toString();
        }
        //Read the property data.
        currentDetail.filePath=propertyArray.at(PropertyFilePath).toString();
        currentDetail.fileName=propertyArray.at(PropertyFileName).toString();
        currentDetail.coverImageHash=propertyArray.at(PropertyCoverImageHash).toString();
        currentDetail.bitRate=propertyArray.at(PropertyBitRate).toInt();
        currentDetail.rating=propertyArray.at(PropertyRating).toInt();
        currentDetail.samplingRate=propertyArray.at(PropertySampleRating).toInt();
        currentDetail.size=propertyArray.at(PropertySize).toString().toLongLong();
        currentDetail.duration=propertyArray.at(PropertyDuration).toString().toLongLong();
        currentDetail.dateAdded=KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyDateAdded).toString());
        currentDetail.dateModified=KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyDateModified).toString());
        currentDetail.lastPlayed=KNMusicModelAssist::dataStringToDateTime(propertyArray.at(PropertyLastPlayed).toString());
        currentDetail.trackFilePath=propertyArray.at(PropertyTrackFilePath).toString();
        currentDetail.trackIndex=propertyArray.at(PropertyTrackIndex).toInt();
        currentDetail.startPosition=propertyArray.at(PropertyStartPosition).toString().toLongLong();
        //Recover the row.
        recoverMusicRow(KNMusicModelAssist::generateRow(currentDetail));
    }
}

KNJSONDatabase *KNMusicLibraryModel::database() const
{
    return m_database;
}

void KNMusicLibraryModel::setDatabase(KNJSONDatabase *database)
{
    m_database = database;
}
