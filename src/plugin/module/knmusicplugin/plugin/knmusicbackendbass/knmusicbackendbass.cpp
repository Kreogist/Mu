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
#include <QDir>
#include <QEasingCurve>

#include "knglobal.h"
#include "knmusicbassglobal.h"
#include "knmusicbackendbassthread.h"

#include "knmusicbackendbass.h"

#include <QDebug>

KNMusicBackendBass::KNMusicBackendBass(QObject *parent) :
    KNMusicStandardBackend(parent)
{
    //Initial all the bass library.
    m_available=initialBass();
    //If initial failed, exit.
    if(!m_available)
    {
        return;
    }
    //Load plugins.
    initialPlugin(KNGlobal::applicationDirPath()+"/Plugins");
    //Initial the main and preview thread.
    m_main=new KNMusicBackendBassThread(this);
    setMainThread(m_main);

    m_preview=new KNMusicBackendBassThread(this);
    setPreviewThread(m_preview);
}

KNMusicBackendBass::~KNMusicBackendBass()
{
    //Free the memory.
    m_main->clear();
    m_preview->clear();
    //Close the bass.
    BASS_Free();
}

bool KNMusicBackendBass::available()
{
    return m_available;
}

void KNMusicBackendBass::loadMusic(const QString &filePath)
{
    m_main->loadFromFile(filePath);
}

qint64 KNMusicBackendBass::duration() const
{
    return m_main->duration();
}

qint64 KNMusicBackendBass::position() const
{
    return m_main->position();
}

void KNMusicBackendBass::playFile(const QString &fileName)
{
    //Play the whole file as a section.
    playSection(fileName);
}

void KNMusicBackendBass::playSection(const QString &fileName,
                                     const qint64 &start,
                                     const qint64 &duration)
{
    //Load the music file first.
    loadMusic(fileName);
    //Play the section
    m_main->playSection(start, duration);
}

void KNMusicBackendBass::play()
{
    m_main->play();
}

void KNMusicBackendBass::pause()
{
    m_main->pause();
}

void KNMusicBackendBass::stop()
{
    m_main->stop();
}

void KNMusicBackendBass::resetMainPlayer()
{
    m_main->clear();
}

int KNMusicBackendBass::volume() const
{
    return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
}

void KNMusicBackendBass::loadPreview(const QString &filePath)
{
    m_preview->loadFromFile(filePath);
}

qint64 KNMusicBackendBass::previewDuration() const
{
    return m_preview->duration();
}

void KNMusicBackendBass::playPreviewFile(const QString &fileName)
{
    //Preview the whole file as a section.
    playPreviewSection(fileName);
}

void KNMusicBackendBass::playPreviewSection(const QString &fileName,
                                            const qint64 &start,
                                            const qint64 &duration)
{
    //Load the music file first.
    loadPreview(fileName);
    //Turn on the smart volume.
    smartVolumeOn();
    //Play the section
    m_preview->playSection(start, duration);
}

void KNMusicBackendBass::playPreview()
{
    smartVolumeOn();
    //Play the preview.
    m_preview->play();
}

void KNMusicBackendBass::stopPreview()
{
    smartVolumeOff();
    //Stop the preview
    m_preview->stop();
}

void KNMusicBackendBass::pausePreview()
{
    smartVolumeOff();
    //Stop the preview
    m_preview->pause();
}

void KNMusicBackendBass::loadUrl(const QString &url)
{
    ;
}

int KNMusicBackendBass::volumeMinimal()
{
    return 0;
}

int KNMusicBackendBass::volumeMaximum()
{
    return 10000;
}

void KNMusicBackendBass::changeMuteState()
{
    setMute(!m_mute);
}

void KNMusicBackendBass::setMute(const bool &mute)
{
    //Check the state is the same or not, if is the same, do nothing.
    if(m_mute==mute)
    {
        return;
    }
    //Set state.
    m_mute=mute;
    if(m_mute)
    {
        //Backup the original volume, mute it.
        m_volumeBeforeMute=volume();
        changeVolume(0);
    }
    else
    {
        //Set the volume to the backup volume.
        changeVolume(m_volumeBeforeMute);
    }
    //Emit changed signal.
    emit muteStateChanged(m_mute);
}

