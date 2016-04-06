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

#include "knanimelabelbutton.h"
#include "knlocalemanager.h"

#include "knmusicstoreglobal.h"

#include "knmusicstoretitlebar.h"

#define TitleBarHeight 50

KNMusicStoreTitleBar::KNMusicStoreTitleBar(QWidget *parent) :
    KNMouseSenseWidget(parent),
    m_buttonLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_homeButton(new KNAnimeLabelButton(this))
{
    updateObjectName("MusicStoreTitle");
    //Set properties.
    setFixedHeight(TitleBarHeight);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->addStretch();
    //Initial the container.
    QWidget *container=new QWidget(this);
    mainLayout->addWidget(container);
    mainLayout->addStretch();
    //Configure the container.
    container->setFixedWidth(knMusicStoreGlobal->storeContentWidth());

    //Initial the container layout.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               container);
    //Add button layout to container layout.
    containerLayout->addLayout(m_buttonLayout);
    containerLayout->addStretch();
    //Add the label to the layout.
    m_buttonLayout->addWidget(m_homeButton);

    //Link translator.
    knI18n->link(this, &KNMusicStoreTitleBar::retranslate);
    retranslate();
}

void KNMusicStoreTitleBar::appendLabel(QWidget *indicator,
                                       KNAnimeLabelButton *label)
{
    //Append the widget to list.
    m_indicatorStack.append(indicator);
    m_labelStack.append(label);
    //Append widget to layout.
    m_buttonLayout->addWidget(indicator);
    m_buttonLayout->addWidget(label);
}

void KNMusicStoreTitleBar::retranslate()
{
    //Update the home button label.
    m_homeButton->setText(tr("Store"));
}

void KNMusicStoreTitleBar::onActionLabelPress(int index)
{
    //Show hide the labels after the index.
    for(int i=index; i<m_indicatorStack.size(); ++i)
    {
        //Hide the widget and indicator.
        m_indicatorStack.at(i)->hide();
        m_labelStack.at(i)->hide();
    }
}
