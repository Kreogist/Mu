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
#include <QTimer>
#include <QUrl>

#ifdef Q_OS_WIN64
#include "basswasapi.h"
#include "bassmix.h"
#endif

#include "knmusicglobal.h"

#include "knmusicbackendbassthread.h"

#include <QDebug>

KNMusicBackendBassThread::KNMusicBackendBassThread(QObject *parent) :
    KNMusicStandardBackendThread(parent),
    m_filePath(QString()),
    m_channel(0),
    m_channelFlags(0),
    m_totalDuration(-1),
    m_duration(-1),
    m_startPosition(-1),
    m_endPosition(-1),
    m_savedPosition(-1),
    m_volume(1.0),
    m_state(Stopped),
    #ifdef Q_OS_WIN64
    m_mixer(0),
    m_wasapiOutputDevice(-1),
    m_wasapiFlag(0),
    m_wasapiEnabled(false),
    #endif
    m_positionUpdater(new QTimer(this)),
    m_syncHandlers(QList<HSYNC>())
{
    //Configure the position updater.
    m_positionUpdater->setInterval(50);
    connect(m_positionUpdater, &QTimer::timeout,
            this, &KNMusicBackendBassThread::checkPosition);
    //Link the reachesFinished) signal.
    connect(this, &KNMusicBackendBassThread::reachesFinished,
            this, &KNMusicBackendBassThread::finishPlaying);
}

KNMusicBackendBassThread::~KNMusicBackendBassThread()
{
    //Stop the position updater.
    m_positionUpdater->stop();
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Free the channel.
    freeChannel();
}

bool KNMusicBackendBassThread::loadFile(const QString &filePath)
{
    //Stop the current thread first.
    stop();
    //Check is the file the current file.
    if(filePath==m_filePath)
    {
        //Reset the current state.
        resetChannelDuration();
        //Update the duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
        //Mission complete.
        return true;
    }
    //Clear up all the previous data.
    reset();
    //Load the file path.
    if(!loadBassThread(filePath))
    {
        //Load the file failed.
        return false;
    }
    //When loading the file complete, load the channel info to the thread.
    //Get the duration of the whole file.
    m_totalDuration=
            BASS_ChannelBytes2Seconds(m_channel,
                                      BASS_ChannelGetLength(m_channel,
                                                            BASS_POS_BYTE))
            *1000.0;
#ifdef Q_OS_WIN64
    if(m_wasapiEnabled)
    {
        // For 64-bit Windows platform, the WASAPI need to be initialized here.
        initialWasapi();
    }
#endif
    //Emit the duration changed signal.
    emit durationChanged(m_totalDuration);
    //Reset the thread information.
    resetChannelDuration();
    //Load complete.
    return true;
}

void KNMusicBackendBassThread::reset()
{
    //Stop the position updater.
    m_positionUpdater->stop();
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
    //Reset the channel data.
    m_filePath.clear();
    //Reset the total duration.
    m_totalDuration=-1;
    //Reset the stream status data.
    resetChannelDuration();
    //Check the current state is stopped or not.
    setPlayingState(Stopped);
}

void KNMusicBackendBassThread::stop()
{
    //Check:
    // 1. The state is already stopped.
    // 2. The channel is null.
    if(m_state==Stopped || (!m_channel))
    {
        return;
    }
    //Stop the channel.
    BASS_ChannelStop(m_channel);
    //Stop the position updater.
    m_positionUpdater->stop();
    //Reset the position to the start position.
    setPosition(0);
    //Update the state.
    setPlayingState(Stopped);
    //Emit stopped signal.
    emit stopped();
}

void KNMusicBackendBassThread::play()
{
    //Check:
    // 1. The state is already playing.
    // 2. The channel is null.
    if(m_state==Playing || (!m_channel))
    {
        return;
    }
    //Check the playing state before.
    if(m_state==Stopped)
    {
        //Check out the duration of the song.
        if(m_duration==0)
        {
            //Finished playing, to on action finished.
            finishPlaying();
            //Mission complete.
            return;
        }
        //Reset the position to fit track playing.
        setPosition(0);
        //Set the volume to the last volume, because of the reset, the
        //volume is back to 1.0.
        BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    }
    //Start the position updater.
    m_positionUpdater->start();
#ifdef Q_OS_WIN64
    if(m_wasapiEnabled)
    {
        //Start the WASAPI playing.
        BASS_WASAPI_Start();
    }
    else
    {
#endif
        //Play the thread.
        BASS_ChannelPlay(m_channel, FALSE);
#ifdef Q_OS_WIN64
    }
#endif
    //Update the state.
    setPlayingState(Playing);
}

