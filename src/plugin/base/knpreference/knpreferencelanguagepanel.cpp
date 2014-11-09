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
#include <QSignalMapper>

#include "knpreferencelanguagepanelitem.h"

#include "knlocalemanager.h"

#include "knpreferencelanguagepanel.h"

#include <QDebug>

KNPreferenceLanguagePanel::KNPreferenceLanguagePanel(QWidget *parent) :
    QScrollArea(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    //Initial the container.
    QWidget *container=new QWidget(this);
    //Set container properties.
    container->setContentsMargins(0,0,0,0);
    setWidget(container);

    //Initial the locale manager.
    m_localeManager=KNLocaleManager::instance();
    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    container->setLayout(m_mainLayout);

    m_mainLayout->addStretch();

    //Initial the request map.
    m_languageRequests=new QSignalMapper(this);
    connect(m_languageRequests, SIGNAL(mapped(int)),
            m_localeManager, SLOT(setLanguage(int)));
    //Generate the language list.
    generateLanguageList();
}

void KNPreferenceLanguagePanel::generateLanguageList()
{
    for(int i=0, languageSize=m_localeManager->languageSize();
        i<languageSize;
        i++)
    {
        //Generate the item for current language.
        KNPreferenceLanguagePanelItem *currentItem=
                new KNPreferenceLanguagePanelItem(this);
        //Set the language info.
        currentItem->setLanguageName(m_localeManager->languageNameAt(i));
        currentItem->setLanguageIcon(m_localeManager->languageIconAt(i));
        //Connect to request map.
        connect(currentItem, SIGNAL(requireSetLanguage()),
                m_languageRequests, SLOT(map()));
        m_languageRequests->setMapping(currentItem, i);
        //Add the language to layout.
        m_mainLayout->insertWidget(m_mainLayout->count()-1,
                                   currentItem);
    }
}
