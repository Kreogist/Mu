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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICSTOREPAGESEARCHRESULT_H
#define KNMUSICSTOREPAGESEARCHRESULT_H

#include "knmusicstorepage.h"

/*!
 * \brief The KNMusicStorePageSearchResult class provides the search result page
 * for the music store. It will ask for all the backend to do the search, and
 * rank the result by the string length.
 */
class KNMusicStorePageSearchResult : public KNMusicStorePage
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStorePageSearchResult widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStorePageSearchResult(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicStorePage::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setPageLabel().
     */
    void setPageLabel(int labelIndex, const QVariant &value) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStorePage::setBackend().
     */
    void setBackend(KNMusicStoreBackend *backend) Q_DECL_OVERRIDE;

private:

};

#endif // KNMUSICSTOREPAGESEARCHRESULT_H
