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
#include "knpreferencelanguageitem.h"
#include "knvwidgetswitcher.h"
#include "knpreferencesidebar.h"
#include "knpreferenceabout.h"
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knpreferencelanguagepanel.h"
#include "knpreferencesettingpanel.h"
#include "knconfigure.h"
#include "knglobal.h"

#include "knpreference.h"

KNPreference::KNPreference(QWidget *parent) :
    KNPreferencePlugin(parent),
    m_preferenceConfigure(knGlobal->userConfigure()->getConfigure(
                              "Preference")),
    m_sidebar(new KNPreferenceSidebar(this)),
    m_content(new KNVWidgetSwitcher(this)),
    m_aboutItem(generateItem(":/plugin/preference/about.png",
                             ":/plugin/preference/header/about.png")),
    m_about(new KNPreferenceAbout(this)),
    m_languagePanel(new KNPreferenceLanguagePanel(this)),
    m_settingPanel(new KNPreferenceSettingPanel(this))
{
    setObjectName("Preference");
    //Set properties.
    setAutoFillBackground(true);
    //Configure preference content.
    m_content->setObjectName("PreferenceContent");
    m_content->setAutoFillBackground(true);
    //Configure the language panel.
    m_languagePanel->generateLanguageList();
    connect(m_languagePanel, &KNPreferenceLanguagePanel::requireUpdateTitle,
            m_sidebar, &KNPreferenceSidebar::setHeaderText);

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

    //Link requests.
    connect(m_sidebar, &KNPreferenceSidebar::requireClosePreference,
            this, &KNPreference::requireClosePreference);
    connect(m_sidebar, &KNPreferenceSidebar::requireChangeContent,
            this, &KNPreference::onActionIndexChange);
    connect(m_sidebar, &KNPreferenceSidebar::advancedToggle,
            this, &KNPreference::onActionAdvancedToggle);
    //Add fixed tabs.
    // Add about item and content to preference.
    m_content->addWidget(m_about);
    m_sidebar->addFixedItemWidget(m_aboutItem);
    // Add language item and content to preference.
    m_content->addWidget(m_languagePanel);
    m_sidebar->addFixedItemWidget(m_languagePanel->languageListItem());
    //Add setting panel to content.
    m_content->addWidget(m_settingPanel);
    //Generate all the items.
    generateSettingItems();

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    knTheme->registerWidget(m_content);

    //Link retranslate.
    knI18n->link(this, &KNPreference::retranslate);
    retranslate();

    //Load the configure.
    m_sidebar->setAdvancedShown(m_preferenceConfigure->data("Advanced",
                                                            false).toBool());
}

void KNPreference::retranslate()
{
    //Update the item data.
    m_aboutItem->setText(tr("About"));
    //Update the item title.
    updateItemTitle();
    //Update the sidebar title.
    m_sidebar->updateTitleBarText();
}

void KNPreference::onActionIndexChange(int index)
{
    //Change the content index.
    m_content->setCurrentIndex(index);
}

void KNPreference::onActionAdvancedToggle(bool toggle)
{
    //Save the toggle state.
    m_preferenceConfigure->setData("Advanced", toggle);
    //Process advanced state.
    //!FIXME: Process the advanced toggle event.
}

inline KNPreferenceItem *KNPreference::generateItem(
        const QString &iconPath,
        const QString &headerIconPath)
{
    //Generate a new item.
    KNPreferenceItem *item=new KNPreferenceItem(this);
    //Set the icon path and header icon path.
    item->setIcon(QPixmap(iconPath));
    item->setHeaderIcon(QPixmap(headerIconPath));
    //Return the item.
    return item;
}

inline KNPreferenceItem *KNPreference::generateItem(const QString &iconPath,
                                                    const QPixmap &headerIcon)
{
    //Generate a new item.
    KNPreferenceItem *item=new KNPreferenceItem(this);
    //Set the icon path and header icon path.
    item->setIcon(QPixmap(iconPath));
    item->setHeaderIcon(headerIcon);
    //Return the item.
    return item;
}

inline void KNPreference::updateItemTitle()
{
    //Update all the item title.
    m_sidebar->setItemTitle(PanelGeneral,
                            tr("General"));
}

inline void KNPreference::generateSettingItems()
{
    //Get the setting icon for all the button.
    QPixmap headerSettingIcon("://preference/header/setting.png");
    //Generate all the panel items.
    KNPreferenceItem *items[PreferencePanelCount];
    items[PanelGeneral]=generateItem("://preference/general.png",
                                     headerSettingIcon);
    //Generate all the items.
    for(int i=0; i<PreferencePanelCount; ++i)
    {
        //Add item to the sidebar.
        m_sidebar->addItemWidget(items[i]);
    }
    //Update the item title.
    updateItemTitle();
}
