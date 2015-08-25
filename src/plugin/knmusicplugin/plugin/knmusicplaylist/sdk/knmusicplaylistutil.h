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

#ifndef KNMUSICPLAYLISTUTIL_H
#define KNMUSICPLAYLISTUTIL_H

#include <QJsonObject>

#include "knmusicutil.h"

using namespace MusicUtil;

class KNMusicPlaylistUtil
{
public:
    /*!
     * \brief Translate a QJsonObject to a KNMusicDetailInfo class.
     * \param object The json object class.
     * \return The translated KNMusicDetailInfo class.
     */
    static KNMusicDetailInfo objectToDetailInfo(const QJsonObject &object);

    /*!
     * \brief Translate a KNMusicDetailInfo to a QJsonObject class.
     * \param detailInfo The KNMusicDetailInfo class.
     * \return The translate QJsonObject class.
     */
    static QJsonObject detailInfoToObject(const KNMusicDetailInfo &detailInfo);

private:
    KNMusicPlaylistUtil();
    KNMusicPlaylistUtil(const KNMusicPlaylistUtil &);
};

#endif // KNMUSICPLAYLISTUTIL_H
