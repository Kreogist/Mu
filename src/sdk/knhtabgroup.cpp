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
#include <QAbstractButton>

#include "knhtabgroup.h"

KNHTabGroup::KNHTabGroup(QWidget *parent) :
    KNAbstractTabGroup(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight, this))
{
    //Configure the main layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(5);
    setLayout(m_mainLayout);

    //Add a stretch to layout to make the item right align.
    m_mainLayout->addStretch();
}

bool KNHTabGroup::isEmpty()
{
    //The group empty state is the same as the layout state.
    return m_mainLayout->isEmpty();
}

int KNHTabGroup::addTabToWidget(QAbstractButton *tab)
{
    //Add the tab to layout.
    m_mainLayout->addWidget(tab);
    //The second maximum index of the main layout will be the tab's index.
    //Because we have a strectch at beginning.
    return m_mainLayout->count()-2;
}

QAbstractButton *KNHTabGroup::tabAt(int index)
{
    Q_ASSERT(index>-1 && index<m_mainLayout->count());
    //Get the index from the main layout.
    return static_cast<QAbstractButton *>
            (m_mainLayout->itemAt(index+1)->widget());
}
