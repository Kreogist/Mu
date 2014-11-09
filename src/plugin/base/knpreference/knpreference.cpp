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
#include "kncategoryplugin.h"
#include "knpreferencepanel.h"
#include "knlocalemanager.h"
#include "preference/knpreferenceitemglobal.h"

#include "knpreference.h"

KNPreference::KNPreference(QObject *parent) :
    KNPreferencePlugin(parent)
{
    //Initial the preference global.
    m_preferenceGlobal=KNPreferenceItemGlobal::instance();
    //Initial the preference panel.
    m_preferencePanel=new KNPreferencePanel;
    connect(m_preferencePanel, &KNPreferencePanel::requireHidePreference,
            this, &KNPreference::onActionHidePreference);

    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNPreference::retranslate);
}

QWidget *KNPreference::preferencePanel()
{
    return m_preferencePanel;
}

void KNPreference::addCategory(KNCategoryPlugin *plugin)
{
    CategoryItem currentCategory;
    //Get the index of current category.
    currentCategory.index=
            m_preferencePanel->addCategory(plugin->caption(),
                                           plugin->preferenceIcon(),
                                           plugin->headerIcon(),
                                           plugin->preferencePanelWidget());
    currentCategory.plugin=plugin;
    //Add to list.
    m_categoryList.append(currentCategory);
}

void KNPreference::setCurrentIndex(const int &index)
{
    m_preferencePanel->setCurrentIndex(index);
}

void KNPreference::onActionHidePreference()
{
    //Save preference first.
    m_preferencePanel->requireSavePreference();
    //Ask to apply the preference.
    m_preferenceGlobal->requireApplyPreference();
    //Ask to hide preference.
    emit requireHidePreference();
}

void KNPreference::retranslate()
{
    //Update the preference caption.
    for(auto i=m_categoryList.begin();
        i!=m_categoryList.end();
        ++i)
    {
        m_preferencePanel->setCategoryText((*i).index, (*i).plugin->caption());
    }
}
