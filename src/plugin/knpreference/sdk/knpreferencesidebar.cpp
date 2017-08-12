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
#include <QLabel>

#include "kndpimanager.h"
#include "knlinearsensewidget.h"
#include "knlocalemanager.h"
#include "knpreferencetitlebar.h"
#include "knpreferenceitem.h"
#include "knpreferenceitemlist.h"
#include "knroundswitchbutton.h"
#include "knshadowscrollarea.h"
#include "knsideshadowwidget.h"
#include "knthememanager.h"

#include "knpreferencesidebar.h"

#include <QDebug>

KNPreferenceSidebar::KNPreferenceSidebar(QWidget *parent) :
    QWidget(parent),
    m_titleBar(new KNPreferenceTitleBar(this)),
    m_fixedItemList(new KNPreferenceItemList(this)),
    m_itemList(new KNPreferenceItemList(this)),
    m_bottomBar(new KNLinearSenseWidget(this)),
    m_advancedButton(new KNRoundSwitchButton(this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_advancedLabel(new QLabel(this)),
    m_shadowWidth(knDpi->width(15))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedWidth(knDpi->width(250));
    //Configure the item list.
    m_itemList->setAutoSelect(false);
    //Configure the label.
    m_advancedLabel->setObjectName("PreferenceSidebarLabel");
    knTheme->registerWidget(m_advancedLabel);
    //Link title bar requests.
    connect(m_titleBar, &KNPreferenceTitleBar::requireClosePreference,
            this, &KNPreferenceSidebar::requireClosePreference);
    //Initial shadow scroll area.
    KNShadowScrollArea *scrollArea=new KNShadowScrollArea(this);
    scrollArea->lower();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_itemList);
    //Link the item list.
    connect(m_fixedItemList, &KNPreferenceItemList::currentIndexChange,
            this, &KNPreferenceSidebar::onFixedIndexChanged);
    connect(m_itemList, &KNPreferenceItemList::currentIndexChange,
            this, &KNPreferenceSidebar::onIndexChanged);
    //Initial bottom bar.
    initialBottomBar();

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add the title bar widget.
    mainLayout->addWidget(m_titleBar);
    //Add the fixed button layout.
    mainLayout->addWidget(m_fixedItemList);
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

void KNPreferenceSidebar::addFixedItemWidget(KNPreferenceItem *item)
{
    //Add the item to the fixed layout.
    m_fixedItemList->addTab(item);
}

bool KNPreferenceSidebar::isAdvancedShown() const
{
    //Return the button is checked.
    return m_advancedButton->isChecked();
}

void KNPreferenceSidebar::setHeaderText(const QString &text)
{
    //Set the header text to header label.
    m_titleBar->setText(text);
}

void KNPreferenceSidebar::setItemTitle(int index, const QString &title)
{
    //Change the item list text.
    m_itemList->setItemText(index, title);
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
    //Update advanced label.
    m_advancedLabel->setText(tr("Advanced Mode"));
}

void KNPreferenceSidebar::updateRoundedButtonPalette()
{
    //Update the advanced button.
    m_advancedButton->updatePalette(
                knTheme->getPalette("PreferenceAdvancedButton"));
}

void KNPreferenceSidebar::updateTitleBarText()
{
    //Check the current index. Update the title if the current index is not -1.
    if(m_itemList->currentIndex()!=-1)
    {
        //Update the title bar text.
        m_titleBar->setText(m_itemList->itemText(m_itemList->currentIndex()));
    }
    //Check the fixed title bar.
    else if(m_fixedItemList->currentIndex()!=-1)
    {
        //Update the title bar text.
        m_titleBar->setText(m_fixedItemList->itemText(
                                m_fixedItemList->currentIndex()));
    }
}

void KNPreferenceSidebar::setAdvancedShown(bool shown)
{
    //Set the shown to the advanced button.
    m_advancedButton->setChecked(shown);
}

void KNPreferenceSidebar::enableItemList()
{
    //Enable the fixed item list.
    m_fixedItemList->setEnabled(true);
    m_itemList->setEnabled(true);
}

void KNPreferenceSidebar::onIndexChanged(int index)
{
    //Check the index first.
    if(index==-1)
    {
        //This signal is send to reset the list, ignore.
        return;
    }
    //Check the fixed item is clicked.
    if(m_fixedItemList->isButtonSelected())
    {
        //A new item is clicked, reset fixed item list.
        m_fixedItemList->reset();
        //Emit content change requirement to the panel widget.
        emit requireChangeContent(m_fixedItemList->itemCount());
    }
    //Update title bar content.
    setTitleBarContent(m_itemList, index);
    //Emit the change panel requirement.
    emit requireChangePanel(index);
}

void KNPreferenceSidebar::onFixedIndexChanged(int index)
{
    //Check the index first.
    if(index==-1)
    {
        //This signal is send to reset the list, ignore.
        return;
    }
    //Update title bar content.
    setTitleBarContent(m_fixedItemList, index);
    //Emit content change requirement.
    emit requireChangeContent(index);
    //Check the item panel is selected.
    if(m_itemList->isButtonSelected())
    {
        //A new fixed item is clicked, reset the item list.
        m_itemList->reset();
    }
}

void KNPreferenceSidebar::initialBottomBar()
{
    //Configure bottom bar.
    m_bottomBar->setObjectName("PreferenceSidebarBottom");
    m_bottomBar->setContentsMargins(0, 0, 0, 0);
    m_bottomBar->setFixedHeight(knDpi->height(34));
    knTheme->registerWidget(m_bottomBar);
    //Set the sidebar layout.
    QBoxLayout *sideBarLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_bottomBar);
    sideBarLayout->setContentsMargins(knDpi->margins(10, 0, 10, 0));
    sideBarLayout->setSpacing(knDpi->width(6));
    m_bottomBar->setLayout(sideBarLayout);
    //Update the advanced button item.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNPreferenceSidebar::updateRoundedButtonPalette);
    updateRoundedButtonPalette();
    //Add the advanced button.
    connect(m_advancedButton, &KNRoundSwitchButton::toggled,
            this, &KNPreferenceSidebar::advancedToggle);
    sideBarLayout->addWidget(m_advancedButton, 0, Qt::AlignVCenter);
    sideBarLayout->addWidget(m_advancedLabel, 0, Qt::AlignVCenter);
    sideBarLayout->addStretch();
}

inline void KNPreferenceSidebar::setTitleBarContent(KNPreferenceItemList *list,
                                                    int index)
{
    //Change the title icon.
    m_titleBar->setIcon(list->itemHeaderIcon(index));
    //Change the title text.
    m_titleBar->setText(list->itemText(index));
}

