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
#include <QSignalMapper>
#include <QBoxLayout>

#include "knpreferenceitem.h"

#include "knpreferenceitemlist.h"

#include <QDebug>

KNPreferenceItemList::KNPreferenceItemList(QWidget *parent) :
    QWidget(parent),
    m_itemMapper(new QSignalMapper(this)),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_currentIndex(-1)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedHeight(10);

    //Link the signal mapper.
    connect(m_itemMapper,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            [=](const int &index)
            {
                if(index==m_currentIndex)
                {
                    return;
                }
                //Save the index.
                m_currentIndex=index;
                //Emit the current changed signal.
                emit currentIndexChange(m_currentIndex);
            });

    //Set layout.
    m_mainLayout->setContentsMargins(0,5,0,5);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

void KNPreferenceItemList::addItemWidget(KNPreferenceItem *item)
{
    //Check the count of main layout, if this is the first tab, select this tab.
    if(m_mainLayout->count()==0)
    {
        //Select the current tab.
        item->setChecked(true);
        //Set the current index to 0.
        m_currentIndex=0;
    }
    //Add the item to the list.
    m_mainLayout->addWidget(item);
    //Resize the list.
    setFixedHeight(height()+item->height());
    //Link the item with the signal mapper.
    connect(item, SIGNAL(pressed()), m_itemMapper, SLOT(map()));
    m_itemMapper->setMapping(item, m_mainLayout->count()-1);
    //Check if this is the first item. Emit the current changed signal if this
    //is the first item.
    if(m_mainLayout->count()==1)
    {
        emit currentIndexChange(0);
    }
}

QString KNPreferenceItemList::itemText(const int &index)
{
    //Return the item text.
    return itemAt(index)->text();
}

int KNPreferenceItemList::currentIndex() const
{
    return m_currentIndex;
}

void KNPreferenceItemList::setCurrentIndex(const int &currentIndex)
{
    Q_ASSERT(currentIndex>-1 && currentIndex<m_mainLayout->count());
    //Unlock the previous item.
    itemAt(m_currentIndex)->setChecked(false);
    //Save the current index.
    m_currentIndex=currentIndex;
    //Check the current index.
    itemAt(m_currentIndex)->setChecked(true);
    //Emit the current changed signal.
    emit currentIndexChange(m_currentIndex);
}

KNPreferenceItem *KNPreferenceItemList::itemAt(const int &index)
{
    Q_ASSERT(index>-1 && index<m_mainLayout->count());
    //Get the index from the main layout.
    return static_cast<KNPreferenceItem *>
            (m_mainLayout->itemAt(index)->widget());
}