void KNMusicBackendBassThread::pause()
{
    //Check:
    // 1. The state is already paused.
    // 2. The channel is null.
    if(m_state==Paused || (!m_channel))
    {
        return;
    }
#ifdef Q_OS_WIN64
    if(m_wasapiEnabled)
    {
        //Stop the WASAPI playing.
        BASS_WASAPI_Stop(false);
    }
    else
    {
#endif
        //Pause the thread first.
        BASS_ChannelPause(m_channel);
#ifdef Q_OS_WIN64
    }
#endif
    //Stop the updater.
    m_positionUpdater->stop();
    //Reset the state.
    setPlayingState(Paused);
}

int KNMusicBackendBassThread::volume()
{
    //Scale the float number.
    return (int)(getChannelVolume()*100.0);
}

qint64 KNMusicBackendBassThread::duration()
{
    return m_duration;
}

qint64 KNMusicBackendBassThread::position()
{
    return getChannelPosition();
}

int KNMusicBackendBassThread::state() const
{
    return m_state;
}

void KNMusicBackendBassThread::setPlaySection(const qint64 &start,
                                              const qint64 &duration)
{
    //Check the validation of the start position.
    // 1. Start is not null(-1).
    // 2. Start is lesser than duration.
    if(start!=-1 && start<m_duration)
    {
        //Save the start position.
        m_startPosition=start;
        //Check the validation of the duration.
        if(duration==-1 || start+duration>m_duration)
        {
            //We will treat the invalid duration when the start is valid to
            //play to the end of the file.
            m_duration-=m_startPosition;
        }
        else
        {
            //Or else, save the new duration.
            m_duration=duration;
        }
        //Update the end position.
        m_endPosition=m_startPosition+m_duration;
        //Emit the new duration.
        emit durationChanged(m_duration);
        //Emit the load success signal.
        emit loadSuccess();
    }
}

void KNMusicBackendBassThread::save()
{
#ifdef Q_OS_WIN64
    if(m_wasapiEnabled)
    {
        //Stop the WASAPI playing.
        BASS_WASAPI_Stop(false);
    }
    else
    {
#endif
        //Pause the thread first.
        BASS_ChannelPause(m_channel);
#ifdef Q_OS_WIN64
    }
#endif
    //Stop the updater.
    m_positionUpdater->stop();
    //Save the position of the current thread.
    m_savedPosition=position();
    //Reset the current playing thread, but saved all the other parameter.
    //Clear up the channel sync handle.
    removeChannelSyncs();
    //Check if the channel is not null.
    freeChannel();
}

void KNMusicBackendBassThread::restore(const QString &updatedFilePath)
{
    //Check out the saved position, if it's -1, means it never saved before.
    //Ignore the invalid call.
    if(m_savedPosition==-1)
    {
        return;
    }
    //Check out the updated file path.
    QString restoreFilePath=
            updatedFilePath.isEmpty()?m_filePath:updatedFilePath;
    //Reload the bass thread.
    loadBassThread(restoreFilePath);
    //Reset the position.
    setPosition(m_savedPosition);
    //Set the volume to the last volume, because of the reset, the
    //volume is back to 1.0.
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL, m_volume);
    //Check out the state.
    if(m_state==Playing)
    {
        //Start the updater.
        m_positionUpdater->start();
        //Play the thread.
        BASS_ChannelPlay(m_channel, FALSE);
    }
    //Reset the saved position.
    m_savedPosition=-1;
}

void KNMusicBackendBassThread::setVolume(const int &volume)
{
    //Check the channel is null.
    if(!m_channel)
    {
        return;
    }
    //The reason why no WASAPI operation here is because that WASAPI will never
    //comes here for setting the preview thread volume.
    //Set the volume to channel.
    BASS_ChannelSetAttribute(m_channel, BASS_ATTRIB_VOL,
                             ((float)volume)/100.0);
    //Save the latest volume size.
    m_volume=getChannelVolume();
}

