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
    //Load plugins from application resource dir and global plugin dir.
#ifdef Q_OS_MACX
    //This is a special OS, on Mac OS X, all the bass related dylib should be
    //placed at the same path of the executable file.
    initialPlugin(KNGlobal::applicationDirPath());
#else
    initialPlugin(KNGlobal::resourceDirPath()+"/Plugins/Bass");
    initialPlugin(KNGlobal::pluginDirPath()+"/Bass");
#endif
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

int KNMusicBackendBass::volume() const
{
    return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
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
    QString dylibSuffix=KNGlobal::instance()->dylibSuffix();
    //Get the specific plugin dir for Bass.
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

void KNMusicBackendBass::changeVolume(const int &volumeSize)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volumeSize);
}

qreal KNMusicBackendBass::smartVolumeScale() const
{
    return 0.125;
}

int KNMusicBackendBass::volumeLevel() const
{
    return 1000;
}
