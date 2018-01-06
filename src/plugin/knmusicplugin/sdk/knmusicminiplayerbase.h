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

#ifndef KNMUSICMINIPLAYERBASE_H
#define KNMUSICMINIPLAYERBASE_H

#include "knmusicplayerbase.h"

/*!
 * \brief The KNMusicMiniPlayerBase class provides the ports for a mini player
 * widget that needs to implement.\n
 * A mini player is a widget-mode player which sync with the data of main player
 * but it would be float as a small widget window.
 */
class KNMusicMiniPlayerBase : public KNMusicPlayerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMiniPlayerBase widget.
     * \param parent The parent widget.
     */
    KNMusicMiniPlayerBase(QWidget *parent = 0) : KNMusicPlayerBase(parent){}

signals:
    /*!
     * \brief When the player is required to close itself, this signal should be
     * emitted. The music plugin will handle this signal.
     */
    void requireHidePlayer();

    /*!
     * \brief When the player is required to close the whole application, this
     * signal should be emitted to close the main window and it will close the
     * whole application.
     */
    void requireCloseMainWindow();

public slots:
    /*!
     * \brief When the playing state is reseted, this slot would be called.
     */
    virtual void reset()=0;

    /*!
     * \brief Save the configure to the specific object.
     */
    virtual void saveConfigure()=0;

    /*!
     * \brief Load the configure from the specific object.
     */
    virtual void loadConfigure()=0;
};

#endif // KNMUSICMINIPLAYERBASE_H
