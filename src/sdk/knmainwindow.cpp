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
#include <QApplication>
#include <QDesktopWidget>
#include <QAction>

#include "knglobal.h"
#include "knconfigure.h"
#include "kncategoryplugin.h"
#include "knpreferenceplugin.h"
#include "knthememanager.h"
#include "knmainwindowcontainer.h"

//Ports
#include "knmainwindowheaderbase.h"

#include "knmainwindow.h"

#include <QDebug>

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_cacheConfigure(knGlobal->cacheConfigure()->getConfigure("MainWindow")),
    m_container(new KNMainWindowContainer(this)),
    m_categoryPlugin(nullptr),
    m_originalWindowState(Qt::WindowNoState)
{
    setObjectName("MainWindow");
    //Set properties.
    setAutoFillBackground(true);
    setCentralWidget(m_container);
    setContentsMargins(0,0,0,0);
    setMinimumSize(730, 432);
    setWindowIcon(QIcon("://icon/mu.png"));
    //Mac OS X title hack.
#ifdef Q_OS_MACX
    setWindowTitle(qApp->applicationDisplayName());
#endif
    //Add main window to theme list.
    knTheme->registerWidget(this);
    //Add full screen short cut actions.
    QAction *fullScreen=new QAction(this);
    fullScreen->setShortcut(QKeySequence(QKeySequence::FullScreen));
    connect(fullScreen, &QAction::triggered,
            this, &KNMainWindow::onActionFullScreen);
    addAction(fullScreen);
    //Recover the geometry.
    recoverGeometry();
}

void KNMainWindow::setHeader(KNMainWindowHeaderBase *header)
{
    //Set the header widget.
    m_container->setHeader(header);
    //Link the header show preference signal to container.
    connect(header, &KNMainWindowHeaderBase::requireShowPreference,
            m_container, &KNMainWindowContainer::showPreference);
}

void KNMainWindow::setMainWidget(KNCategoryPlugin *mainWidget)
{
    //Save the main widget.
    m_categoryPlugin=mainWidget;
    //Set the new category plugin.
    m_container->setMainWidget(m_categoryPlugin);
}

void KNMainWindow::setPreferencePanel(KNPreferencePlugin *preferencePanel)
{
    //Set the preference widget.
    m_container->setPreferencePanel(preferencePanel);
    //Link the hide preference request.
    connect(preferencePanel, &KNPreferencePlugin::requireClosePreference,
            m_container, &KNMainWindowContainer::hidePreference);
}

void KNMainWindow::setMainPlayer(QWidget *mainPlayer)
{
    //Set the main player widget.
    m_container->setMainPlayer(mainPlayer);
}

void KNMainWindow::showMainPlayer()
{
    //Set the container to show the main player.
    m_container->showMainPlayer();
}

void KNMainWindow::hideMainPlayer()
{
    //Hide the main player.
    m_container->hideMainPlayer();
}

void KNMainWindow::showEvent(QShowEvent *event)
{
    //Emit the main window show signal.
    emit mainWindowShown();
    //Show the main window.
    QMainWindow::showEvent(event);
}

void KNMainWindow::closeEvent(QCloseEvent *event)
{
    //Save the configure of the category plugin, if the category is valid.
    if(m_categoryPlugin)
    {
        //Save the configure.
        m_categoryPlugin->saveConfigure();
    }
    //Save the geometry.
    backupGeometry();
    //Do the mainwindow close event.
    QMainWindow::closeEvent(event);
}

void KNMainWindow::onActionFullScreen()
{
    //Check out the full screen state.
    if(isFullScreen())
    {
        //Check the original window state.
        if(m_originalWindowState==Qt::WindowFullScreen)
        {
            //Set it to be no state.
            m_originalWindowState=Qt::WindowNoState;
        }
        //Set the window to normal state.
        setWindowState(m_originalWindowState);
    }
    else
    {
        //Save the original window state.
        m_originalWindowState=windowState();
        //Full screen the window.
        setWindowState(Qt::WindowFullScreen);
    }
}

inline void KNMainWindow::recoverGeometry()
{
    //Check is the last record is complete. If there's no windowWidth property
    //in the configure, means we don't save the last geometry. Ignore the
    //recover request.
    if(m_cacheConfigure->data("windowWidth").isNull())
    {
        return;
    }

    //Recover the window state.
    int windowState=m_cacheConfigure->data("windowState").toInt();
    //For maximum and full screen, we only need to set the window state.
    switch (windowState)
    {
    case Qt::WindowMaximized:
        setWindowState(Qt::WindowMaximized);
        return;
    case Qt::WindowFullScreen:
        setWindowState(Qt::WindowFullScreen);
        return;
    default:
        setWindowState(Qt::WindowNoState);
    }
    //Read the resolution data of the last time closed.
    int lastScreenWidth=getCacheValue("desktopWidth"),
        lastScreenHeight=getCacheValue("desktopHeight"),
        currentScreenWidth=qApp->desktop()->width(),
        currentScreenHeight=qApp->desktop()->height(),
            lastX=getCacheValue("windowX"),
            lastY=getCacheValue("windowY"),
            lastWidth=getCacheValue("windowWidth"),
            lastHeight=getCacheValue("windowHeight");
    //Check is the resolution is the same as the last closed time.
    if(!(lastScreenWidth==currentScreenWidth &&
         lastScreenHeight==currentScreenHeight))
    {
        //The resolution has been changed, recalculate the size parameters.
        //Get the width ratio and the height ratio.
        qreal widthRatio=(qreal)currentScreenWidth/(qreal)lastScreenWidth,
              heightRatio=(qreal)currentScreenHeight/(qreal)lastScreenHeight;
        //Recalculate the last parameters.
        zoomParameter(lastX, widthRatio);
        zoomParameter(lastY, heightRatio);
        zoomParameter(lastWidth, widthRatio);
        zoomParameter(lastHeight, heightRatio);
    }
    //Check the parameter, ensure that one part of the window must be inside
    //screen.
    //If it's not inside the screen, make the top of the window 0.
    if(lastY<0 || lastY>currentScreenHeight)
    {
        lastY=0;
    }
    //Set the geometry.
    setGeometry(lastX, lastY, lastWidth, lastHeight);
}

inline void KNMainWindow::backupGeometry()
{
    //Set the window state.
    setCacheValue("windowState", static_cast<int>(windowState()));
    //Set the window position.
    setCacheValue("windowX", geometry().x());
    setCacheValue("windowY", geometry().y());
    setCacheValue("windowWidth", geometry().width());
    setCacheValue("windowHeight", geometry().height());
    //Set the current desktop size.
    setCacheValue("desktopWidth", qApp->desktop()->width());
    setCacheValue("desktopHeight", qApp->desktop()->height());
}

inline int KNMainWindow::getCacheValue(const QString &valueName)
{
    return m_cacheConfigure->data(valueName).toInt();
}

inline void KNMainWindow::setCacheValue(const QString &valueName,
                                        const int &value)
{
    m_cacheConfigure->setData(valueName, value);
}

inline void KNMainWindow::zoomParameter(int &parameter, const qreal &ratio)
{
    parameter=(qreal)parameter*ratio;
}
