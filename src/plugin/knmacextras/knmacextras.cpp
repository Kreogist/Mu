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
#include <QAction>
#include <QMenu>

#include "knlocalemanager.h"

#include "knmacextras.h"

#include <QDebug>

//Export set dock menu function from Qt.
extern void qt_mac_set_dock_menu(QMenu *);

KNMacExtras::KNMacExtras(QObject *parent) :
    KNPlatformExtras(parent),
    m_dockMenu(new QMenu()),
    m_statusBeforeAction(nullptr),
    m_isStatePlay(false),
    m_isNowPlayingShown(false)
{
    //Construct actions.
    for(int i=0; i<MacDockMenuActionCount; ++i)
    {
        //Construct action.
        m_menuAction[i]=new QAction(this);
    }
    //Set the status before action.
    m_statusBeforeAction=m_menuAction[PlayNPause];
    //Configure the actions.
    m_menuAction[NowPlaying]->setEnabled(false);
    m_menuAction[SongName]->setEnabled(false);
    m_menuAction[ArtistAndAlbumName]->setEnabled(false);
    connect(m_menuAction[NextSong], &QAction::triggered,
            this, &KNMacExtras::requirePlayNext);
    connect(m_menuAction[PreviousSong], &QAction::triggered,
            this, &KNMacExtras::requirePlayPrev);
    //Add actions.
    m_dockMenu->addAction(m_menuAction[PlayNPause]);
    m_dockMenu->addAction(m_menuAction[NextSong]);
    m_dockMenu->addAction(m_menuAction[PreviousSong]);
    //Set the dock menu.
    m_dockMenu->setAsDockMenu();

    //Link the retranslator.
    knI18n->link(this, &KNMacExtras::retranslate);
    retranslate();
}

void KNMacExtras::setMainWindow(QMainWindow *mainWindow)
{
    Q_UNUSED(mainWindow)
}

void KNMacExtras::onActionPlayStateChanged(const bool &isPlay)
{
    //Save the playing state data.
    m_isStatePlay=isPlay;
    //Update the play state.
    updatePlayState();
}

void KNMacExtras::onActionMuteStateChanged(const bool &isMute)
{
    Q_UNUSED(isMute)
}

void KNMacExtras::onActionLoopStateChanged(const int &loopState)
{
    Q_UNUSED(loopState)
}

void KNMacExtras::onActionNowPlayingChanged(const PlatformPlayingInfo &info)
{
    //Save the info.
    m_nowPlaying=info;
    //Update the playing information.
    updatePlayingInfo();
}

void KNMacExtras::loadPreference()
{
    ;
}

void KNMacExtras::savePreference()
{
    ;
}

void KNMacExtras::retranslate()
{
    //Update the fixed text.
    m_menuAction[NowPlaying]->setText(tr("Now Playing"));
    m_menuAction[NextSong]->setText(tr("Next"));
    m_menuAction[PreviousSong]->setText(tr("Previous"));
    //Update the state text.
    m_playText=tr("Play");
    m_pauseText=tr("Pause");
    m_noArtistText=tr("No Artist");
    m_noAlbumText=tr("No Album");
    //Update the play state.
    updatePlayState();
    //Update the playing information.
    updatePlayingInfo();
}

inline void KNMacExtras::updatePlayingInfo()
{
    //Check the validation of the now playing item.
    if(m_nowPlaying.isNull)
    {
        //Hide the action.
        //Check teh shown flag.
        if(m_isNowPlayingShown)
        {
            //Mark the playing shown flag to false.
            m_isNowPlayingShown=false;
            //Remove the action from the menu.
            m_dockMenu->removeAction(m_menuAction[NowPlaying]);
            m_dockMenu->removeAction(m_menuAction[SongName]);
            m_dockMenu->removeAction(m_menuAction[ArtistAndAlbumName]);
            //Clear its text.
            m_menuAction[SongName]->setText("");
            m_menuAction[ArtistAndAlbumName]->setText("");
        }
        //Mission complete.
        return;
    }
    //Show the action.
    if(!m_isNowPlayingShown)
    {
        //Update the shown flag.
        m_isNowPlayingShown=true;
        //Insert the action widget to the menu.
        m_dockMenu->insertAction(m_statusBeforeAction,
                                 m_menuAction[NowPlaying]);
        m_dockMenu->insertAction(m_statusBeforeAction,
                                 m_menuAction[SongName]);
        m_dockMenu->insertAction(m_statusBeforeAction,
                                 m_menuAction[ArtistAndAlbumName]);
    }
    //Update the song name action text.
    m_menuAction[SongName]->setText("  " + m_nowPlaying.name);
    //Update the artist and album action text.
    m_menuAction[ArtistAndAlbumName]->setText("  " +
                (m_nowPlaying.artist.isEmpty()?
                    m_noArtistText:
                    m_nowPlaying.artist) + " - " +
                (m_nowPlaying.album.isEmpty()?
                     m_noAlbumText:
                     m_nowPlaying.album));
}

inline void KNMacExtras::updatePlayState()
{
    //Update the state.
    m_menuAction[PlayNPause]->setText(m_isStatePlay?m_pauseText:m_playText);
    //Cut down the previous connection.
    disconnect(m_menuAction[PlayNPause], 0, 0, 0);
    //Update the link according to the state.
    connect(m_menuAction[PlayNPause], &QAction::triggered, this,
            m_isStatePlay?
                (&KNMacExtras::requirePause):
                (&KNMacExtras::requirePlay));
}
