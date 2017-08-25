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
#include "knconfigure.h"

#include "knglobal.h"

#include "knmusicbackendmpvthread.h"

#include "knmusicbackendmpv.h"

KNMusicBackendMpv::KNMusicBackendMpv(QObject *parent) :
    KNMusicStandardBackend(parent),
    m_systemConfigure(knGlobal->systemConfigure()->getConfigure("Backend")),
    m_userConfigure(knGlobal->userConfigure()->getConfigure("Backend")),
    m_main(nullptr),
    m_preview(nullptr)
{
#ifdef Q_OS_LINUX
    //When using Mac OS X to do the test, it doesn't need to change the locale.
    //Under Linux, it pops up the following message.
    /*
     * Non-C locale detected. This is not supported.
     * Call 'setlocale(LC_NUMERIC, "C");' in your code.
     */
    setlocale(LC_NUMERIC, "C");
#endif
    //Initial the device name.
    QString deviceName;
    {
        //Get the initial flags from the configure.
        QJsonObject outputDevice=
                m_systemConfigure->data("OutputDevice").toJsonObject();
        if(outputDevice.contains("DeviceName"))
        {
            //Save the device name.
            deviceName=outputDevice.value("DeviceName").toString();
        }
    }
    //Get the buffer length.
    int bufferLength=m_systemConfigure->data("BufferLength", 500).toInt();
    //Check buffer length is valid.
    if(bufferLength<10)
    {
        //For minimum, buffer should be 10ms.
        bufferLength=10;
    }
    else if(bufferLength>5000)
    {
        //For maximum, buffer should be 5000ms.
        bufferLength=5000;
    }
    //Check the no buffer setting.
    if(!m_systemConfigure->data("Buffer", true).toBool())
    {
        //No buffer seting is enabled.
        bufferLength=0;
    }
    //Check the force to stero output setting.
    bool forceStereo=m_systemConfigure->data("Stereo", false).toBool();
    //Get the output device sample rate settings.
    int initialSampleRate=-1;
    QString userSampleRate=
            m_systemConfigure->data("SampleRate", "None").toString();
    //Check the user sample rate.
    if(QString::number(userSampleRate.toInt())==userSampleRate)
    {
        //Update the initial sample rate.
        initialSampleRate=userSampleRate.toInt();
    }
    //Initial the main and preview of the threads.
    m_main=new KNMusicBackendMpvThread;
    m_preview=new KNMusicBackendMpvThread;
    //Set the flag to both thread.
    m_main->setCreateFlags(deviceName, bufferLength, initialSampleRate,
                           forceStereo);
    m_preview->setCreateFlags(deviceName, bufferLength, initialSampleRate,
                              forceStereo);
    //Set the main and preview threads.
    setMainThread(m_main);
    setPreviewThread(m_preview);
    //Link the main thread success signal to backend signal.
    connect(m_main, &KNMusicBackendMpvThread::loadSuccess,
            this, &KNMusicBackendMpv::loadSuccess);
}

KNMusicBackendMpv::~KNMusicBackendMpv()
{
    m_main->deleteLater();
    m_preview->deleteLater();
}


bool KNMusicBackendMpv::loadMusic(const QString &filePath,
                                  const qint64 &start,
                                  const qint64 &duration)
{
    //Check thread first, if thread exist, load the music.
    if(!m_main || !m_main->loadFile(filePath))
    {
        //If there's no thread or failed to load, load music will always be
        //false.
        return false;
    }
    //Set the section for thread.
    m_main->setPlaySection(start, duration);
    //Load the music compelte.
    return true;
}

bool KNMusicBackendMpv::previewLoadMusic(const QString &filePath,
                                         const qint64 &start,
                                         const qint64 &duration)
{
    //Check thread first, if thread exist, load the music.
    if(!m_preview || !m_preview->loadFile(filePath))
    {
        //If there's no thread or failed to load, load music will always be
        //false.
        return false;
    }
    //Set the section for thread.
    m_preview->setPlaySection(start, duration);
    //Load the music compelte.
    return true;
}

int KNMusicBackendMpv::volume() const
{
    return m_main->volume();
}

int KNMusicBackendMpv::minimalVolume() const
{
    return 0;
}

int KNMusicBackendMpv::maximumVolume() const
{
    return 10000;
}

QJsonArray KNMusicBackendMpv::deviceList()
{
    //Create the tempoary handle for device detecting.
    mpv_handle *deviceHandler=mpv_create();
    //Check the pointer.
    if(!deviceHandler)
    {
        //No valid device is detect if you have no more memory.
        return QJsonArray();
    }
    //Initialized the mpv handle.
    mpv_initialize(deviceHandler);
    //Get the device list.
    mpv_node deviceListNode;
    if(mpv_get_property(deviceHandler,
                        "audio-device-list",
                        MPV_FORMAT_NODE,
                        &deviceListNode)!=0 ||
            deviceListNode.format!=MPV_FORMAT_NODE_ARRAY)
    {
        //Failed to get the device list.
        return QJsonArray();
    }
    //Extract the node array from the node.
    mpv_node_list *deviceNodeList=deviceListNode.u.list;
    //Prepare the json array list.
    QJsonArray deviceList;
    //Construct the device information.
    for(int i=0; i<deviceNodeList->num; ++i)
    {
        //Get the format of the format.
        if(MPV_FORMAT_NODE_MAP!=deviceNodeList->values[i].format)
        {
            //Move to the next map.
            continue;
        }
        //Check the result.
        mpv_node_list *deviceInfoMap=deviceNodeList->values[i].u.list;
        //Prepare the device data list.
        QJsonObject deviceData;
        //Set the device Id.
        deviceData.insert("Type", "CustomObject");
        deviceData.insert("Id", i);
        //Get the data from the list.
        for(int j=0; j<deviceInfoMap->num; ++j)
        {
            //Check the key data.
            if(strncmp("name", deviceInfoMap->keys[j], 4)==0)
            {
                //Simply save the device name as the customer data.
                deviceData.insert("DeviceName",
                                  deviceInfoMap->values[j].u.string);
            }
            else if(strncmp("description", deviceInfoMap->keys[j], 11)==0)
            {
                //Save the description data as the display name.
                QString deviceName=deviceInfoMap->values[j].u.string;
                //Check the device name.
                if(deviceName=="Autoselect device")
                {
                    //Clear the auto select device.
                    deviceName=QString();
                }
                //Insert the data.
                deviceData.insert("Name", deviceName);
            }
        }
        //Append the device data.
        deviceList.append(deviceData);
    }
    //Free the node contents.
    mpv_free_node_contents(&deviceListNode);
    //Free the mpv handler.
    mpv_terminate_destroy(deviceHandler);
    //Give back the device list data.
    return deviceList;
}

void KNMusicBackendMpv::setGlobalVolume(const int &volume)
{
    //Set to the main thread.
    m_main->setVolume(volume);
    //Emit the volume changed signal.
    emit volumeChanged(volume);
}

int KNMusicBackendMpv::volumeLevel() const
{
    //Get the volume level setting.
    int percentageLevel=m_userConfigure->data("VolumeLevel", 0).toInt();
    if(percentageLevel>0 && percentageLevel<21)
    {
        //Give back the value.
        return (int)(100.0*(qreal)percentageLevel);
    }
    //Or else, simple return the default value.
    return 1000;
}

qreal KNMusicBackendMpv::smartVolumeScale() const
{
    return 0.1;
}
