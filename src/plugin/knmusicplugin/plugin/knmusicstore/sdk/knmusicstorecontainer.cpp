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
#include "knsideshadowwidget.h"

#include "knmusicstorecontainer.h"

#define ShadowSize 15

KNMusicStoreContainer::KNMusicStoreContainer(QWidget *parent) :
    QScrollArea(parent),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow,
                                       this)),
    m_titleBar(nullptr)
{
}

void KNMusicStoreContainer::setTitleBar(QWidget *titleBar)
{
    //Check pointer first.
    if(m_titleBar)
    {
        //Already has a title bar, ignore the operation.
        return;
    }
    //Save the title bar.
    m_titleBar=titleBar;
    //Check title bar.
    if(!m_titleBar)
    {
        //We cannot do anything about a null thing.
        return;
    }
    //Change the relationship.
    m_titleBar->setParent(this);
    //Reset the view port margin.
    setViewportMargins(0, m_titleBar->height(), 0, 0);
    //Resize the title bar.
    m_titleBar->resize(width(), m_titleBar->height());
}

void KNMusicStoreContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QScrollArea::resizeEvent(event);
    //Check title bar first.
    if(m_titleBar)
    {
        //Resize the title bar.
        m_titleBar->resize(width(), m_titleBar->height());
    }
    //Resize the scroll bar.
    m_topShadow->resize(width(), ShadowSize);
}
