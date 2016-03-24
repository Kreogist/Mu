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
#ifndef KNMUSICSTORESEARCHMODEL_H
#define KNMUSICSTORESEARCHMODEL_H

#include "knmusicstoresonglistmodel.h"

/*!
 * \brief The KNMusicStoreSearchModel class provides a model to show the result
 * of search.\n
 * It could be reused for several times. It contains the total count.
 */
class KNMusicStoreSearchModel : public KNMusicStoreSongListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Constructu a KNMusicStoreSearchModel object.
     * \param parent The parent object pointer.
     */
    explicit KNMusicStoreSearchModel(QObject *parent = 0);

    /*!
     * \brief Get the total count.
     * \return The total count of the search result.
     */
    int totalCount() const;

    /*!
     * \brief Reset the total count.
     */
    void resetTotalCount();

signals:

public slots:
    /*!
     * \brief Set the total count.
     * \param totalCount The total count of the search result.
     */
    void setTotalCount(int totalCount);

private:
    int m_totalCount;
};

#endif // KNMUSICSTORESEARCHMODEL_H
