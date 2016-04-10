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
#include "kndarkwaitingwheel.h"
#include "knsearchbox.h"

#include "knmusicstoreglobal.h"
#include "knmusicstoreutil.h"

#include "knmusicstoretitlebar.h"

#define TitleBarHeight 50

KNMusicStoreTitleBar::KNMusicStoreTitleBar(QWidget *parent) :
    KNMouseSenseWidget(parent),
    m_buttonLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_containerLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_homeButton(new KNAnimeLabelButton(this)),
    m_networkState(new KNDarkWaitingWheel(this))
{
    updateObjectName("MusicStoreTitle");
    //Set properties.
    setFixedHeight(TitleBarHeight);
    //Configure state widgets.
    m_networkState->hide();
    //Link the home button with the signal.
    m_homeButton->setCursor(Qt::PointingHandCursor);
    connect(m_homeButton, &KNAnimeLabelButton::clicked,
            this, &KNMusicStoreTitleBar::requireShowHome);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->addStretch();
    //Initial the container.
    QWidget *container=new QWidget(this);
    mainLayout->addWidget(container, 1);
    mainLayout->addStretch();
    //Configure the container.
    container->setMaximumWidth(knMusicStoreGlobal->storeContentWidth());

    //Configure the container layout.
    m_containerLayout->setSpacing(5);
    container->setLayout(m_containerLayout);
    //Add button layout to container layout.
    m_containerLayout->addLayout(m_buttonLayout);
    m_containerLayout->addStretch();
    //Add the label to the layout.
    m_buttonLayout->addWidget(m_homeButton);
    //Add state widget to container layout.
    m_containerLayout->addWidget(m_networkState);

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

void KNMusicStoreTitleBar::appendIcon(QWidget *iconWidget)
{
    //Append widget to the end of the container layout.
    m_containerLayout->addWidget(iconWidget);
}

void KNMusicStoreTitleBar::setStatesButton(int state, bool value)
{
    switch(state)
    {
    case KNMusicStoreUtil::StateNetwork:
        //Show and start the value.
        if(value)
        {
            //Start and show the icon.
            m_networkState->show();
            m_networkState->startTick();
        }
        else
        {
            //Stop and hide the icon.
            m_networkState->hide();
            m_networkState->stopTick();
        }
        break;
    default:
        break;
    }
}

void KNMusicStoreTitleBar::retranslate()
{
    //Update the home button label.
    m_homeButton->setText(tr("Store"));
}

void KNMusicStoreTitleBar::setNavigationLevel(int index)
{
    //Show the label. Because there's no home label, the target label will be
    //index-1.
    int targetIndex=index-1;
    //Chekc the target index validation.
    if(targetIndex>0)
    {
        //Show the widget and indicator.
        m_indicatorStack.at(targetIndex)->show();
        m_labelStack.at(targetIndex)->show();
    }
    //Hide the labels after the index.
    for(int i=index; i<m_indicatorStack.size(); ++i)
    {
        //Hide the widget and indicator.
        m_indicatorStack.at(i)->hide();
        m_labelStack.at(i)->hide();
    }
}
