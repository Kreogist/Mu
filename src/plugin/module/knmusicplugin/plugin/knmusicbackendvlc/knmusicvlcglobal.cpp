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
#include "knmusicvlcglobal.h"

KNMusicVLCGlobal *KNMusicVLCGlobal::m_instance=nullptr;

KNMusicVLCGlobal *KNMusicVLCGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicVLCGlobal:m_instance;
}

KNMusicVLCGlobal::~KNMusicVLCGlobal()
{
    //Release the vlc instance.
    libvlc_release(m_vlcInstance);
}

KNMusicVLCGlobal::KNMusicVLCGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the VLC.
    m_vlcInstance=libvlc_new(0, nullptr);
}

libvlc_instance_t *KNMusicVLCGlobal::vlcInstance() const
{
    return m_vlcInstance;
}
