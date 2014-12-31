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
#include "preference/knpreferenceitempathbrowser.h"

#include "knlocalemanager.h"

#include "knpreferencegeneralpanel.h"

KNPreferenceGeneralPanel::KNPreferenceGeneralPanel(QWidget *parent) :
    KNPreferenceWidgetsPanel(parent)
{
    //Link the retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNPreferenceGeneralPanel::retranslate);
    //Do restranslate.
    retranslate();
}

KNPreferenceGeneralPanel::~KNPreferenceGeneralPanel()
{
    ;
}

void KNPreferenceGeneralPanel::getLibrarySettings(KNPreferenceTitleInfo &listTitle,
                                                  QList<KNPreferenceItemInfo> &list)
{
    listTitle.advanced=false;
    listTitle.title=tr("Library");
    listTitle.titleIdentifier="Library";

    list.clear();
    list.append(KNPreferenceItemGlobal::generateInfo(PathEdit,
                                                     tr("Library Path"),
                                                     "LibraryPath",
                                                     KNGlobal::libraryPath()));
}

void KNPreferenceGeneralPanel::retranslate()
{
    //Initial the basic items.
    KNPreferenceTitleInfo blockTitle;
    QList<KNPreferenceItemInfo> blockItemList;
    //Get library settings and insert to panel.
    getLibrarySettings(blockTitle, blockItemList);
    insertItemInfoList(blockTitle, blockItemList);
    //Get the library path detector.
    KNPreferenceItemPathBrowser *currentLibraryPathDetector=
            (KNPreferenceItemPathBrowser *)itemFromValueName("LibraryPath");
    if(currentLibraryPathDetector!=m_libraryDetector)
    {
        //Disconnect the original detector.
        if(m_libraryDetector!=nullptr)
        {
            disconnect(m_libraryDetector, &KNPreferenceItemPathBrowser::dirMoved,
                       this, &KNPreferenceGeneralPanel::onLibraryMoved);
        }
        //Get the new detector.
        m_libraryDetector=currentLibraryPathDetector;
        //Linke the new detector.
        connect(m_libraryDetector, &KNPreferenceItemPathBrowser::dirMoved,
                this, &KNPreferenceGeneralPanel::onLibraryMoved);
    }
}

void KNPreferenceGeneralPanel::onLibraryMoved()
{
    //Get the latest data.
    KNGlobal::instance()->setCustomData(panelName(),
                                        "LibraryPath",
                                        m_libraryDetector->value());
    //Emit moved signal.
    emit libraryDirMoved();
}
