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

#include "knmainwindowiconbutton.h"
#include "kncategoryplugin.h"
#include "knlocalemanager.h"

#include "knmainwindowheader.h"

#include <QDebug>

KNMainWindowHeader::KNMainWindowHeader(QWidget *parent) :
    KNMainWindowHeaderBase(parent),
    m_widgetLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_categoryPlugin(nullptr),
    m_iconButton(new KNMainWindowIconButton(this))
{
    updateObjectName("MainWindowHeader");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(70);

    //Initial the layout of the header.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add button to main layout.
    mainLayout->addWidget(m_iconButton);
    //Configure the widget layout.
    m_widgetLayout->setContentsMargins(0,0,0,0);
    m_widgetLayout->setSpacing(0);
    //Add the widget layout to header.
    mainLayout->addLayout(m_widgetLayout, 1);

    //Link the icon button.
    connect(m_iconButton, SIGNAL(clicked(bool)),
            this, SIGNAL(requireShowPreference()));

    //Link retranlsate.
    knI18n->link(this, &KNMainWindowHeader::retranslate);
    retranslate();
}

void KNMainWindowHeader::addHeaderWidget(QWidget *widget,
                                         int stretch,
                                         Qt::Alignment alignment)
{
    //Add the widget to widget layout.
    m_widgetLayout->addWidget(widget, stretch, alignment);
}

void KNMainWindowHeader::retranslate()
{
    //Check the category plugin first.
    if(m_categoryPlugin!=nullptr)
    {
        //Update the button text.
        m_iconButton->setButtonText(m_categoryPlugin->title());
    }
}

void KNMainWindowHeader::setCategoryPlugin(KNCategoryPlugin *categoryPlugin)
{
    //Save the category plugin.
    m_categoryPlugin = categoryPlugin;
    //Ensure this is not a null widget.
    if(m_categoryPlugin==nullptr)
    {
        return;
    }
    //Add the header widget.
    addHeaderWidget(m_categoryPlugin->headerWidget());
    //Set the icon of the plugin.
    m_iconButton->setButtonIcon(m_categoryPlugin->icon());
    //Set the text of the plugin.
    m_iconButton->setButtonText(m_categoryPlugin->title());
}
