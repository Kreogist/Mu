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
#include "knnotificationdata.h"

KNNotificationData::KNNotificationData(QObject *parent) :
    m_icon(QPixmap()),
    m_fixedTitle(QString()),
    m_fixedContent(QString()),
    m_parent(parent),
    m_iconIndex(-1)
{
}

KNNotificationData::~KNNotificationData()
{
}

KNNotificationData::KNNotificationData(QString title, QString content,
                                       int iconIndex,
                                       QObject *parent):
    m_icon(QPixmap()),
    m_fixedTitle(title),
    m_fixedContent(content),
    m_parent(parent),
    m_iconIndex(iconIndex)
{
}

QObject *KNNotificationData::parent() const
{
    //Give back the parent pointer.
    return m_parent;
}

QString KNNotificationData::title() const
{
    //Simply return the fixed title text.
    return m_fixedTitle;
}

QString KNNotificationData::content() const
{
    //Simply return the fixed content text.
    return m_fixedContent;
}

QPixmap KNNotificationData::icon() const
{
    //Check the icon index.
    //When -1, return the customized icon, or else, the default icon.
    return m_iconIndex==-1?m_icon:QPixmap();
}

void KNNotificationData::setIcon(const QPixmap &icon)
{
    //Save the icon pixmap.
    m_icon=icon;
    //Update the icon index.
    m_iconIndex=-1;
}
