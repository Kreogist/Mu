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
#include <QBoxLayout>

#include "knpreferencecategory.h"
#include "knpreferencecontents.h"

#include "knpreferencepanel.h"

KNPreferencePanel::KNPreferencePanel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial widgets.
    m_categoryList=new KNPreferenceCategory(this);
    connect(m_categoryList, &KNPreferenceCategory::requireHidePreference,
            this, &KNPreferencePanel::requireHidePreference);
    m_layout->addWidget(m_categoryList, 1);
    m_contents=new KNPreferenceContents(this);
    m_layout->addWidget(m_contents, 1);
}
