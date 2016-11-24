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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "knlocalemanager.h"

#include "knmusicstoreglobal.h"

KNMusicStoreGlobal *KNMusicStoreGlobal::m_instance=nullptr;

KNMusicStoreGlobal *KNMusicStoreGlobal::instance()
{
    return m_instance;
}

void KNMusicStoreGlobal::initial(QObject *parent)
{
    //Check the instance.
    if(m_instance==nullptr)
    {
        //Construct the object.
        m_instance=new KNMusicStoreGlobal(parent);
    }
}

void KNMusicStoreGlobal::retranslate()
{
    ;
}

KNMusicStoreGlobal::KNMusicStoreGlobal(QObject *parent) :
    QObject(parent)
{
    //Link the retranslator.
    knI18n->link(this, &KNMusicStoreGlobal::retranslate);
    retranslate();
}
