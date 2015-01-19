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
#ifndef KNMUSICBACKENDBASS_H
#define KNMUSICBACKENDBASS_H

/*
 * Oct 2nd, 2014:
 *    This is only a optional module for Windows and Mac OS X. Because bass is
 * not a open source project, it's only free for non-commercial use. But it
 * works really nice on Windows. It even support ASIO on Windows.
 *    So, we are going to use bass lib on Windows and Mac OS X as an optional
 * choice. But don't expect it can have many optional choice. Though it may be
 * 'optional' enough.
 *    This module can also be used on Linux, but you should configure the binary
 * package yourself. And it works really bad under Ubuntu Linux 14.04 LTS in
 * VirtualBox 4.13. Don't enable this module under Linux.
 */

#include "bass.h"

#include "knmusicstandardbackend.h"

class KNMusicBackendBassThread;
class KNMusicBackendBass : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    explicit KNMusicBackendBass(QObject *parent = 0);
    ~KNMusicBackendBass();
    bool available();
    int volume() const;

    void loadUrl(const QString &url);

    int volumeMinimal();
    int volumeMaximum();

signals:

public slots:

protected:
    void changeVolume(const int &volumeSize);

private:
    bool initialBass();
    void initialPlugin(const QString &pluginDirPath);
    KNMusicBackendBassThread *m_main, *m_preview;
    bool m_available=false;
};

#endif // KNMUSICBACKENDBASS_H
