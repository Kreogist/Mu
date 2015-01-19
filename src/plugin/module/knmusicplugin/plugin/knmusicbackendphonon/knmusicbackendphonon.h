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
#ifndef KNMUSICBACKENDPHONON_H
#define KNMUSICBACKENDPHONON_H

/*
 * Jan 15th, 2015
 *     This is the prefer module under Linux - Phonon4Qt5 (Phonon for Qt5).
 *     You should find this package easily in your package manager. Install the
 * following package(s):
 *
 *      phonon
 *      phonon-backend-gstreamer (or any other phonon backend)
 *      libphonon4
 *      phonon4qt5
 *      phonon4qt5-backend-vlc (or any other phonon4qt5 backend)
 *      libphonon4qt5-4
 *      libphonon4qt5-dev
 *
 *     On Ubuntu Linux 14.04 LTS, Synaptic (or called Xin Li De in Chinese
 *  Pinyin) provides all the packages. It should be easy for other Linux
 *  Releases to install these packages via package manager.
 */

#include "knmusicstandardbackend.h"

class KNMusicBackendPhononThread;
class KNMusicBackendPhonon : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    explicit KNMusicBackendPhonon(QObject *parent = 0);
    ~KNMusicBackendPhonon();

    bool available();

    int volume() const;

    void loadUrl(const QString &url);

    int volumeMinimal();
    int volumeMaximum();

protected:
    void changeVolume(const int &volumeSize);
    qreal smartVolumeScale() const;

private:
    KNMusicBackendPhononThread *m_main, *m_preview;
};

#endif // KNMUSICBACKENDPHONON_H
