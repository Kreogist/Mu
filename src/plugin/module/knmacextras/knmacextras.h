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
#ifndef KNMACEXTRAS_H
#define KNMACEXTRAS_H

#include <QSystemTrayIcon>

#include "knplatformextras.h"

class QMenu;
class KNMacExtras : public KNPlatformExtras
{
    Q_OBJECT
public:
    explicit KNMacExtras(QObject *parent = 0);
    ~KNMacExtras();
    void setMainWindow(QMainWindow *mainWindow);

signals:

public slots:
    void onActionPlayStateChanged(const bool &isPlay);
    void onActionMuteStateChanged(const bool &isMute);
    void onActionLoopStateChanged(const int &loopState);

private slots:
    void prevActivated(const QSystemTrayIcon::ActivationReason &reason);
    void playNPauseActivated(const QSystemTrayIcon::ActivationReason &reason);
    void nextActivated(const QSystemTrayIcon::ActivationReason &reason);

private:
    inline void initialTrayIconControls();

    enum ThumbnailActions
    {
        PlayPrev,
        PlayAndPause,
        PlayNext,
        ThumbnailActionsCount
    };
    QSystemTrayIcon *m_actionButtons[ThumbnailActionsCount];
    QMenu *m_dummyMenu;
    QIcon m_playIcon, m_pauseIcon;
    bool m_isPlay=false;
};

#endif // KNMACEXTRAS_H
