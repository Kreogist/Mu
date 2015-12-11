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

#ifndef KNMUSICLYRICSDOWNLOADDIALOGBASE_H
#define KNMUSICLYRICSDOWNLOADDIALOGBASE_H

#include "knmusicutil.h"

#include "knmessagebox.h"

using namespace MusicUtil;

/*!
 * \brief The KNMusicLyricsDownloadDialogBase class provide all the basic
 * function of the lyrics download wildget. The lyrics download dialog should be
 * implemented from this class.
 */
class KNMusicLyricsDownloadDialogBase : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLyricsDownloadDialogBase dialog widget.
     * \param parent The parent widget.
     */
    KNMusicLyricsDownloadDialogBase(QWidget *parent = 0):KNMessageBox(parent){}

public slots:
    /*!
     * \brief Set the detail info of the song which will be download.
     * \param detailInfo The detail info structure.
     */
    virtual void setDetailInfo(const KNMusicDetailInfo &detailInfo)=0;
};

#endif // KNMUSICLYRICSDOWNLOADDIALOGBASE_H
