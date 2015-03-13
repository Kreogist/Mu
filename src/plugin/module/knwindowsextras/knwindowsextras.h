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
#include <QSystemTrayIcon>

#include "knplatformextras.h"

class QMenu;
class QAction;
class QLabel;
class QWinThumbnailToolButton;
class QWinThumbnailToolBar;
class QWinTaskbarButton;
class QWinTaskbarProgress;
class KNConfigure;
class KNPreferenceItem;
class KNWindowsExtras : public KNPlatformExtras
{
    Q_OBJECT
public:
    explicit KNWindowsExtras(QObject *parent = 0);
    ~KNWindowsExtras();
    void setMainWindow(QMainWindow *mainWindow);

signals:

public slots:
    void onActionPlayStateChanged(const bool &isPlay);
    void onActionMuteStateChanged(const bool &isMute);
    void onActionLoopStateChanged(const int &loopState);

private slots:
    void retranslate();
    void onActionPlayAndPause();
    void onActionTrayIconActivate(const QSystemTrayIcon::ActivationReason &reason);
    void onActionTrayMenuActionTriggered(const int &index);

private:
    inline void setButtonIcon(const int &index, const QPixmap &icon);
    inline void initialThumbnailToolBar();
    inline void initialTrayIcon();
    inline void initialPreferenceItems();
    QWinThumbnailToolBar *m_thumbnailToolbar;
    QWinTaskbarButton *m_taskbarButton;
    QWinTaskbarProgress *m_taskbarProgress;
    enum ThumbnailActions
    {
        PlayPrev,
        PlayAndPause,
        PlayNext,
        VolumeUp,
        VolumeDown,
        Mute,
        LoopMode,
        ThumbnailActionsCount
    };
    bool m_isStatePlay=true;
    QPixmap m_playIcon, m_pauseIcon, m_muteOn, m_muteOff,
            m_loopStates[LoopStateButtonStateCount];
    QWinThumbnailToolButton *m_thumbnailButtons[ThumbnailActionsCount];
    QSystemTrayIcon *m_trayIcon;
    enum TrayIconMenuActions
    {
        Exit,
        TrayIconMenuActionCount
    };
    QAction *m_trayIconActions[TrayIconMenuActionCount];
    QMenu *m_trayIconMenu;
    QMainWindow *m_mainWindow;
    enum PlatformPreferenceItems
    {
        MinimizeToTray,
        CloseToTray,
        PlatformPreferenceItemsCount
    };
    QLabel *m_extraPreferenceTitle;
    KNPreferenceItem *m_extraPreferenceItem[PlatformPreferenceItemsCount];
    KNConfigure *m_extraConfigure;
};

#endif // KNWINDOWSEXTRAS_H
