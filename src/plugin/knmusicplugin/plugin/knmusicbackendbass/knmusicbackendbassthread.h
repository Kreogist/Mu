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

#ifndef KNMUSICBACKENDBASSTHREAD_H
#define KNMUSICBACKENDBASSTHREAD_H

#include "bass.h"

#include "knmusicutil.h"

#include "knmusicstandardbackendthread.h"

using namespace MusicUtil;

/*!
 * \brief The KNMusicBackendBassThread class is the standard playing thread for
 * the bass backend. It should be constructed only in the KNMusicBackendBass.
 */
class KNMusicBackendBassThread : public KNMusicStandardBackendThread
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendBassThread object.
     * \param parent The KNMusicBackendBass object.
     */
    explicit KNMusicBackendBassThread(QObject *parent = 0);
    ~KNMusicBackendBassThread();

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

#ifdef Q_OS_WIN64

    HSTREAM getMixerHandle() const;
#endif

signals:
    /*!
     * \brief This signal is used only for threadReachesEnd(). It's used for
     * avoid the 'QObject::killTimer: Timers cannot be stopped from another
     * thread' bug.
     */
    void reachesFinished();

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

    /*!
     * \brief Set the flags when using at creating a stream.
     * \param channelFlags The new channel flags.
     */
    void setCreateFlags(const DWORD &channelFlags,
                        const float &useBuffer);

#ifdef Q_OS_WIN64
    /*!
     * \brief Set the WASAPI playing data.
     * \param enabled Whether the WASAPI is enabled.
     * \param outputDevice The output device index.
     * \param wasapiFlag The stream build flag.
     */
    void setWasapiData(bool enabled, int outputDevice, DWORD wasapiFlag);
#endif

private slots:
    void checkPosition();

private:
    static void CALLBACK threadReachesEnd(HSYNC handle,
                                          DWORD channel,
                                          DWORD data,
                                          void *user);
#ifdef Q_OS_WIN64
    // WASAPI function
    static DWORD CALLBACK wasapiProcess(void *buffer, DWORD length, void *user);
#endif
    inline void finishPlaying();
    inline void resetChannelDuration()
    {
        //Set the duration to the total duration.
        m_duration=m_totalDuration;
        //Set the start position at the very beginning.
        m_startPosition=0;
        //Set the default end position as the whole file.
        m_endPosition=m_duration;
    }

    inline void setPlayingState(const int &state);
    inline void setChannelSyncs()
    {
        m_syncHandlers.append(BASS_ChannelSetSync(m_channel,
                                                  BASS_SYNC_END,
                                                  0,
                                                  threadReachesEnd,
                                                  this));
    }

    inline void removeChannelSyncs();
    inline qint64 getChannelPosition()
    {
        //Get the current position
        return (qint64)
                (BASS_ChannelBytes2Seconds(
                     m_channel,
                     BASS_ChannelGetPosition(m_channel, BASS_POS_BYTE))
                 //Translate the unit from second to msecond, remove the start
                 //position msecond.
                 *1000)-m_startPosition;
    }
    inline qreal getChannelVolume();
    inline void freeChannel();
    inline bool loadBassThread(const QString &filePath);
#ifdef Q_OS_WIN64
    inline bool initialWasapi();
#endif

    //Channel data.
    QString m_filePath;
    DWORD m_channel;
    DWORD m_channelFlags;

    //Channel information.
    qint64 m_totalDuration,
           m_duration,
           m_startPosition,
           m_endPosition;
    qint64 m_savedPosition;
    qreal m_volume;
    float m_useBuffer;
    int m_state;
#ifdef Q_OS_WIN64
    HSTREAM m_mixer;
    int m_wasapiOutputDevice;
    DWORD m_wasapiFlag;
    bool m_wasapiEnabled;
#endif

    //Updater.
    QTimer *m_positionUpdater;

    //Sync Handlers.
    QList<HSYNC> m_syncHandlers;
};

#endif // KNMUSICBACKENDBASSTHREAD_H
