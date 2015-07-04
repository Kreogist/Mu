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

#include "knthememanager.h"

#include "knpreferencetitlebar.h"
#include "knshadowscrollarea.h"
#include "knpreferenceitemlist.h"
#include "knlinearsensewidget.h"
#include "knsideshadowwidget.h"

#include "knpreferencesidebar.h"

KNPreferenceSidebar::KNPreferenceSidebar(QWidget *parent) :
    QWidget(parent),
    m_titleBar(new KNPreferenceTitleBar(this)),
    m_itemList(new KNPreferenceItemList(this)),
    m_bottomBar(new KNLinearSenseWidget(this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_shadowWidth(15)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedWidth(250);
    //Initial shadow scroll area.
    KNShadowScrollArea *scrollArea=new KNShadowScrollArea(this);
    scrollArea->lower();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_itemList);
    //Link the item list.
    connect(m_itemList, &KNPreferenceItemList::currentIndexChange,
            this, &KNPreferenceSidebar::requireChangeContent);
    //Initial bottom bar.
    initialBottomBar();

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add the title bar widget.
    mainLayout->addWidget(m_titleBar);
    //Add the shadow scroll area widget.
    mainLayout->addWidget(scrollArea, 1);
    //Add the bottom bar.
    mainLayout->addWidget(m_bottomBar);

    //Link requests.
    connect(m_titleBar, &KNPreferenceTitleBar::requireClosePreference,
            this, &KNPreferenceSidebar::requireClosePreference);
}

void KNPreferenceSidebar::addItemWidget(KNPreferenceItem *item)
{
    m_itemList->addItemWidget(item);
}

void KNPreferenceSidebar::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Move the shadow.
    m_rightShadow->setGeometry(width()-m_shadowWidth,
                               0,
                               m_shadowWidth,
                               height());
}

void KNPreferenceSidebar::initialBottomBar()
{
    //Configure bottom bar.
    m_bottomBar->setObjectName("PreferenceSidebarBottom");
    m_bottomBar->setContentsMargins(0,0,0,0);
    m_bottomBar->setFixedHeight(34);
    knTheme->registerWidget(m_bottomBar);
}

