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

#include "knpreferenceitem.h"

#include "knpreferenceitemlist.h"

KNPreferenceItemList::KNPreferenceItemList(QWidget *parent) :
    KNAbstractTabGroup(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedHeight(10);

    //Set layout.
    m_mainLayout->setContentsMargins(0,5,0,5);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

QString KNPreferenceItemList::itemText(const int &index)
{
    //Return the item text.
    return tabAt(index)->text();
}

bool KNPreferenceItemList::isEmpty()
{
    //The group empty state is the same as the layout state.
    return m_mainLayout->isEmpty();
}

int KNPreferenceItemList::addTabToWidget(QAbstractButton *tab)
{
    //Add the tab item to the layout.
    m_mainLayout->addWidget(tab);
    //Resize the list.
    setFixedHeight(height()+tab->height());
    //The maximum index of the main layout will be the tab's index.
    return m_mainLayout->count()-1;
}

QAbstractButton *KNPreferenceItemList::tabAt(const int &index)
{
    Q_ASSERT(index>-1 && index<m_mainLayout->count());
    //Get the index from the main layout.
    return static_cast<QAbstractButton *>
            (m_mainLayout->itemAt(index)->widget());
}
