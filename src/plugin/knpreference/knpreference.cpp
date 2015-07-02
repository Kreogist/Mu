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
#include <QBoxLayout>

#include "knthememanager.h"
#include "knvwidgetswitcher.h"
#include "knpreferencesidebar.h"

#include "knpreference.h"

KNPreference::KNPreference(QWidget *parent) :
    KNPreferencePlugin(parent),
    m_sidebar(new KNPreferenceSidebar(this)),
    m_content(new KNVWidgetSwitcher(this))
{
    setObjectName("Preference");
    //Set properties.
    setAutoFillBackground(true);

    //Configure content.
    m_content->setObjectName("PreferenceContent");
    m_content->setAutoFillBackground(true);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add the widgets to preference panel.
    mainLayout->addWidget(m_sidebar);
    //Add the contents to preference panel.
    mainLayout->addWidget(m_content, 1);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    knTheme->registerWidget(m_content);

    //Link requests.
    connect(m_sidebar, &KNPreferenceSidebar::requireClosePreference,
            this, &KNPreference::requireClosePreference);
}
