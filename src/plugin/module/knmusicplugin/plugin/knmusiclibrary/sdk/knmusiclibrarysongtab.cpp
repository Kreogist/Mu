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
#include <QWidget>

#include "knmusiclibrarysongtab.h"

KNMusicLibrarySongTab::KNMusicLibrarySongTab(QObject *parent) :
    KNMusicTab(parent)
{
    m_widget=new QWidget;
}

QString KNMusicLibrarySongTab::caption()
{
    return tr("Song");
}

QPixmap KNMusicLibrarySongTab::icon()
{
    return QPixmap(":/plugin/music/category/01_musics.png");
}

QWidget *KNMusicLibrarySongTab::widget()
{
    return m_widget;
}

void KNMusicLibrarySongTab::onActionSearch(const QString &text)
{
    ;
}
