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
#include <gst/gst.h>

#include "knmusicbackendgstreamerthread.h"

#include "knmusicbackendgstreamer.h"

KNMusicBackendGStreamer::KNMusicBackendGStreamer(QObject *parent) :
    KNMusicStandardBackend(parent),
    m_main(nullptr),
    m_preview(nullptr)
{
    //Initial the gstreamer first.
    gst_init(NULL, NULL);
    //Initial the main and preview of the threads.
    m_main=new KNMusicBackendGStreamerThread;
    m_preview=new KNMusicBackendGStreamerThread;
    //Set the main and preview threads.
    setMainThread(m_main);
    setPreviewThread(m_preview);
}

KNMusicBackendGStreamer::~KNMusicBackendGStreamer()
{
    m_main->deleteLater();
    m_preview->deleteLater();
}

int KNMusicBackendGStreamer::volume() const
{
    return m_main->volume();
}

int KNMusicBackendGStreamer::minimalVolume() const
{
    return 0;
}

int KNMusicBackendGStreamer::maximumVolume() const
{
    return 10000;
}

void KNMusicBackendGStreamer::setGlobalVolume(const int &volume)
{
    //Set to the main thread.
    m_main->setVolume(volume);
    //Emit the volume changed signal.
    emit volumeChanged(volume);
}

int KNMusicBackendGStreamer::volumeLevel() const
{
    return 1000;
}

qreal KNMusicBackendGStreamer::smartVolumeScale() const
{
    return 0.25;
}
