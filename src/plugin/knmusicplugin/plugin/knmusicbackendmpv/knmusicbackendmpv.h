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

#ifndef KNMUSICBACKENDMPV_H
#define KNMUSICBACKENDMPV_H

#include <QThread>

#include "knmusicstandardbackend.h"

class KNMusicBackendMpvThread;
class KNMusicBackendMpv : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    explicit KNMusicBackendMpv(QObject *parent = 0);
    ~KNMusicBackendMpv();

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
    QThread m_mainThread, m_previewThread;
    KNMusicBackendMpvThread *m_main, *m_preview;
};

#endif // KNMUSICBACKENDMPV_H
