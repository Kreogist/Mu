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

#include "knpreferenceitem.h"
#include "knvwidgetswitcher.h"
#include "knpreferencesidebar.h"
#include "knpreferenceabout.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knpreference.h"

KNPreference::KNPreference(QWidget *parent) :
    KNPreferencePlugin(parent),
    m_sidebar(new KNPreferenceSidebar(this)),
    m_content(new KNVWidgetSwitcher(this)),
    m_aboutItem(new KNPreferenceItem(this)),
    m_about(new KNPreferenceAbout(this))
{
    setObjectName("Preference");
    //Set properties.
    setAutoFillBackground(true);

    //Configure preference content.
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

    //Add about item and content to preference.
    addPreferenceTab(m_aboutItem, m_about);

    //Link requests.
    connect(m_sidebar, &KNPreferenceSidebar::requireClosePreference,
            this, &KNPreference::requireClosePreference);
    connect(m_sidebar, &KNPreferenceSidebar::requireChangeContent,
            m_content, &KNVWidgetSwitcher::setCurrentIndex);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    knTheme->registerWidget(m_content);

    //Link retranslate.
    knI18n->link(this, &KNPreference::retranslate);
    retranslate();
}

void KNPreference::addTab(KNPreferenceItem *tabWidget, QWidget *content)
{
    //Add the tab to sidebar, add the content to content.
    addPreferenceTab(tabWidget, content);
}

void KNPreference::retranslate()
{
    //Update the item data.
    m_aboutItem->setText(tr("About"));

    ;
}

inline void KNPreference::addPreferenceTab(KNPreferenceItem *tabWidget,
                                           QWidget *content)
{
    //Add content widget to content container.
    m_content->addWidget(content);
    //Add tab widget to sidebar.
    m_sidebar->addItemWidget(tabWidget);
}
