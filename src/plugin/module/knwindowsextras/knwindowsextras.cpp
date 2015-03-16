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
#include <QtWinExtras>

#include <QApplication>
#include <QAction>
#include <QLabel>
#include <QSignalMapper>
#include <QMainWindow>
#include <QMenu>

#include "knglobal.h"
#include "knconfigure.h"
#include "knpreferenceitempanel.h"
#include "knpreferenceitem.h"
#include "knpreferenceitemfactory.h"
#include "knexpandmainwindow.h"

#include "knwindowsextras.h"

#include <QDebug>

KNWindowsExtras::KNWindowsExtras(QObject *parent) :
    KNPlatformExtras(parent)
{
    //Initial resource.
    m_playIcon=QPixmap(":/platform/windows/play.png");
    m_pauseIcon=QPixmap(":/platform/windows/pause.png");
    m_muteOn=QPixmap(":/platform/windows/mute_on.png");
    m_muteOff=QPixmap(":/platform/windows/mute.png");

    m_loopStates[ButtonNoRepeat]=QPixmap(":/platform/windows/loopmode_norepeat.png");
    m_loopStates[ButtonRepeat]=QPixmap(":/platform/windows/loopmode_repeatsingle.png");
    m_loopStates[ButtonRepeatAll]=QPixmap(":/platform/windows/loopmode_repeat.png");
    m_loopStates[ButtonShuffle]=QPixmap(":/platform/windows/loopmode_random.png");
    //Initial toolbar.
    initialThumbnailToolBar();
    //Initial tray icon.
    initialTrayIcon();
    //Initial the preference panel.
    initialPreferenceItems();

    //Link apply preference request.
    connect(KNGlobal::instance(), &KNGlobal::requireApplyPreference,
            this, &KNWindowsExtras::loadPreference);
    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNWindowsExtras::retranslate);
    retranslate();
}

KNWindowsExtras::~KNWindowsExtras()
{
    delete m_trayIconMenu;
}

void KNWindowsExtras::setMainWindow(QMainWindow *mainWindow)
{
    //Cast main window to expand main window.
    m_mainWindow=static_cast<KNExpandMainWindow *>(mainWindow);
    //Native window hack.
    //We need to do this becuase windowHandle() only returns an available value
    //when the QWidget is a native widget. Using this hack to make the main
    //window to a native widget and the windowHandle() will return the handle.
    m_mainWindow->setAttribute(Qt::WA_NativeWindow);
    //Initial the toolbar.
    m_thumbnailToolbar->setParent(m_mainWindow);
    m_thumbnailToolbar->setWindow(m_mainWindow->windowHandle());
}

inline void KNWindowsExtras::setButtonIcon(const int &index,
                                           const QPixmap &icon)
{
    m_thumbnailButtons[index]->setIcon(QIcon(icon));
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

void KNWindowsExtras::retranslate()
{
    //Update menu items.
    m_trayIconActions[Exit]->setText(tr("Exit"));

    //Update preference items.
    m_extraPreferenceTitle->setText(tr("Tray Icon"));
    m_extraPreferenceItem[CloseToTray]->setCaption(tr("Close to tray icon"));
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

void KNWindowsExtras::onActionTrayIconActivate(const QSystemTrayIcon::ActivationReason &reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::DoubleClick:
        m_mainWindow->show();
        break;
    default:
        break;
    }
}

void KNWindowsExtras::onActionTrayMenuActionTriggered(const int &index)
{
    switch(index)
    {
    case Exit:
        //Force set close to icon false.
        m_mainWindow->setCloseToIcon(false);
        //Then close the main window.
        m_mainWindow->close();
        break;
    default:
        break;
    }
}

void KNWindowsExtras::loadPreference()
{
    m_mainWindow->setCloseToIcon(m_extraConfigure->getData("CloseToIcon",
                                                           false).toBool());
}

void KNWindowsExtras::savePreference()
{
    for(int i=0; i<PlatformPreferenceItemsCount; i++)
    {
        m_extraPreferenceItem[i]->saveValue();
    }
}

inline void KNWindowsExtras::initialThumbnailToolBar()
{
    //Initial the toolbar.
    m_thumbnailToolbar=new QWinThumbnailToolBar(this);

    //Initial the buttons.
    for(int i=0; i<ThumbnailActionsCount; i++)
    {
        m_thumbnailButtons[i]=new QWinThumbnailToolButton(m_thumbnailToolbar);
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
    setButtonIcon(PlayPrev, QPixmap(":/platform/windows/previous.png"));
    setButtonIcon(PlayNext, QPixmap(":/platform/windows/next.png"));
    setButtonIcon(VolumeUp, QPixmap(":/platform/windows/volup.png"));
    setButtonIcon(VolumeDown, QPixmap(":/platform/windows/voldown.png"));
    //Set default state button.
    onActionMuteStateChanged(false);
    onActionLoopStateChanged(ButtonNoRepeat);
    onActionPlayStateChanged(false);
}

inline void KNWindowsExtras::initialTrayIcon()
{
    //Initial the tray icon.
    m_trayIcon=new QSystemTrayIcon(this);
    m_trayIcon->setToolTip(QApplication::applicationName());
    m_trayIcon->setIcon(QIcon("://icon/mu.png"));
    connect(m_trayIcon, &QSystemTrayIcon::activated,
            this, &KNWindowsExtras::onActionTrayIconActivate);
    //Initial the tray icon menu.
    m_trayIconMenu=new QMenu;
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->show();
    //Initial the tray icon menu actions.
    QSignalMapper *actionTriggerMapper=new QSignalMapper(this);
    connect(actionTriggerMapper, SIGNAL(mapped(int)),
            this, SLOT(onActionTrayMenuActionTriggered(int)));
    for(int i=0; i<TrayIconMenuActionCount; i++)
    {
        //Initial the action and add to the menu.
        m_trayIconActions[i]=new QAction(m_trayIconMenu);
        m_trayIconMenu->addAction(m_trayIconActions[i]);
        //Link the action to the mapper.
        connect(m_trayIconActions[i], SIGNAL(triggered()),
                actionTriggerMapper, SLOT(map()));
        actionTriggerMapper->setMapping(m_trayIconActions[i], i);
    }
}

inline void KNWindowsExtras::initialPreferenceItems()
{
    //Initial the configure.
    m_extraConfigure=new KNConfigure(this);
    m_extraConfigure->setCaption("Windows Extras");
    KNGlobal::instance()->systemConfigure()->addSubConfigure(m_extraConfigure);
    //Initial the items.
    m_extraPreferenceItem[CloseToTray]=
            KNPreferenceItemFactory::create(SwitcherItem,
                                            "CloseToIcon",
                                            m_extraConfigure,
                                            false);
    //Add the items to the panel.
    m_extraPreferenceTitle=KNPreferenceItemFactory::createTitle();
    KNPreferenceItemPanel *generalPanel=
            KNGlobal::instance()->generalPreferencePanel();
    generalPanel->addTitle(m_extraPreferenceTitle);
    generalPanel->addItem(m_extraPreferenceItem[CloseToTray]);
}
