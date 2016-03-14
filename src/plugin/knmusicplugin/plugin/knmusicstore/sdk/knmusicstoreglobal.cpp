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

#include "knmusicstoreglobal.h"

KNMusicStoreGlobal *KNMusicStoreGlobal::m_instance=nullptr;

KNMusicStoreGlobal *KNMusicStoreGlobal::instance()
{
    return m_instance;
}

KNMusicStoreGlobal *KNMusicStoreGlobal::initial(QObject *parent)
{
    //Check instance pointer.
    if(m_instance==nullptr)
    {
        //Initial the instance.
        m_instance=new KNMusicStoreGlobal(parent);
    }
    //Give back the instance.
    return m_instance;
}

int KNMusicStoreGlobal::storeContentWidth() const
{
    return 980;
}

KNMusicStoreGlobal::KNMusicStoreGlobal(QObject *parent) :
    QObject(parent)
{

}