void KNMusicBackendBass::setVolume(const int &volumeSize)
{
    if(m_mute)
    {
        m_mute=false;
        emit muteStateChanged(m_mute);
    }
    changeVolume(volumeSize);
}

void KNMusicBackendBass::setPosition(const qint64 &position)
{
    m_main->setPosition(position);
}

void KNMusicBackendBass::setPreviewPosition(const qint64 &position)
{
    m_preview->setPosition(position);
}

bool KNMusicBackendBass::initialBass()
{
    //Check the bass version.
    if(HIWORD(BASS_GetVersion())!=BASSVERSION)
    {
        //! FIXME: Warning that version error.
        return false;
    }
    //Enabled float digital signal processing.
    //DON'T MOVE THIS, this should config before bass init.
    BASS_SetConfig(BASS_CONFIG_FLOATDSP, TRUE);
    //Initial Bass:
    if(!BASS_Init(-1,44100,0,NULL,NULL))
    {
        //! FIXME: Warning cannot load bass.
        return false;
    }
    //Check float dsp:
    DWORD fdpsCheck=BASS_StreamCreate(44100,2,BASS_SAMPLE_FLOAT,NULL,0);
    if(fdpsCheck)
    {
        //Free the check channel.
        BASS_StreamFree(fdpsCheck);
        //Set the float flag.
        KNMusicBassGlobal::setFdps(BASS_SAMPLE_FLOAT);
    }
    //Enable playlist processing.
    BASS_SetConfig(BASS_CONFIG_NET_PLAYLIST,1);
    //Minimize automatic pre-buffering, so we can do it (and display it) instead.
    BASS_SetConfig(BASS_CONFIG_NET_PREBUF,0);
    //Disable web proxy.
    BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY,NULL);
    return true;
}

void KNMusicBackendBass::initialPlugin(const QString &pluginDirPath)
{
    QString dylibSuffix=KNGlobal::dylibSuffix();
    QDir pluginDir(pluginDirPath);
    //Check the dir exist.
    if(pluginDir.exists())
    {
        //Get all the file from the plugin dir.
        QFileInfoList pluginList=pluginDir.entryInfoList();
        //Check all the file.
        while(!pluginList.isEmpty())
        {
            QFileInfo currentInfo=pluginList.takeFirst();
            if(currentInfo.isFile() && currentInfo.suffix().toLower()==dylibSuffix)
            {
                //If we can load the plugin, output the data.
                HPLUGIN plugin=
#ifdef Q_OS_WIN32
                        BASS_PluginLoad(currentInfo.absoluteFilePath().toStdWString().data(), 0);
#endif
#ifdef Q_OS_UNIX
                        BASS_PluginLoad(currentInfo.absoluteFilePath().toStdString().data(), 0);
#endif
                //If plugin is load complete, read the plugin info.
                if(plugin)
                {
                    const BASS_PLUGININFO *pluginInfo=BASS_PluginGetInfo(plugin);
                    for(DWORD i=0; i<pluginInfo->formatc; i++)
                    {
//                        qDebug()<<pluginInfo->formats[i].exts;
                    }
                }
            }
        }
    }
}

void KNMusicBackendBass::smartVolumeOn()
{
    //If the origial volume is not -1, means smart volume has been turned on.
    if(m_originalVolume!=-1)
    {
        return;
    }
    //Backup the original volume.
    m_originalVolume=volume();
    //Set the preview as the full volume.
    m_preview->setVolume(m_originalVolume);
    //Set the main volume as the 1/8 of the full volume.
    m_main->setVolume(m_originalVolume>8?m_originalVolume/8:0);
}

void KNMusicBackendBass::smartVolumeOff()
{
    //If the original volume is -1, means it's not turned on.
    if(m_originalVolume==-1)
    {
        return;
    }
    //Set the main volume back to full volume.
    m_main->setVolume(m_originalVolume);
    //Reset the original volume.
    m_originalVolume=-1;
}

void KNMusicBackendBass::changeVolume(const int &volumeSize)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volumeSize);
}
