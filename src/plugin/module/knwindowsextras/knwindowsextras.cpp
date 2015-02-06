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
#include <QMainWindow>
#include <QtWinExtras>

#include "knwindowsextras.h"

#include <QDebug>

KNWindowsExtras::KNWindowsExtras(QObject *parent) :
    KNPlatformExtras(parent)
{
    //Initial resource.
    m_playIcon=QPixmap(":/platform/windows/play.png");
    m_pauseIcon=QPixmap(":/platform/windows/pause.png");
    //Initial toolbar.
    initialThumbnailToolBar();
}

void KNWindowsExtras::setMainWindow(QMainWindow *mainWindow)
{
    //Native window hack.
    //We need to do this becuase windowHandle() only returns an available value
    //when the QWidget is a native widget. Using this hack to make the main
    //window to a native widget and the windowHandle() will return the handle.
    mainWindow->setAttribute(Qt::WA_NativeWindow);
    //Initial the toolbar.
    m_thumbnailToolbar->setParent(mainWindow);
    m_thumbnailToolbar->setWindow(mainWindow->windowHandle());
}

void KNWindowsExtras::setButtonIcon(const int &index, const QPixmap &icon)
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

void KNWindowsExtras::initialThumbnailToolBar()
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
    //Set icon.
    setButtonIcon(PlayPrev, QPixmap(":/platform/windows/previous.png"));
    setButtonIcon(PlayNext, QPixmap(":/platform/windows/next.png"));
    onActionPlayStateChanged(false);
}
