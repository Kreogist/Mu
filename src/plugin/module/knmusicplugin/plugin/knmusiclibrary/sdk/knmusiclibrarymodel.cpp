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
#include "knmusiclibraryanalysisextend.h"
#include "knhashpixmaplist.h"

#include "knlocalemanager.h"

#include "knmusiclibrarymodel.h"

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
    connect(m_analysisExtend, &KNMusicLibraryAnalysisExtend::requireUpdateAlbumArt,
            this, &KNMusicLibraryModel::updateCoverImage);
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
                                    1);
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
                                        1);
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
    //Add the row data to category models.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryAdded(musicRow);
    }
}

void KNMusicLibraryModel::updateCoverImage(const KNMusicDetailInfo &detailInfo)
{
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
    //Remove the row.
    KNMusicModel::removeMusicRow(row);
}

void KNMusicLibraryModel::initialHeader()
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
