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
#include "knanimelabelbutton.h"

#include "knmusicstoreglobal.h"

#include "knmusicstorepanel.h"

KNMusicStorePanel::KNMusicStorePanel(QWidget *parent) :
    KNMouseSenseWidget(parent),
    m_bulletWidget(new QLabel(this)),
    m_headerLabel(new KNAnimeLabelButton(this))
{
    //Configure the header label.
    m_headerLabel->hide();
    m_headerLabel->setMaximumWidth(knMusicStoreGlobal->storeHeaderLabelWidth());
    m_headerLabel->setChangeCursor(true);
    //Configure the bullet widget.
    m_bulletWidget->setPixmap(QPixmap("://public/bullet.png"));
    m_bulletWidget->hide();
}

KNAnimeLabelButton *KNMusicStorePanel::headerLabel()
{
    //Give the header label back.
    return m_headerLabel;
}

QLabel *KNMusicStorePanel::bulletLabel()
{
    //Give the bullet widget back.
    return m_bulletWidget;
}
