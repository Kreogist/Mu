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

#ifndef KNABSTRACTMUSICPLUGIN_H
#define KNABSTRACTMUSICPLUGIN_H

#include "kncategoryplugin.h"

class KNPlatformExtras;
/*!
 * \brief The KNAbstractMusicCategoryPlugin class is the port class of the music
 * plugin. Your own music plugin should provide these function.
 */
class KNAbstractMusicPlugin : public KNCategoryPlugin
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAbstractMusicPlugin object.
     * \param parent The parent object.
     */
    KNAbstractMusicPlugin(QWidget *parent=0):KNCategoryPlugin(parent){}

    /*!
     * \brief Provides the main player widget pointer.
     * \return The main player widget pointer.
     */
    virtual QWidget *mainPlayer()=0;

    /*!
     * \brief Provides whether current music pluign is working.
     * \return Whether the music plugin is still working.
     */
    virtual bool isWorking()=0;

signals:
    /*!
     * \brief This signal will ask the main window to show the main player.
     */
    void requireShowMainPlayer();

    /*!
     * \brief This signal will ask the main window to hide the main player.
     */
    void requireHideMainPlayer();

    /*!
     * \brief This signal will ask the main window to show itself.
     */
    void requireShowMainWindow();

    /*!
     * \brief This signal will ask the main window to hide itself.
     */
    void requireHideMainWindow();

    /*!
     * \brief When user click the close button on mini player, this signal will
     * be emitted to ask to close the main window.
     */
    void requireCloseMainWindow();

public slots:
    /*!
     * \brief setPlatformExtras
     * \param platformExtras
     */
    virtual void setPlatformExtras(KNPlatformExtras *platformExtras)=0;

    /*!
     * \brief Show the mini player if the music plugin provide a mini player.\n
     * This function should be only called when application launched.
     */
    virtual void showMiniPlayer()=0;

    /*!
     * \brief Hide the mini player if the music plugin provide a mini player.\n
     * This function should only be called when the main window is showing.
     */
    virtual void hideMiniPlayer()=0;
};

#endif // KNABSTRACTMUSICPLUGIN_H
