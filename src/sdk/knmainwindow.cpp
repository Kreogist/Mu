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

#include "knglobal.h"
#include "knconfigure.h"
#include "knthememanager.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_cacheConfigure(knGlobal->cacheConfigure()->getConfigure("MainWindow"))
{
    setObjectName("MainWindow");
    //Set properties.
    setMinimumSize(730, 432);
    setWindowIcon(QIcon("://icon/mu.png"));
#ifdef Q_OS_MACX
    //Mac OS X title hack.
    setWindowTitle(qApp->applicationDisplayName());
#endif
    //Add main window to theme list.
    THEME_THIS_WIDGET;
    //Recover the geometry.
    recoverGeometry();
}

void KNMainWindow::recoverGeometry()
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
    setGeometry(lastX, lastY, lastWidth, lastHeight);
}

inline int KNMainWindow::getCacheValue(const QString &valueName)
{
    return m_cacheConfigure->data(valueName).toInt();
}

inline void KNMainWindow::zoomParameter(int &parameter, const qreal &ratio)
{
    parameter=(qreal)parameter*ratio;
}
