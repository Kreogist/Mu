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

#include "knconfigure.h"
#include "kncategoryplugin.h"
#include "kncategorypreference.h"
#include "knglobal.h"

#include "knpreferencegeneral.h"
#include "knpreferenceitempanel.h"
#include "knpreferencecategory.h"
#include "knpreferencecontents.h"
#include "knpreferencelanguageitem.h"
#include "knpreferencelanguagepanel.h"

#include "knlocalemanager.h"

#include "knpreferencepanel.h"

#include <QDebug>

KNPreferencePanel::KNPreferencePanel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Initial locale manager.
    m_localeManager=KNLocaleManager::instance();

    //Initial layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight,
                            this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

    //Initial widgets.
    m_categoryList=new KNPreferenceCategory(this);
    connect(m_categoryList, &KNPreferenceCategory::currentIndexChanged,
            this, &KNPreferencePanel::onActionCategoryIndexChange);
    connect(m_categoryList, &KNPreferenceCategory::requireHidePreference,
            this, &KNPreferencePanel::requireHidePreference);
    m_layout->addWidget(m_categoryList, 1);
    m_contents=new KNPreferenceContents(this);
    m_layout->addWidget(m_contents, 1);

    //Initial the language panel.
    initialLanguagePanel();

    //Initial the general configure and panel.
    m_generalPreference=new KNPreferenceGeneral(this);
    //Link the request.
    connect(m_generalPreference, &KNPreferenceGeneral::libraryDirMoved,
            this, &KNPreferencePanel::requireUpdateInfrastructure);

    //Add general category.
    m_categoryList->addCategory("",
                                QPixmap(":/plugin/configure/general/icon.png"),
                                QPixmap(":/plugin/configure/general/headicon.png"));
    addCategoryPreference(m_generalPreference);

    //Connect retranslate signal.
    connect(m_localeManager, &KNLocaleManager::requireRetranslate,
            this, &KNPreferencePanel::retranslate);
    //Retranslate.
    retranslate();
}

void KNPreferencePanel::addLanguageButton(KNAnimeCheckedButton *languageButton,
                                          const QPixmap &headerIcon,
                                          QWidget *panel)
{
    //Simply add the button to the category list.
    m_categoryList->addLanguageButton(languageButton, headerIcon);
    //Add panel to content.
    m_contents->addPanelWidget(panel);
}

int KNPreferencePanel::addCategory(KNCategoryPlugin *plugin)
{
    //Add the preference panel, link request.
    addCategoryPreference(plugin->preference());
    //Add the info of this category to the category list and content.
    return m_categoryList->addCategory(plugin->caption(),
                                       plugin->icon(),
                                       plugin->headerIcon());
}

void KNPreferencePanel::setCategoryText(const int &index,
                                        const QString &title)
{
    m_categoryList->setCategoryText(index, title);
}

void KNPreferencePanel::setCurrentIndex(const int &index)
{
    //Because when category list change the index, the content will
    //automatically changed, so we don't need to change the contents index.
    m_categoryList->setCurrentIndex(index);
}

void KNPreferencePanel::retranslate()
{
    //Update the general text.
    setCategoryText(1, tr("General"));
    //Update the language item.
    m_languageItem->setIcon(m_localeManager->currentLanguageIcon());
    m_languageItem->setText(m_localeManager->currentLanguageCaption());
}

void KNPreferencePanel::onActionCategoryIndexChange(const int &index)
{
    m_contents->setCurrentIndex(index);
}

inline void KNPreferencePanel::initialLanguagePanel()
{
    //Initial the language item and panel.
    m_languageItem=new KNPreferenceLanguageItem(this);
    m_languagePanel=new KNPreferenceLanguagePanel(this);
    //Add language button.
    addLanguageButton(m_languageItem,
                      QPixmap(":/plugin/configure/locale/headicon.png"),
                      m_languagePanel);
}

void KNPreferencePanel::addCategoryPreference(KNCategoryPreference *preference)
{
    //Add the panel to contents.
    KNPreferenceItemPanel *panel=preference->panel();
    m_contents->addPanelWidget(panel);
    //Link the panel.
    connect(m_categoryList, &KNPreferenceCategory::requireShowNormal,
            panel, &KNPreferenceItemPanel::setNormalMode);
    connect(this, &KNPreferencePanel::requireSavePreference,
            panel, &KNPreferenceItemPanel::saveItemsValue);
}
