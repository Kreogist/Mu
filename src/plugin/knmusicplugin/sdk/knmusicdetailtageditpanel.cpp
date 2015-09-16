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
#include "kncircleiconbutton.h"

#include "knmusicdetailtageditpanel.h"

KNMusicDetailTagEditPanel::KNMusicDetailTagEditPanel(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_button(new KNCircleIconButton(this))
{
    //Configure the button.
    m_button->setIcon(QIcon(":/plugin/music/detaildialog/tab_icon/tag.png"));

}

QAbstractButton *KNMusicDetailTagEditPanel::tabButton()
{
    return m_button;
}

void KNMusicDetailTagEditPanel::setAnalysisItem(
        const MusicUtil::KNMusicAnalysisItem &item)
{
    ;
}

