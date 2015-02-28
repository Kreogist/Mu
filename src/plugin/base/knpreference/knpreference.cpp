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
#include "knpreferenceitemfactory.h"
#include "knpreferencepanel.h"

#include "knpreference.h"

KNPreference::KNPreference(QObject *parent) :
    KNPreferencePlugin(parent)
{
    //Initial the preference item factory.
    KNPreferenceItemFactory::initialResource();
    //Initial the preference panel.
    m_preferencePanel=new KNPreferencePanel;
    //Link the update infrastructure signal at very beginning.
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
    return m_preferencePanel->addCategory(plugin);
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
    emit requireApplyPreference();
    //Ask to hide preference.
    emit requireHidePreference();
}
