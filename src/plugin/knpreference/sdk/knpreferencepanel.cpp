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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>

#include "knthememanager.h"
#include "knpreferencepanel.h"
#include "kndpimanager.h"

//Items
#include "items/knpreferencepanelitem.h"
#include "items/knpreferencepanelbooleanitem.h"
#include "items/knpreferencepanelfontitem.h"
#include "items/knpreferencepanelintitem.h"
#include "items/knpreferencepanelcomboitem.h"
#include "items/knpreferencepanelshortcutitem.h"

#include <QDebug>

#define TitleTopMargin 20
#define TitleHeightFixed 3
#define TitleBottomMargin 5

KNPreferencePanel::KNPreferencePanel(QWidget *parent) :
    QWidget(parent),
    m_titleFont(font()),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this))
{
    //Set properties.
    setAutoFillBackground(true);
    setFixedWidth(knDpi->width(542));
    //Set layout.
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
    //Get the palette.
    setPalette(knTheme->getPalette("PreferencePanel"));
    //Set the title font.
    m_titleFont.setPixelSize(knDpi->height(14));
}

void KNPreferencePanel::setPanelBlocks(
        const QList<PreferencePanelBlock> &blocks)
{
    //Parse each block.
    for(auto block : blocks)
    {
        //Generate the label.
        QLabel *titleLabel=new QLabel(this);
        //Configure the label.
        titleLabel->setFixedHeight(m_titleFont.pixelSize()+
                                   knDpi->height(TitleTopMargin+
                                                 TitleHeightFixed+
                                                 TitleBottomMargin));
        titleLabel->setContentsMargins(knDpi->margins(20, TitleTopMargin,
                                                      20, TitleBottomMargin));
        titleLabel->setText(block.blockName);
        titleLabel->setPalette(palette());
        titleLabel->setFont(m_titleFont);
        //Add the block title to layout.
        m_mainLayout->addWidget(titleLabel);
        //Loop and generate all the item.
        for(auto item : block.options)
        {
            //Generate the item according to its type.
            KNPreferencePanelItem *itemWidget=generateItem(item);
            //Check the item widget.
            if(!itemWidget)
            {
                //Go on to next item.
                continue;
            }
            //Check the item widget is advanced item.
            if(item.advanced)
            {
                //Add the widget to the advanced item list.
                m_advancedItems.append(itemWidget);
            }
            //Add item to the layout.
            m_mainLayout->addWidget(itemWidget);
        }
    }
    //Add stretch.
    m_mainLayout->addStretch();
}

void KNPreferencePanel::setAdvancedItemShown(bool isShow)
{
    //Loop in the advanced item list.
    for(auto item : m_advancedItems)
    {
        //Set the visible item to the item.
        item->setVisible(isShow);
    }
}

inline KNPreferencePanelItem *KNPreferencePanel::generateItem(
        const PreferencePanelOption &option)
{
    //Prepare the item.
    KNPreferencePanelItem *item=nullptr;
    //Generate the item.
    switch(option.type)
    {
    case TypeBoolean:
        item=new KNPreferencePanelBooleanItem(this);
        break;
    case TypeCombo:
        item=new KNPreferencePanelComboItem(this);
        break;
    case TypeInt:
        item=new KNPreferencePanelIntItem(this);
        break;
    case TypeFont:
        item=new KNPreferencePanelFontItem(this);
        break;
    case TypeShortcut:
        item=new KNPreferencePanelShortcutItem(this);
        break;
    default:
        break;
    }
    //Set the option.
    item->setPreferenceOption(option);
    //Give back the item.
    return item;
}
