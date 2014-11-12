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
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>
#include <QWinThumbnailToolBar>
#include <QWinThumbnailToolButton>

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
    m_thumbnailToolbar->setWindow(mainWindow->windowHandle());
}

void KNWindowsExtras::setButtonIcon(const int &index, const QPixmap &icon)
{
    m_thumbnailButtons[index]->setIcon(QIcon(icon));
}

void KNWindowsExtras::onActionPlayStateChanged(const bool &isPlay)
{
    //Save the state.
    m_isShownPlay=isPlay;
    //Set the icon according to the state.
    setButtonIcon(PlayAndPause, m_isShownPlay?m_playIcon:m_pauseIcon);
}

void KNWindowsExtras::onActionPlayAndPause()
{
    //Emit play or pause signal according to the icon.
    if(m_isShownPlay)
    {
        emit requirePlay();
        return;
    }
    emit requirePause();
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
}
