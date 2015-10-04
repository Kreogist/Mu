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

#ifndef KNPLATFORMEXTRAS_H
#define KNPLATFORMEXTRAS_H

#include <QObject>

class QMainWindow;
class KNPlatformExtras : public QObject
{
    Q_OBJECT
public:
    enum LoopStateButtonState
    {
        ButtonNoRepeat,
        ButtonRepeat,
        ButtonRepeatAll,
        ButtonShuffle,
        LoopStateButtonStateCount
    };
    KNPlatformExtras(QObject *parent = 0): QObject(parent){}
    virtual void setMainWindow(QMainWindow *mainWindow)=0;

signals:
    void requirePlay();
    void requirePause();
    void requirePlayNext();
    void requirePlayPrev();
    void requireVolumeUp();
    void requireVolumeDown();
    void requireChangeMuteState();
    void requireChangeLoopState();

public slots:
    virtual void onActionPlayStateChanged(const bool &isPlay)=0;
    virtual void onActionMuteStateChanged(const bool &isMute)=0;
    virtual void onActionLoopStateChanged(const int &loopState)=0;
    virtual void loadPreference()=0;
    virtual void savePreference()=0;
};

#endif // KNPLATFORMEXTRAS_H
