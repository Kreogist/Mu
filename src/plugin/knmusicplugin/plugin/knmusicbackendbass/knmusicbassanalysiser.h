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

#ifndef KNMUSICBASSANALYSISER_H
#define KNMUSICBASSANALYSISER_H

#include "knmusicanalysiser.h"

/*!
 * \brief The KNMusicBassAnalysiser class
 */
class KNMusicBassAnalysiser : public KNMusicAnalysiser
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBassAnalysiser object.
     * \param parent The parent object pointer.
     */
    explicit KNMusicBassAnalysiser(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicAnalysiser::analysis().
     */
    bool analysis(KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;
};

#endif // KNMUSICBASSANALYSISER_H
