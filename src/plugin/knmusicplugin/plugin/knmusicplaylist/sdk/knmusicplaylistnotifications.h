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
#ifndef KNMUSICPLAYLISTNOTIFICATIONS_H
#define KNMUSICPLAYLISTNOTIFICATIONS_H

#include "knnotificationdata.h"

//Defines all the notification of the playlist.
class KNMusicPlaylistFailedLoaded : public KNNotificationData
{
public:
    /*!
     * \brief Construct a KNMusicPlaylistFailedLoaded object.
     */
    explicit KNMusicPlaylistFailedLoaded(QObject *parent = 0);

    /*!
     * \brief Reimplemented from KNNotificationData::title().
     */
    QString title() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNNotificationData::content().
     */
    QString content() const Q_DECL_OVERRIDE;

    /*!
     * \brief Set the displayed failed counter number.
     * \param failedCounter The number of the failed loaded playlist number.
     */
    void setFailedCounter(int failedCounter);

private:
    int m_failedCounter;
};

#endif // KNMUSICPLAYLISTNOTIFICATIONS_H
