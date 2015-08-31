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

#ifndef KNMUSICLISTPARSER_H
#define KNMUSICLISTPARSER_H

#include <QFile>

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

/*!
 * \brief The KNMusicListParser class will used to parse a list file. Like cue
 * or cda.
 */
class KNMusicListParser : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicListParser object.
     * \param parent The parent object. It should be the parser.
     */
    KNMusicListParser(QObject *parent = 0):QObject(parent){}

    /*!
     * \brief Parse the list file into several music details.
     * \param listFile The list file.
     * \param listDetailInfo The result of parsing.
     * \return If the parser can parse the list file successfully, return true.
     */
    virtual bool parseList(QFile &listFile,
                           KNMusicListDetailInfo &listDetailInfo)=0;

    /*!
     * \brief Write a detail information to the list file.
     * \param analysisItem The information of the music track.
     * \return If the information written successfully, then return true.
     */
    virtual bool writeDetail(const KNMusicAnalysisItem &analysisItem)=0;
};

#endif // KNMUSICLISTPARSER_H
