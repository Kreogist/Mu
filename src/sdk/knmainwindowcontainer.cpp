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
#include <QPropertyAnimation>

#include "knmainwindowcontainer.h"

KNMainWindowContainer::KNMainWindowContainer(QWidget *parent) :
    QWidget(parent),
    m_preferenceAnimeGroup(new QParallelAnimationGroup(this))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Reset the widgets pointer, initial the animation.
    for(int i=0; i<ContainerWidgetCount; i++)
    {
        //Reset the pointer.
        m_elementWidget[i]=nullptr;
        //Initial the animation.
        m_elementAnime[i]=generateAnime();
        //Add it to parallel animation group.
        m_preferenceAnimeGroup->addAnimation(m_elementAnime[i]);
    }
    //Configure the properties of the animations.
    m_elementAnime[Header]->setPropertyName("pos");
    m_elementAnime[MainWidget]->setPropertyName("pos");
    m_elementAnime[PreferencePanel]->setPropertyName("geometry");
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

void KNMainWindowContainer::showPreference()
{
    //Update the header position.
    m_elementAnime[Header]->setStartValue(QPoint(0,0));
    m_elementAnime[Header]->setEndValue(
                QPoint(0,
                       -m_elementWidget[Header]->height()));
    //Update the main widget position.
    m_elementAnime[MainWidget]->setStartValue(
                QPoint(0,
                       m_elementWidget[Header]->height()));
    m_elementAnime[MainWidget]->setEndValue(QPoint(0, height()));
    //Update the preference geometry.
    m_elementAnime[PreferencePanel]->setStartValue(
                QRect(width()>>2, height()>>2, width()>>1, height()>>1));
    m_elementAnime[PreferencePanel]->setEndValue(QRect(0,0,width(),height()));
    //Start the animation.
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::hidePreference()
{
    ;
}

void KNMainWindowContainer::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Resize the header widget.
    if(m_elementWidget[Header]!=nullptr)
    {
        m_elementWidget[Header]->resize(width(),
                                        m_elementWidget[Header]->height());
    }
    //Resize the main widget and reposition the main widget if there's no
    //animation running.
    if(m_elementWidget[MainWidget]!=nullptr)
    {
        //Resize the main widget for no reason.
        m_elementWidget[MainWidget]->resize(
                    width(),
                    height()-m_elementWidget[Header]->height());
        //Check the state of the animation group.
        if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
        {
            //Update the animation group parameters.
            ;
        }
        else
        {
            //Check the position the main widget.
            ;
        }
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
        //Set the animation target.
        m_elementAnime[index]->setTargetObject(widget);
    }
}

inline QPropertyAnimation *KNMainWindowContainer::generateAnime()
{
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    anime->setEasingCurve(QEasingCurve::OutCubic);
    return anime;
}
