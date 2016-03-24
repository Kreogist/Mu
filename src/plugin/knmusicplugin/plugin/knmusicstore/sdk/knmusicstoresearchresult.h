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
#ifndef KNMUSICSTORESEARCHRESULT_H
#define KNMUSICSTORESEARCHRESULT_H

#include "knmusicstoreutil.h"

#include <QObject>

class KNMusicStoreSearchModel;
/*!
 * \brief The KNMusicStoreSearchResult class provides several models which is
 * used to describe the search result back from the backend.
 */
class KNMusicStoreSearchResult : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreSearchResult object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreSearchResult(QObject *parent = 0);

    /*!
     * \brief Get the search result model pointer.
     * \param index The index of the model.
     * \return The search result model pointer.
     */
    KNMusicStoreSearchModel *searchResultModel(int index);

signals:

public slots:

private:
    KNMusicStoreSearchModel *m_searchResults[
                        KNMusicStoreUtil::StoreSearchCategoryCount];
};

#endif // KNMUSICSTORESEARCHRESULT_H
