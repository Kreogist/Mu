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

#ifndef KNMUSICLYRICSBASE_H
#define KNMUSICLYRICSBASE_H

#include <QWidget>

class KNMusicLyricsBackend;
/*!
 * \brief The KNMusicLyricsBase class provides the basic interface of a lyrics
 * widget should provided. You have to implemented it and provides all the ports
 * to your own widget.\n
 * This is a MVC-liked part for lyrics. Treat this part as a View part. It
 * decides how the data of the lyrics will be shown and the transfer animation.
 */
class KNMusicLyricsBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicLyricsBase object.
     * \param parent The parent object.
     */
    KNMusicLyricsBase(QWidget *parent = 0):QWidget(parent){}

    /*!
     * \brief Get the backend of the lyrics view.
     * \return The lyrics backend pointer. If you haven't set it before
     */
    virtual KNMusicLyricsBackend *backend()=0;

signals:

public slots:
    /*!
     * \brief Set the lyrics backend of the lyrics view.
     * \param backend The backend object pointer.
     */
    virtual void setBackend(KNMusicLyricsBackend *backend)=0;

    /*!
     * \brief Move the view to the specific line.
     * \param lineIndex The lyrics line index.
     */
    virtual void moveToLine(const int &lineIndex)=0;
};

#endif // KNMUSICLYRICSBASE_H
