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
#include <QSignalMapper>

#include "knpreferencecategorylist.h"

#include <QDebug>

KNPreferenceCategoryList::KNPreferenceCategoryList(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Set main layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Initial the signal mapper.
    m_buttonMapper=new QSignalMapper(this);
    connect(m_buttonMapper, SIGNAL(mapped(int)),
            this, SLOT(setCurrentIndex(int)));

    m_mainLayout->addStretch();
}

QString KNPreferenceCategoryList::categoryText(const int &index) const
{
    return m_itemList.at(index)->text();
}

int KNPreferenceCategoryList::currentIndex() const
{
    return m_currentIndex;
}

int KNPreferenceCategoryList::itemCount() const
{
    return m_itemList.size();
}

void KNPreferenceCategoryList::addCategory(const QString &text,
                                           const QPixmap &pixmap)
{
    //Generate a item for this category
    KNPreferenceCategoryItem *currentItem=new KNPreferenceCategoryItem(this);
    currentItem->setText(text);
    currentItem->setIcon(pixmap);
    //Connect the signal.
    connect(currentItem, SIGNAL(clicked()),
            m_buttonMapper, SLOT(map()));
    int itemIndex=m_mainLayout->count()-1;
    m_buttonMapper->setMapping(currentItem, itemIndex);
    //Add the item to the list.
    m_itemList.append(currentItem);
    //Add to layout.
    m_mainLayout->insertWidget(itemIndex, currentItem);
}

void KNPreferenceCategoryList::setCategoryText(const int &index,
                                               const QString &caption)
{
    m_itemList.at(index)->setText(caption);
}

void KNPreferenceCategoryList::setCurrentIndex(const int &index)
{
    //Check is the last index available.
    if(m_currentIndex!=-1 && m_currentIndex<m_itemList.size())
    {
        //Set that item unchecked.
        m_itemList.at(m_currentIndex)->setChecked(false);
    }
    //Change the current index.
    m_currentIndex=index;
    //Set it checked and emit the signal
    m_itemList.at(m_currentIndex)->setChecked(true);
    emit currentIndexChanged(m_currentIndex);
}
