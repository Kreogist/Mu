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
#ifndef KNWINDOWSEXTRAS_H
#define KNWINDOWSEXTRAS_H

#include <QPixmap>

#include "knplatformextras.h"

class QWinThumbnailToolButton;
class QWinThumbnailToolBar;
class QWinTaskbarButton;
class QWinTaskbarProgress;
class KNWindowsExtras : public KNPlatformExtras
{
    Q_OBJECT
public:
    explicit KNWindowsExtras(QObject *parent = 0);
    void setMainWindow(QMainWindow *mainWindow);

signals:

public slots:
    void setButtonIcon(const int &index, const QPixmap &icon);
    void onActionPlayStateChanged(const bool &isPlay);

private slots:
    void onActionPlayAndPause();

private:
    inline void initialThumbnailToolBar();
    QWinThumbnailToolBar *m_thumbnailToolbar;
    QWinTaskbarButton *m_taskbarButton;
    QWinTaskbarProgress *m_taskbarProgress;
    enum ThumbnailActions
    {
        PlayPrev,
        PlayAndPause,
        PlayNext,
        ThumbnailActionsCount
    };
    bool m_isShownPlay=true;
    QPixmap m_playIcon, m_pauseIcon;
    QWinThumbnailToolButton *m_thumbnailButtons[ThumbnailActionsCount];
};

#endif // KNWINDOWSEXTRAS_H
