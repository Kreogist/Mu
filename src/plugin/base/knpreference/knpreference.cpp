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
#include "knglobal.h"
#include "kncategoryplugin.h"
#include "knpreferencepanel.h"
#include "preference/knpreferenceitemglobal.h"

#include "knpreference.h"

KNPreference::KNPreference(QObject *parent) :
    KNPreferencePlugin(parent)
{
    //Initial the preference global.
    m_preferenceGlobal=KNPreferenceItemGlobal::instance();
    //Link the update infrastructure signal at very beginning.
    //Initial the preference panel.
    m_preferencePanel=new KNPreferencePanel;
    connect(m_preferencePanel, &KNPreferencePanel::requireUpdateInfrastructure,
            KNGlobal::instance(), &KNGlobal::updateInfrastructure);
    connect(m_preferencePanel, &KNPreferencePanel::requireHidePreference,
            this, &KNPreference::onActionHidePreference);
}

QWidget *KNPreference::preferencePanel()
{
    return m_preferencePanel;
}

int KNPreference::addCategory(KNCategoryPlugin *plugin)
{
    return m_preferencePanel->addCategory(plugin->caption(),
                                          plugin->preferenceIcon(),
                                          plugin->headerIcon(),
                                          plugin->preferencePanelWidget());
}

void KNPreference::setCategoryText(const int &index, const QString &title)
{
    m_preferencePanel->setCategoryText(index, title);
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
