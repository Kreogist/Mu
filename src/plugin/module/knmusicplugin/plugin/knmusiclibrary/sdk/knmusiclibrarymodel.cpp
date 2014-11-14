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

#include "knmusiclibrarymodel.h"

KNMusicLibraryModel::KNMusicLibraryModel(QObject *parent) :
    KNMusicModel(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Connect language changed request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicLibraryModel::retranslate);
    //Initial header.
    initialHeader();
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

void KNMusicLibraryModel::appendMusicRow(const QList<QStandardItem *> &musicRow)
{
    //Add current data to category models.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryAdded(musicRow.at((*i)->categoryIndex())->text());
    }
    //Add the row to model.
    KNMusicModel::appendMusicRow(musicRow);
}

void KNMusicLibraryModel::removeMusicRow(const int &row)
{
    //Ask category model to remove this row.
    for(QLinkedList<KNMusicCategoryModel *>::iterator i=m_categoryModels.begin();
        i!=m_categoryModels.end();
        ++i)
    {
        (*i)->onCategoryRemoved(data(index(row, (*i)->categoryIndex()),
                                     Qt::DisplayRole).toString());
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
