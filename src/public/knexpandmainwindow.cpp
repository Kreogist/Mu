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
#include <QApplication>
#include <QDesktopWidget>
#include <QCloseEvent>

#include "knconfigure.h"

#include "knexpandmainwindow.h"

#include <QDebug>

KNExpandMainWindow::KNExpandMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //Set properties.
    setMinimumSize(730, 432);

    //Generate the
}

void KNExpandMainWindow::closeEvent(QCloseEvent *event)
{
    //When user is going to close the main window, check the close to icon flag.
    //If the flag is enabled, .
    if(m_closeToIcon)
    {
        //Ignore the close event.
        event->ignore();
        //Hide the main window.
        hide();
        return;
    }
    //Emit about to close signal.
    emit windowAboutToClose();
    //Save the geometry.
    backupGeometry();
    //Do the mainwindow close event.
    QMainWindow::closeEvent(event);
}

bool KNExpandMainWindow::closeToIcon() const
{
    return m_closeToIcon;
}

void KNExpandMainWindow::setCloseToIcon(bool closeToIcon)
{
    m_closeToIcon = closeToIcon;
}

void KNExpandMainWindow::setCacheConfigure(KNConfigure *cacheConfigure)
{
    m_cacheConfigure = cacheConfigure;
}

void KNExpandMainWindow::backupGeometry()
{
    //Backup the window state.
    m_cacheConfigure->setData("windowState", (int)windowState());
    //Window position.
    m_cacheConfigure->setData("windowX", geometry().x());
    m_cacheConfigure->setData("windowY", geometry().y());
    m_cacheConfigure->setData("windowWidth", geometry().width());
    m_cacheConfigure->setData("windowHeight", geometry().height());
    //Desktop size.
    m_cacheConfigure->setData("desktopWidth", QApplication::desktop()->width());
    m_cacheConfigure->setData("desktopHeight", QApplication::desktop()->height());
}

void KNExpandMainWindow::recoverGeometry()
{
    //Check is the value avaliable.
    //!FIXME: We need a better way to check is the last record is complete.
    if(m_cacheConfigure->getData("windowWidth").isNull())
    {
        return;
    }

    //Recover the window state.
    int windowState=m_cacheConfigure->getData("windowState").toInt();
    switch(windowState)
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
    //Read the last time resolution and window size.
    int lastResolutionWidth=m_cacheConfigure->getData("desktopWidth").toInt(),
        lastResolutionHeight=m_cacheConfigure->getData("desktopHeight").toInt(),
        currentResolutionWidth=QApplication::desktop()->width(),
        currentResolutionHeight=QApplication::desktop()->height(),
        lastX=m_cacheConfigure->getData("windowX").toInt(),
        lastY=m_cacheConfigure->getData("windowY").toInt(),
        lastWidth=m_cacheConfigure->getData("windowWidth").toInt(),
        lastHeight=m_cacheConfigure->getData("windowHeight").toInt();
    //Check is the resolution is the same as the last time.
    if(currentResolutionWidth==lastResolutionWidth &&
            currentResolutionHeight==lastResolutionHeight)
    {
        //Check if X or Y is joking.
        if(lastX+lastWidth<0 || lastX+lastWidth>currentResolutionWidth)
        {
            lastX=0;
        }
        if(lastY+lastHeight<0 || lastY+lastHeight>currentResolutionHeight)
        {
            lastY=0;
        }
        //In the same resolution, just change the geometry.
        setGeometry(lastX, lastY, lastWidth, lastHeight);
    }
    else
    {
        //Calculate the new geometry in the current resolution.
        setGeometry((qreal)lastX/(qreal)lastResolutionWidth*(qreal)currentResolutionWidth,
                    (qreal)lastY/(qreal)lastResolutionHeight*(qreal)currentResolutionHeight,
                    (qreal)lastWidth/(qreal)lastResolutionWidth*(qreal)currentResolutionWidth,
                    (qreal)lastHeight/(qreal)lastResolutionHeight*(qreal)currentResolutionHeight);
    }
}