void KNMusicBackendBassThread::setPosition(const qint64 &position)
{
    //Check the channel is null.
    if(!m_channel)
    {
        return;
    }
    //If the state is stopped and the position is not 0, then changed the state
    //to pause.
    if(m_state==Stopped && position!=0)
    {
        //Change to paused.
        setPlayingState(Paused);
    }
    //Change the position, the unit of the position should be translate into
    //second.
    BASS_ChannelSetPosition(
                m_channel,
                BASS_ChannelSeconds2Bytes(m_channel,
                                          //The position here should be the
                                          //'absolute' position.
                                          //That means it should be the
                                          //position plus the start
                                          //position.
                                          (double)(m_startPosition+position)
                                          /1000.0),
                BASS_POS_BYTE);
    //Check the position.
    checkPosition();
}

void KNMusicBackendBassThread::setCreateFlags(const DWORD &channelFlags)
{
    //Save the channel flags.
    m_channelFlags=channelFlags;
}

void KNMusicBackendBassThread::checkPosition()
{
    //Get the current position.
    qint64 currentPosition=getChannelPosition();
    //Emit position changed signal.
    emit positionChanged(currentPosition);
    //Check the position is longer than the duration.
    /*
     * Q: Why we still need to do this?
     * A: When cue is playing, it may not stopped at the end of the file.
     *    The callback is only used to solve the position won't reach the end
     *    bug, the track duration stopped will still process here.
     */
    if(currentPosition>=m_duration)
    {
        //Finished the playing.
        finishPlaying();
    }
}

void KNMusicBackendBassThread::threadReachesEnd(HSYNC handle,
                                                DWORD channel,
                                                DWORD data,
                                                void *user)
{
    Q_UNUSED(handle)
    Q_UNUSED(channel)
    Q_UNUSED(data)
    //Finished the playing of the thread.
    //Call the signal function to emit this signal.
    static_cast<KNMusicBackendBassThread *>(user)->reachesFinished();
}

#ifdef Q_OS_WIN64
DWORD KNMusicBackendBassThread::wasapiProcess(void *buffer, DWORD length,
                                              void *user)
{
    //Get the immediate sample data of the mixer sample stream.
    DWORD mixerChannelData=
            BASS_ChannelGetData(
                static_cast<KNMusicBackendBassThread *>(user)->getMixerHandle(),
                buffer, length);
    //Check the channel info data.
    if (mixerChannelData==-1)
    {
        //An error happened, set to no data.
        mixerChannelData=0;
    }
    //Give back the mixer data.
    return mixerChannelData;
}
#endif

inline void KNMusicBackendBassThread::finishPlaying()
{
    //Stop playing.
    stop();
    //Emit finished signal.
    emit finished();
}

inline void KNMusicBackendBassThread::setPlayingState(const int &state)
{
    //Only process the calling when the state is different.
    if(state!=m_state)
    {
        //Save the new state.
        m_state=state;
        //Emit state changed signal.
        emit stateChanged(m_state);
    }
}

inline void KNMusicBackendBassThread::removeChannelSyncs()
{
    //Get all the handlers.
    for(auto i=m_syncHandlers.begin(); i!=m_syncHandlers.end(); ++i)
    {
        //Remove all the sync from the list.
        BASS_ChannelRemoveSync(m_channel, *i);
    }
    //Clear the hanlder list.
    m_syncHandlers.clear();
}

qreal KNMusicBackendBassThread::getChannelVolume()
{
    //Check if the thread is loaded.
    if(!m_channel)
    {
        //For a unloaded volume it will be 0.
        return 0.0;
    }
    //Initial a cache.
    float channelVolume;
#ifdef Q_OS_WIN64
    if(m_wasapiEnabled)
    {
        channelVolume=BASS_WASAPI_GetVolume(BASS_WASAPI_VOL_SESSION |
                                            BASS_WASAPI_CURVE_LINEAR);
    }
    else
    {
#endif
        //Get the volume to the cache.
        BASS_ChannelGetAttribute(m_channel, BASS_ATTRIB_VOL, &channelVolume);
#ifdef Q_OS_WIN64
    }
#endif
    //Give back the channel volume.
    return channelVolume;
}

inline void KNMusicBackendBassThread::freeChannel()
{
#ifdef Q_OS_WIN64
    //When using WASAPI.
    if(m_wasapiEnabled)
    {
        //Check the mixer stream is not null.
        if(m_mixer)
        {
            //Free the Mix stream.
            BASS_StreamFree(m_mixer);
            //Reset the mixer channel.
            m_mixer=0;
        }
        //Free the WASAPI first.
        BASS_WASAPI_Free();
    }
#endif
    //Check if the channel is not null.
    if(m_channel)
    {
        //Free the streams or the music.
        if(!BASS_StreamFree(m_channel))
        {
            BASS_MusicFree(m_channel);
        }
        //Reset the channel.
        m_channel=0;
    }
}

