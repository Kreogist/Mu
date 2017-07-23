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

#ifndef KNMUSICBACKENDGSTREAMERTHREAD_H
#define KNMUSICBACKENDGSTREAMERTHREAD_H

#include <mpv/client.h>

#include "knmusicglobal.h"

#include "knmusicstandardbackendthread.h"

/*!
 * \brief The KNMusicBackendMpvThread class is the standard playing thread for
 * the mpv backend provided as official plugin.\n
 * It should only be used and constructed by KNMusicBackendMpv.
 */
class KNMusicBackendMpvThread : public KNMusicStandardBackendThread
{
    Q_OBJECT
public:
    /*!
     * \brief Construct KNMusicBackendMpvThread object.
     * \param parent The parent object.
     */
    explicit KNMusicBackendMpvThread(QObject *parent = 0);
    ~KNMusicBackendMpvThread();

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::loadFile().
     */
    bool loadFile(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::stop().
     */
    void stop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::play().
     */
    void play() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::pause().
     */
    void pause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::volume().
     */
    int volume() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::duration().
     * \return
     */
    qint64 duration() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::position().
     */
    qint64 position() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::state().
     */
    int state() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPlaySection().
     */
    void setPlaySection(const qint64 &start=-1,
                        const qint64 &duration=-1) Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief When MPV needs to wake up for event, this signal need to be
     * called.
     */
    void mpvEvent();

public slots:
    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::save().
     */
    void save() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::restore().
     */
    void restore(const QString &updatedFilePath=QString()) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setVolume().
     */
    void setVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPosition().
     */
    void setPosition(const qint64 &position) Q_DECL_OVERRIDE;

private slots:
    void onMpvEvent();

private:
    inline void setPlayingState(int state);
    inline void updateStartAndEndPosition();
    inline bool loadMusicFile(const QString &filePath);
    inline bool buildMpvHandle();
    inline void clearMpvHandle();
    inline void finishPlaying();
    QString m_filePath;
    KNConfigure *m_playbackConfigure;
    QWidget *m_container;
    mpv_handle *m_mpvHandle;
    qint64 m_startPosition,
           m_endPosition,
           m_duration,
           m_totalDuration,
           m_position;
    int m_state, m_volume;
    bool m_sectionSet, m_fileLoaded, m_restoreFlag;
};

#endif // KNMUSICBACKENDGSTREAMERTHREAD_H
