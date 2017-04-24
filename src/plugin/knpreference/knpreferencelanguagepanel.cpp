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
#include <QSignalMapper>

#include "knlocalemanager.h"
#include "knconfigure.h"
#include "knglobal.h"

#include "knpreferencelanguageitem.h"
#include "knpreferencelanguagepanelitem.h"

#include "knpreferencelanguagepanel.h"

#include <QDebug>

KNPreferenceLanguagePanel::KNPreferenceLanguagePanel(QWidget *parent) :
    QScrollArea(parent),
    m_languageListItem(new KNPreferenceLanguageItem(this)),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_languageRequests(new QSignalMapper(this)),
    m_userConfigure(knGlobal->userConfigure())
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    //Configure the language list item.
    m_languageListItem->setHeaderIcon(
                QPixmap(":/plugin/preference/header/language.png"));

    //Initial the container.
    QWidget *container=new QWidget(this);
    //Set container properties.
    container->setContentsMargins(0,0,0,0);
    setWidget(container);

    //Initial the layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    container->setLayout(m_mainLayout);
    //Add default stretch.
    m_mainLayout->addStretch();

    //Initial the language switch request map.
    connect(m_languageRequests,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            this, &KNPreferenceLanguagePanel::setCurrentLanguage);
}

void KNPreferenceLanguagePanel::generateLanguageList()
{
    //Get the language size.
    for(int i=0, languageSize=knI18n->languageCount(); i<languageSize; ++i)
    {
        //Generate language item for the current language.
        KNPreferenceLanguagePanelItem *item=
                new KNPreferenceLanguagePanelItem(this);
        //Set the language information.
        item->setLanguageIcon(knI18n->languageIcon(i));
        item->setLanguageName(knI18n->languageName(i));
        //Link the item to the set language maps.
        connect(item, SIGNAL(clicked(bool)), m_languageRequests, SLOT(map()));
        //Set mapping in the language request.
        m_languageRequests->setMapping(item, i);
        //Add the item to main layout.
        m_mainLayout->insertWidget(m_mainLayout->count()-1, item);
    }
    //Sync the language item.
    syncLanguageItem(knI18n->currentLanguageIndex());
    //Remove the previous index checked state.
    static_cast<KNPreferenceLanguagePanelItem *>(
                m_mainLayout->itemAt(
                    knI18n->currentLanguageIndex(
                        ))->widget())->setChecked(true);
}

void KNPreferenceLanguagePanel::setCurrentLanguage(const int &index)
{
    //Remove the previous index checked state.
    static_cast<KNPreferenceLanguagePanelItem *>(
                m_mainLayout->itemAt(
                    knI18n->currentLanguageIndex(
                        ))->widget())->setChecked(false);
    //Ask the locale manager to change the language.
    knI18n->setLanguage(index);
    //Save the language in the user configure.
    m_userConfigure->setData("Language", knI18n->languageKey(index));
    //Update the language panel item.
    syncLanguageItem(index);
}

inline void KNPreferenceLanguagePanel::syncLanguageItem(const int &index)
{
    //Get the international translate in current language.
    QString headerTitle=tr("Languages");
    //Update the header title.
    emit requireUpdateTitle(headerTitle);
    //Set the item title.
    m_languageListItem->setText(headerTitle);
    //Set the language name.
    m_languageListItem->setLanguageName(knI18n->languageName(index));
    //Set the language icon.
    m_languageListItem->setIcon(knI18n->languageIcon(index));
}

KNPreferenceLanguageItem *KNPreferenceLanguagePanel::languageListItem() const
{
    return m_languageListItem;
}
