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
#include <QtWinExtras>

#include <QApplication>
#include <QMenu>
#include <QSignalMapper>

#include "knlocalemanager.h"
#include "knmainwindow.h"

#include "knwindowsextras.h"

KNWindowsExtras::KNWindowsExtras(QObject *parent) :
    KNPlatformExtras(parent),
    m_playIcon(QPixmap(":/platform/platforms/windows/play.png")),
    m_pauseIcon(QPixmap(":/platform/platforms/windows/pause.png")),
    m_muteOn(QPixmap(":/platform/platforms/windows/mute_on.png")),
    m_muteOff(QPixmap(":/platform/platforms/windows/mute.png")),
    m_thumbnailToolbar(new QWinThumbnailToolBar(this)),
    m_trayIcon(new QSystemTrayIcon(this)),
    m_trayIconMenu(new QMenu),
    m_mainWindow(nullptr),
    m_isStatePlay(true)
{
    //Initial the loop states.
    m_loopStates[ButtonNoRepeat]=
            QPixmap(":/platform/platforms/windows/loopmode_norepeat.png");
    m_loopStates[ButtonRepeat]=
            QPixmap(":/platform/platforms/windows/loopmode_repeatsingle.png");
    m_loopStates[ButtonRepeatAll]=
            QPixmap(":/platform/platforms/windows/loopmode_repeat.png");
    m_loopStates[ButtonShuffle]=
            QPixmap(":/platform/platforms/windows/loopmode_random.png");
    //Initial the buttons.
    for(int i=0; i<ThumbnailActionsCount; i++)
    {
        //Generate the button.
        m_thumbnailButtons[i]=new QWinThumbnailToolButton(m_thumbnailToolbar);
        //Add the button to toolbar.
        m_thumbnailToolbar->addButton(m_thumbnailButtons[i]);
    }
    //Connect signals.
    connect(m_thumbnailButtons[PlayPrev], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requirePlayPrev);
    connect(m_thumbnailButtons[PlayAndPause], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::onActionPlayAndPause);
    connect(m_thumbnailButtons[PlayNext], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requirePlayNext);
    connect(m_thumbnailButtons[VolumeUp], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requireVolumeUp);
    connect(m_thumbnailButtons[VolumeDown], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requireVolumeDown);
    connect(m_thumbnailButtons[Mute], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requireChangeMuteState);
    connect(m_thumbnailButtons[LoopMode], &QWinThumbnailToolButton::clicked,
            this, &KNWindowsExtras::requireChangeLoopState);
    //Set icon.
    setButtonIcon(PlayPrev,
                  QPixmap(":/platform/platforms/windows/previous.png"));
    setButtonIcon(PlayNext,
                  QPixmap(":/platform/platforms/windows/next.png"));
    setButtonIcon(VolumeUp,
                  QPixmap(":/platform/platforms/windows/volup.png"));
    setButtonIcon(VolumeDown,
                  QPixmap(":/platform/platforms/windows/voldown.png"));
    //Set default state button.
    onActionMuteStateChanged(false);
    onActionLoopStateChanged(ButtonNoRepeat);
    onActionPlayStateChanged(false);
    //Configure the tray icon.
    m_trayIcon->setToolTip(QApplication::applicationName());
    m_trayIcon->setIcon(QIcon("://icon/mu.png"));
    connect(m_trayIcon, &QSystemTrayIcon::activated,
                this, &KNWindowsExtras::onActionTrayIconActivate);
    //Configure the tray icon menu.
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();
    //Initial the tray icon menu actions.
    QSignalMapper *actionTriggerMapper=new QSignalMapper(this);
    connect(actionTriggerMapper,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            this, &KNWindowsExtras::onActionTrayMenuActionTriggered);
    //Initial the tray icon actions.
    for(int i=0; i<TrayIconMenuActionCount; i++)
    {
        //Initial the action and add to the menu.
        m_trayIconActions[i]=new QAction(m_trayIconMenu);
        //Add action to menu.
        m_trayIconMenu->addAction(m_trayIconActions[i]);
        //Link the action to the mapper.
        connect(m_trayIconActions[i], SIGNAL(triggered(bool)),
                actionTriggerMapper, SLOT(map()));
        actionTriggerMapper->setMapping(m_trayIconActions[i], i);
    }
    //Link locale manager.
    knI18n->link(this, &KNWindowsExtras::retranslate);
    retranslate();
}

void KNWindowsExtras::setMainWindow(QMainWindow *mainWindow)
{
    //Cast main window to expand main window.
    m_mainWindow=static_cast<KNMainWindow *>(mainWindow);
    //Check main window pointer.
    if(m_mainWindow==nullptr)
    {
        //Ignore the invalid pointer.
        return;
    }
    //Native window hack.
    //We need to do this because windowHandle() only returns an available value
    //when the QWidget is a native widget. Using this hack to make the main
    //window to a native widget and the windowHandle() will return the handle.
    m_mainWindow->setAttribute(Qt::WA_NativeWindow);
    //Initial the toolbar.
    m_thumbnailToolbar->setParent(m_mainWindow);
    m_thumbnailToolbar->setWindow(m_mainWindow->windowHandle());
}

void KNWindowsExtras::onActionPlayStateChanged(const bool &isPlay)
{
    //Save the state.
    m_isStatePlay=isPlay;
    //Set the icon according to the state.
    setButtonIcon(PlayAndPause, m_isStatePlay?m_pauseIcon:m_playIcon);
}

void KNWindowsExtras::onActionMuteStateChanged(const bool &isMute)
{
    setButtonIcon(Mute, isMute?m_muteOn:m_muteOff);
}

void KNWindowsExtras::onActionLoopStateChanged(const int &loopState)
{
    setButtonIcon(LoopMode, m_loopStates[loopState]);
}

void KNWindowsExtras::onActionNowPlayingChanged(const PlatformPlayingInfo &info)
{
    Q_UNUSED(info)
}

void KNWindowsExtras::savePreference()
{
    ;
}

void KNWindowsExtras::loadPreference()
{
    ;
}

void KNWindowsExtras::retranslate()
{
    //Update menu items.
    m_trayIconActions[Exit]->setText(tr("Exit"));
}

void KNWindowsExtras::onActionPlayAndPause()
{
    //Emit play or pause signal according to the icon.
    if(m_isStatePlay)
    {
        emit requirePause();
        return;
    }
    emit requirePlay();
}

void KNWindowsExtras::onActionTrayIconActivate(
        QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    //When double click the tray icon,
    case QSystemTrayIcon::DoubleClick:
        //Show the main window.
        m_mainWindow->show();
        break;
    default:
        break;
    }
}

void KNWindowsExtras::onActionTrayMenuActionTriggered(int index)
{
    switch(index)
    {
    case Exit:
        //Then close the main window from the tray instance.
        m_mainWindow->forceClose();
        break;
    default:
        break;
    }
}

inline void KNWindowsExtras::setButtonIcon(const int &index,
                                           const QPixmap &icon)
{
    //Set the thumb nail for the icon.
    m_thumbnailButtons[index]->setIcon(QIcon(icon));
}
