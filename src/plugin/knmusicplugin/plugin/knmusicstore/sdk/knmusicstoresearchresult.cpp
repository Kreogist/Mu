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

#include "knmusicstoresearchmodel.h"

#include "knmusicstoresearchresult.h"

KNMusicStoreSearchResult::KNMusicStoreSearchResult(QObject *parent) :
    QObject(parent)
{
    //Initial the models.
    for(int i=0; i<KNMusicStoreUtil::StoreSearchCategoryCount; ++i)
    {
        //Generate the model.
        m_searchResults[i]=new KNMusicStoreSearchModel(this);
    }

    //Link the locale manager.
    knI18n->link(this, &KNMusicStoreSearchResult::retranslate);
    retranslate();
}

KNMusicStoreSearchModel *KNMusicStoreSearchResult::searchResultModel(
        int index)
{
    Q_ASSERT(index>-1 && index<KNMusicStoreUtil::StoreSearchCategoryCount);
    //Give back the pointer.
    return m_searchResults[index];
}

void KNMusicStoreSearchResult::retranslate()
{
    //Prepare the string list.
    QStringList songColumn, artistColumn, albumColumn;
    //Update song model column title text.
    songColumn.append(tr("Name"));
    songColumn.append(tr("Artist"));
    songColumn.append(tr("Album"));
    songColumn.append(tr("Duration"));
    //Set the data to search result.
    m_searchResults[KNMusicStoreUtil::CategorySong]->setColumnFieldNames(
                songColumn);
}
