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
#include <QMenu>

#include "knmacextras.h"

KNMacExtras::KNMacExtras(QObject *parent) :
    KNPlatformExtras(parent)
{
    //Initial the tray icon.
    initialTrayIconControls();
}

KNMacExtras::~KNMacExtras()
{
    delete m_dummyMenu;
}

void KNMacExtras::setMainWindow(QMainWindow *mainWindow)
{
    Q_UNUSED(mainWindow);
}

void KNMacExtras::onActionPlayStateChanged(const bool &isPlay)
{
    //Save the state.
    m_isPlay=isPlay;
    //Change the button icon.
    m_actionButtons[PlayAndPause]->setIcon(m_isPlay?m_pauseIcon:m_playIcon);
}

void KNMacExtras::onActionMuteStateChanged(const bool &isMute)
{
    Q_UNUSED(isMute)
}

void KNMacExtras::onActionLoopStateChanged(const int &loopState)
{
    Q_UNUSED(loopState)
}

void KNMacExtras::loadPreference()
{
    ;
}

void KNMacExtras::savePreference()
{
    ;
}

void KNMacExtras::prevActivated(const QSystemTrayIcon::ActivationReason &reason)
{
    //Check the reason.
    if(reason==QSystemTrayIcon::Trigger)
    {
        emit requirePlayPrev();
    }
}

void KNMacExtras::playNPauseActivated(const QSystemTrayIcon::ActivationReason &reason)
{
    if(reason==QSystemTrayIcon::Trigger)
    {
        if(m_isPlay)
        {
            emit requirePause();
        }
        else
        {
            emit requirePlay();
        }
    }
}

void KNMacExtras::nextActivated(const QSystemTrayIcon::ActivationReason &reason)
{
    //Check the reason.
    if(reason==QSystemTrayIcon::Trigger)
    {
        emit requirePlayNext();
    }
}

inline void KNMacExtras::initialTrayIconControls()
{
    //Initial the dummy menu.
    m_dummyMenu=new QMenu;

    //Initial the playing and pause icon.
    m_playIcon=QIcon(":/platform/mac/play.png");
    m_pauseIcon=QIcon(":/platform/mac/pause.png");

    //Configure the button icons.
    QIcon actionIcons[ThumbnailActionsCount];
    actionIcons[PlayPrev]=QIcon(":/platform/mac/prev.png");
    actionIcons[PlayAndPause]=m_playIcon;
    actionIcons[PlayNext]=QIcon(":/platform/mac/next.png");

    //Initial all the system tray icon button.
    for(int i=ThumbnailActionsCount-1; i>-1; i--)
    {
        m_actionButtons[i]=new QSystemTrayIcon(this);
        m_actionButtons[i]->setContextMenu(m_dummyMenu);
        m_actionButtons[i]->setIcon(QIcon(actionIcons[i]));
        m_actionButtons[i]->show();
    }

    //Link the buttons.
    connect(m_actionButtons[PlayPrev], &QSystemTrayIcon::activated,
            this, &KNMacExtras::prevActivated);
    connect(m_actionButtons[PlayAndPause], &QSystemTrayIcon::activated,
            this, &KNMacExtras::playNPauseActivated);
    connect(m_actionButtons[PlayNext], &QSystemTrayIcon::activated,
            this, &KNMacExtras::nextActivated);
}
