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
#include "knmusiclibraryanalysisextend.h"
#include "knmusiclibrarydatabase.h"
#include "knmusiclibraryimagemanager.h"

#include "knlocalemanager.h"

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
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
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
    //If we can't find it, check in the filelist.
    if(fileCheck.isEmpty())
    {
        return -1;
    }
    return fileCheck.first().row();
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
    updateRowInDatabase(row);
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
    //Add the row to database.
    m_database->appendMusicRow(musicRow);
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
    //Update the row in database.
    updateRowInDatabase(row);
}

void KNMusicLibraryModel::updateCoverImage(const int &row,
                                           const KNMusicDetailInfo &detailInfo)
{
    //Ask to update the image key in the database.
    m_database->updateArtworkKey(row, detailInfo.coverImageHash);
    //Set the artwork key for the model.
    setRowProperty(row, ArtworkKeyRole, detailInfo.coverImageHash);
    //Get the cover image.
    QPixmap coverImagePixmap=QPixmap::fromImage(detailInfo.coverImage);
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
    m_database->removeMusicRow(row);
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
                                                const KNMusicDetailInfo &detailInfo)
{
    //Append the music row first.
    appendMusicRow(musicRow);
    //Ask to analysis album art.
    m_analysisExtend->onActionAnalysisAlbumArt(musicRow.at(Name), detailInfo);
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

inline void KNMusicLibraryModel::updateRowInDatabase(const int &row)
{
    //Quick generate the row, this shouldn't so slow.
    QList<QStandardItem *> currentRow;
    for(int i=0; i<columnCount(); i++)
    {
        currentRow.append(item(row, i));
    }
    //Ask to update the row in the database.
    m_database->updateMusicRow(row, currentRow);
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

KNMusicLibraryDatabase *KNMusicLibraryModel::database() const
{
    return m_database;
}

void KNMusicLibraryModel::setDatabase(KNMusicLibraryDatabase *database)
{
    m_database = database;
    //Linked request.
    connect(m_database, &KNMusicLibraryDatabase::requireRecoverMusicRow,
            this, &KNMusicLibraryModel::recoverMusicRow);
}
