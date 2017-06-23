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
#ifndef KNMACEXTRAS_H
#define KNMACEXTRAS_H

#include "knplatformextras.h"

class QAction;
class QMenu;
/*!
 * \brief The KNMacExtras class provides some extra interface under Mac OS X.\n
 * Including the following features:
 *  - Dock bar menu.
 */
class KNMacExtras : public KNPlatformExtras
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMacExtras object with given parent.
     * \param parent The parent object.
     */
    explicit KNMacExtras(QObject *parent = 0);
    ~KNMacExtras();

    /*!
     * \brief Reimplemented from KNPlatformExtras::setMainWindow().
     */
    void setMainWindow(QMainWindow *mainWindow) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNPlatformExtras::onActionPlayStateChanged().
     */
    void onActionPlayStateChanged(const bool &isPlay) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPlatformExtras::onActionMuteStateChanged().
     */
    void onActionMuteStateChanged(const bool &isMute) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPlatformExtras::onActionLoopStateChanged().
     */
    void onActionLoopStateChanged(const int &loopState) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPlatformExtras::onActionNowPlayingChanged().
     */
    void onActionNowPlayingChanged(const PlatformPlayingInfo &info)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPlatformExtras::loadPreference().
     */
    void loadPreference() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNPlatformExtras::savePreference().
     */
    void savePreference() Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    enum MacDockMenuActions
    {
        NowPlaying,
        SongName,
        ArtistAndAlbumName,
        PlayNPause,
        NextSong,
        PreviousSong,
        MacDockMenuActionCount
    };
    enum MacDockSubMenus
    {
        Repeat,
        MacDockSubMenuCount
    };
    inline void updatePlayingInfo();
    inline void updatePlayState();
    QMenu *m_dockMenu, *m_subMenus[MacDockSubMenuCount];
    QAction *m_menuAction[MacDockMenuActionCount], *m_infoSeparator;
    QString m_playText, m_pauseText, m_noArtistText, m_noAlbumText;
    PlatformPlayingInfo m_nowPlaying;
    bool m_isStatePlay;
};

#endif // KNMACEXTRAS_H
