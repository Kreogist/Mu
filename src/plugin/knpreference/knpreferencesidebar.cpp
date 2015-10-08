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
#include "knlocalemanager.h"
#include "knpreferenceitem.h"

#include "knpreferencesidebar.h"

#include <QDebug>

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
    //Link title bar requests.
    connect(m_titleBar, &KNPreferenceTitleBar::requireClosePreference,
            this, &KNPreferenceSidebar::requireClosePreference);
    //Initial shadow scroll area.
    KNShadowScrollArea *scrollArea=new KNShadowScrollArea(this);
    scrollArea->lower();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_itemList);
    //Link the item list.
    connect(m_itemList, &KNPreferenceItemList::currentIndexChange,
            this, &KNPreferenceSidebar::onActionIndexChanged);
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

    //Link retranslate.
    knI18n->link(this, &KNPreferenceSidebar::retranslate);
    retranslate();
}

void KNPreferenceSidebar::addItemWidget(KNPreferenceItem *item)
{
    //Add the item to item list.
    m_itemList->addTab(item);
}

void KNPreferenceSidebar::setHeaderText(const QString &text)
{
    //Set the header text to header label.
    m_titleBar->setText(text);
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

void KNPreferenceSidebar::retranslate()
{
    //Check the current index. Update the title if the current index is not -1.
    if(m_itemList->currentIndex()!=-1)
    {
        m_titleBar->setText(m_itemList->itemText(m_itemList->currentIndex()));
    }
}

void KNPreferenceSidebar::onActionIndexChanged(const int &index)
{
    //Change the title icon.
    m_titleBar->setIcon(m_itemList->itemHeaderIcon(index));
    //Change the title text.
    m_titleBar->setText(m_itemList->itemText(index));
    //Emit content change requirement.
    emit requireChangeContent(index);
}

void KNPreferenceSidebar::initialBottomBar()
{
    //Configure bottom bar.
    m_bottomBar->setObjectName("PreferenceSidebarBottom");
    m_bottomBar->setContentsMargins(0,0,0,0);
    m_bottomBar->setFixedHeight(34);
    knTheme->registerWidget(m_bottomBar);
}

