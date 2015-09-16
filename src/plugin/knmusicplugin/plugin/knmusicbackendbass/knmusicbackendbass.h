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
 * * July 24th, 2015:
 *    This module has a known bug: it cannot play the complete m4a file under
 * Mac OS X for some reason. I hope the latest version of bass can solve this
 * bug.
 *
 * Oct 2nd, 2014:
 *    This is only a optional module for Windows and Mac OS X. Because bass is
 * not a open source project, it's only free for non-commercial use. But it
 * works really nice on Windows. It even support ASIO on Windows.
 *    So, we are going to use bass lib on Windows and Mac OS X as an optional
 * choice. But don't expect it can have many optional choice. Though it may be
 * 'optional' enough.
 *    This module can also be used on Linux, but you should configure the binary
 * package yourself. And it works really bad under Ubuntu Linux 14.04 LTS under
 * VirtualBox 4.13. In one word, don't enable this module under Linux.
 */
#include "bass.h"

#include "knmusicstandardbackend.h"

class KNMusicBackendBassThread;
/*!
 * \brief The KNMusicBackendBass class provides you a backend interface of the
 * backend bass from un4seen.
 */
class KNMusicBackendBass : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendBassclass with given parent.
     * \param parent The parent object.
     */
    explicit KNMusicBackendBass(QObject *parent = 0);
    ~KNMusicBackendBass();

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::volume().
     */
    int volume() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::minimalVolume().
     */
    int minimalVolume() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::maximumVolume().
     */
    int maximumVolume() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMusicStandardBackend::setGlobalVolume().
     */
    void setGlobalVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::volumeLevel().
     */
    int volumeLevel() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::smartVolumeScale().
     */
    qreal smartVolumeScale() const Q_DECL_OVERRIDE;

private:
    inline bool initialBass(DWORD &channelFlags);
    inline void loadPlugin(const QString &dirPath);
    inline KNMusicBackendBassThread *generateThread(const DWORD &channelFlags);

    QList<HPLUGIN> m_pluginList;
};

#endif // KNMUSICBACKENDBASS_H
