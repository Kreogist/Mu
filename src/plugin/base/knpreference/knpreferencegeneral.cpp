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
#include <QLabel>

#include "knglobal.h"
#include "knconfigure.h"
#include "preference/knpreferenceitempathbrowser.h"
#include "knpreferenceitempanel.h"
#include "knpreferenceitemfactory.h"

#include "knpreferencegeneral.h"

KNPreferenceGeneral::KNPreferenceGeneral(QObject *parent) :
    KNCategoryPreference(parent)
{
    //Get system configure.
    m_systemConfigure=KNGlobal::instance()->systemConfigure();
    //Initial general configure.
    m_generalConfigure=new KNConfigure(this);
    m_generalConfigure->setCaption("General");
    KNGlobal::instance()->userConfigure()->addSubConfigure(m_generalConfigure);
    //Initial the preference panel.
    m_panel=new KNPreferenceItemPanel;

    //Generate title.
    m_titles[GeneralLibrary]=KNPreferenceItemFactory::createTitle();
    //Generate items.
    m_items[LibraryPath]=KNPreferenceItemFactory::create(PathEditItem, "LibraryPath", m_systemConfigure);

    //Link the library detector.
    connect((KNPreferenceItemPathBrowser *)m_items[LibraryPath], &KNPreferenceItemPathBrowser::dirMoved,
            this, &KNPreferenceGeneral::onLibraryMoved);

    //Initial panel.
    m_panel->addTitle(m_titles[GeneralLibrary]);
    m_panel->addItem(m_items[LibraryPath]);

    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNPreferenceGeneral::retranslate);
    retranslate();
}

KNPreferenceItemPanel *KNPreferenceGeneral::panel()
{
    return m_panel;
}

KNConfigure *KNPreferenceGeneral::configure()
{
    return m_generalConfigure;
}

void KNPreferenceGeneral::loadConfigures()
{
    for(int i=0; i<GeneralPreferenceItemCount; i++)
    {
        m_items[i]->loadValue();
    }
}

void KNPreferenceGeneral::saveConfigures()
{
    for(int i=0; i<GeneralPreferenceItemCount; i++)
    {
        m_items[i]->saveValue();
    }
}

void KNPreferenceGeneral::retranslate()
{
    m_titles[GeneralLibrary]->setText(tr("Library"));

    m_items[LibraryPath]->setCaption(tr("Library Path"));
}

void KNPreferenceGeneral::onLibraryMoved()
{
    //Set the latest data.
    m_systemConfigure->setData("LibraryPath", m_libraryDetector->value());
    //Emit moved signal.
    emit libraryDirMoved();
}