inline bool KNMusicBackendBassThread::loadBassThread(const QString &filePath)
{
    //Clear the file path.
    m_filePath.clear();
    //Try to load the file.
#ifdef Q_OS_WIN
    std::wstring uniPath=filePath.toStdWString();
#endif
#ifdef Q_OS_UNIX
    std::string uniPath=filePath.toStdString();
#endif
    //Create the file using the stream.
    m_channel=BASS_StreamCreateFile(FALSE,
                                    uniPath.data(),
                                    0,
                                    0,
                                    m_channelFlags);
    //Check if the stream create successful.
    if(!m_channel)
    {
        //Set the MOD loading flag.
        DWORD modLoadFlag=BASS_MUSIC_RAMPS;
#ifdef Q_OS_WIN64
        if(m_wasapiEnabled)
        {
            //When using WASAPI, it will only use for decoding.
            modLoadFlag |= BASS_MUSIC_PRESCAN;
        }
#endif
        //Create the file using the fixed music load.
        m_channel=BASS_MusicLoad(FALSE,
                                 uniPath.data(),
                                 0,
                                 0,
                                 modLoadFlag | m_channelFlags,
                                 1);
        //Check if the music create successful.
        if(!m_channel)
        {
            //Emit load failed signal.
            emit loadFailed();
            //Bass is failed to load the music file.
            return false;
        }
    }
    //Save the new file path.
    m_filePath=filePath;
    //Set the sync handler.
    setChannelSyncs();
    //Load success.
    return true;
}

#ifdef Q_OS_WIN64
void KNMusicBackendBassThread::setWasapiData(bool enabled,
                                             int outputDevice,
                                             DWORD wasapiFlag)
{
    //Save the enable setting.
    m_wasapiEnabled=enabled;
    //Save the output device.
    m_wasapiOutputDevice=outputDevice;
    //Save the WASAPI initial flag.
    m_wasapiFlag=wasapiFlag;
    //Check the enable flag.
    if(m_wasapiEnabled)
    {
        //For 64-bit Windows, when use WASAPI as output, the stream will only be
        //used as decode stream.
        m_channelFlags |= BASS_STREAM_DECODE;
    }
}

inline bool KNMusicBackendBassThread::initialWasapi()
{
    // Setup output
    // Using a smaller buffer with event-driven system (only affects exclusive
    //mode)
    float bufferLength=(m_wasapiFlag&BASS_WASAPI_EVENT ? 0.1 : 0.4);
    BASS_WASAPI_INFO wasapiInfo;
    //Get the channel information.
    BASS_CHANNELINFO channelInfo;
    BASS_ChannelGetInfo(m_channel, &channelInfo);
    //Initialize the WASAPI device.
    if (!BASS_WASAPI_Init(m_wasapiOutputDevice,
                          channelInfo.freq, channelInfo.chans,
                          m_wasapiFlag, bufferLength,
                          0.05, wasapiProcess, this)) {
        // Failed, try falling back to shared mode
        if (!(m_wasapiFlag & BASS_WASAPI_EXCLUSIVE) ||
                !BASS_WASAPI_Init(m_wasapiOutputDevice,
                                  channelInfo.freq, channelInfo.chans,
                                  m_wasapiFlag & ~BASS_WASAPI_EXCLUSIVE,
                                  bufferLength, 0.05, wasapiProcess, this))
        {
            //Can't initialize device.
            return false;
        }
    }
    //Get the output details.
    BASS_WASAPI_GetInfo(&wasapiInfo);
    //Create a mixer with the same sample format (and enable GetPositionEx)
    m_mixer=BASS_Mixer_StreamCreate(wasapiInfo.freq, wasapiInfo.chans,
                                    BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE |
                                    BASS_MIXER_POSEX);
    // add the source to the mixer (downmix if necessary)
    BASS_Mixer_StreamAddChannel(m_mixer, m_channel, BASS_MIXER_DOWNMIX);
    //Complete the WASAPI initialize.
    return true;
}

HSTREAM KNMusicBackendBassThread::getMixerHandle() const
{
    return m_mixer;
}
#endif
