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

#include "knmusicstandardbackend.h"

/*
 * Sept 16th, 2015
 *   I rewrite this module, with the following packages we can compile this
 * plugin on Ubuntu Linux 15.04:
 *
 *      libphonon4qt5-dev
 *      phonon4qt5-backend-gstreamer
 *
 *   The following packages will be installed automatically.
 *
 *      phonon4qt5
 *      phonon4qt5-backend-null
 *
 *   You need a backend for phonon to work. Nearly all the Linux Distributions
 * have installed gstreamer 1.0. Now we don't need the vlc anymore.
 *   We won't even need phonon itself anymore. What we need is only phonon4qt5,
 * and a useful backend. The codec you have to configure by yourself.
 *   Which make me shocked, that now it will ask the system to download a codec
 * for it.
 *
 * Jan 15th, 2015
 *   This is the prefer module under Linux - Phonon4Qt5 (Phonon for Qt5).
 *   You should find this package easily in your package manager. Install the
 * following packages:
 *
 *      phonon
 *      phonon-backend-gstreamer (or any other phonon backend)
 *      libphonon4
 *      phonon4qt5
 *      phonon4qt5-backend-vlc (or any other phonon4qt5 backend)
 *      libphonon4qt5-4
 *      libphonon4qt5-dev
 *
 *   On Ubuntu Linux 14.04 LTS, Synaptic (or called Xin Li De in Chinese
 * Pinyin) provides all the packages. It should be easy for other Linux
 * Releases to install these packages via package manager.
 */
class KNMusicBackendPhononThread;
/*!
 * \brief The KNMusicBackendPhonon class
 */
class KNMusicBackendPhonon : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendPhonon object.
     * \param parent The parent object.
     */
    explicit KNMusicBackendPhonon(QObject *parent = 0);

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
    KNMusicBackendPhononThread *m_main, *m_preview;
};

#endif // KNMUSICBACKENDPHONON_H
