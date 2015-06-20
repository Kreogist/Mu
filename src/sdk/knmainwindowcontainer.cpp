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
#include <QParallelAnimationGroup>

#include "knmainwindowcontainer.h"

KNMainWindowContainer::KNMainWindowContainer(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Reset the widgets pointer, initial the animation.
    for(int i=0; i<ContainerWidgetCount; i++)
    {
        m_elementWidget[i]=nullptr;
    }
}

QWidget *KNMainWindowContainer::header() const
{
    return m_elementWidget[Header];
}

void KNMainWindowContainer::setHeader(QWidget *header)
{
    setWidget(Header, header);
}

QWidget *KNMainWindowContainer::mainWidget() const
{
    return m_elementWidget[MainWidget];
}

void KNMainWindowContainer::setMainWidget(QWidget *mainWidget)
{
    setWidget(MainWidget, mainWidget);
}

QWidget *KNMainWindowContainer::preferencePanel() const
{
    return m_elementWidget[PreferencePanel];
}

void KNMainWindowContainer::setPreferencePanel(QWidget *preferencePanel)
{
    setWidget(PreferencePanel, preferencePanel);
}

void KNMainWindowContainer::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Resize the header widget.
    if(m_elementWidget[Header]!=nullptr)
    {
        m_elementWidget[Header]->resize(width(), m_elementWidget[Header]->height());
    }
}

inline void KNMainWindowContainer::updateTheStackRelationship()
{
    if(m_elementWidget[PreferencePanel]!=nullptr)
    {
        m_elementWidget[PreferencePanel]->lower();
    }
    if(m_elementWidget[Header]!=nullptr)
    {
        m_elementWidget[Header]->raise();
    }
}

void KNMainWindowContainer::setWidget(const int &index, QWidget *widget)
{
    //Check whether the index has already been set a widget.
    if(m_elementWidget[index]==nullptr)
    {
        //Save the preference panel.
        m_elementWidget[index] = widget;
        //Change the parent relationship.
        m_elementWidget[index]->setParent(this, Qt::Widget);
        //Update the stack relationship.
        updateTheStackRelationship();
    }
}
