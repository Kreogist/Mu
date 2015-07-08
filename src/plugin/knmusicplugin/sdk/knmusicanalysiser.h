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

#ifndef KNMUSICANALYSISER_H
#define KNMUSICANALYSISER_H

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

/*!
 * \brief The KNMusicAnalysiser class is a interface class of music parser. The
 * parser class will be run at the same thread as the parser. Parser will tried
 * all the analysiser to analysis a file. If any analysiser return true, the
 * file won't be parsered again.
 */
class KNMusicAnalysiser : public QObject
{
public:
    /*!
     * \brief Construct a KNMusicAnalysiser class.
     * \param parent The parent object. It should be the parser.
     */
    KNMusicAnalysiser(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Analysis a file, write the information to the detail info.
     * \param detailInfo The detail info of the file.
     * \return If analysis the file successfully, return true.
     */
    virtual bool analysis(KNMusicDetailInfo &detailInfo)=0;
};

#endif // KNMUSICANALYSISER_H
