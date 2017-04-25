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
#include <QAbstractButton>

#include "knabstracttabgroup.h"

#include <QDebug>

KNAbstractTabGroup::KNAbstractTabGroup(QWidget *parent) :
    QWidget(parent),
    m_itemMapper(new QSignalMapper(this)),
    m_currentIndex(-1),
    m_autoSelect(true)
{
    //Link the signal mapper.
    connect(m_itemMapper,
            static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
            this, &KNAbstractTabGroup::onActionMapped);
}

void KNAbstractTabGroup::addTab(QAbstractButton *tab)
{
    //Check is this tab the first tab. If the group is empty before we add this
    //tab, this tab should be the first tab.
    bool isFirstTab=isEmpty();
    //Set the tab checked if the tab is the first tab when auto select enabled.
    if(m_autoSelect && isFirstTab)
    {
        //Set the current tab checked.
        tab->setChecked(true);
        //Set the current index to 0.
        m_currentIndex=0;
    }
    //Add the tab to the group, and tweak the UI elements. Get the index of the
    //tab. Link the item with the signal mapper, the id of the tab is the index.
    connect(tab, &QAbstractButton::pressed,
            m_itemMapper,
            static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
    m_itemMapper->setMapping(tab, addTabToWidget(tab));
    //Check if this is the first item. Emit the current changed signal if this
    //is the first item. This is only checked when auto selected enabled.
    if(m_autoSelect && isFirstTab)
    {
        //Select the default first item.
        emit currentIndexChange(0);
    }
}

int KNAbstractTabGroup::currentIndex() const
{
    return m_currentIndex;
}

bool KNAbstractTabGroup::isButtonSelected() const
{
    //When current index is -1, it means that none of them is selected.
    return (m_currentIndex!=-1);
}

void KNAbstractTabGroup::setCurrentIndex(int currentIndex)
{
    //Check if the current index is the same as the current one. Ignore this
    //kind of request.
    if(currentIndex==m_currentIndex)
    {
        return;
    }
    //Check the current index, if it is -1, ignore the operation.
    if(-1!=m_currentIndex)
    {
        //Unlock the previous item.
        tabAt(m_currentIndex)->setChecked(false);
    }
    //Save the current index.
    m_currentIndex=currentIndex;
    //Check the current index.
    tabAt(m_currentIndex)->setChecked(true);
    //Emit the current changed signal.
    emit currentIndexChange(m_currentIndex);
}

void KNAbstractTabGroup::reset()
{
    //Check the current index.
    if(-1==m_currentIndex)
    {
        //None of the button is selected.
        return;
    }
    //Clear the selection table.
    tabAt(m_currentIndex)->setChecked(false);
    //Update the current index to -1.
    m_currentIndex=-1;
    //Emit the current changed signal.
    emit currentIndexChange(-1);
}

void KNAbstractTabGroup::onActionMapped(int index)
{
    //If the mapped index is the current index, ignore this index changed
    //mapping.
    if(index==m_currentIndex)
    {
        return;
    }
    //Set the current index.
    setCurrentIndex(index);
}

bool KNAbstractTabGroup::autoSelect() const
{
    return m_autoSelect;
}

void KNAbstractTabGroup::setAutoSelect(bool autoSelect)
{
    m_autoSelect = autoSelect;
}
