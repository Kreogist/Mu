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

#ifndef KNMUSICBACKENDGSTREAMER_H
#define KNMUSICBACKENDGSTREAMER_H

/*
 * * Nov 27, 2015
 *   This is the new standard module under Linux.
 *   libmpv would only be used under Linux. Actually, it could be used on both
 * Windows and Mac OS X. However, the bass backend will still keep be used.
 */

#include "knmusicstandardbackend.h"

class KNConfigure;
class KNMusicBackendMpvThread;
/*!
 * \brief The KNMusicBackendGStreamer class provides you a backend interface of
 * MPV player.\n
 * Basically, this module would keep tracking of the latest version of MPV. It
 * will still be compatible with the version of the Ubuntu LTS version.
 */
class KNMusicBackendMpv : public KNMusicStandardBackend
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendMpv object with parent object.
     * \param parent The parent object.
     */
    explicit KNMusicBackendMpv(QObject *parent = 0);
    ~KNMusicBackendMpv();

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::loadMusic().
     */
    bool loadMusic(const QString &filePath,
                   const qint64 &start=-1,
                   const qint64 &duration=-1) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::previewLoadMusic().
     */
    bool previewLoadMusic(const QString &filePath,
                          const qint64 &start=-1,
                          const qint64 &duration=-1) Q_DECL_OVERRIDE;

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

    /*!
     * \brief Reimplemented from KNMusicStandardBackend::deviceList().
     */
    QJsonArray deviceList() Q_DECL_OVERRIDE;

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
    KNConfigure *m_systemConfigure, *m_userConfigure;
    KNMusicBackendMpvThread *m_main, *m_preview;
};

#endif // KNMUSICBACKENDGSTREAMER_H
