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
#include <QGraphicsOpacityEffect>

#include "knmainwindowcontainer.h"

KNMainWindowContainer::KNMainWindowContainer(QWidget *parent) :
    QWidget(parent),
    m_preferenceAnimeGroup(new QParallelAnimationGroup(this)),
    m_preferenceOpacity(generateAnime("opacity")),
    m_preferenceOpacityEffect(new QGraphicsOpacityEffect(this)),
    m_headerHeight(-1)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Reset the widgets pointer, initial the animation.
    QByteArray propertyList[ContainerWidgetCount];
    propertyList[Header]="pos";
    propertyList[MainWidget]="pos";
    propertyList[PreferencePanel]="geometry";
    for(int i=0; i<ContainerWidgetCount; i++)
    {
        //Reset the pointer.
        m_elementWidget[i]=nullptr;
        //Initial the animation.
        m_elementAnime[i]=generateAnime(propertyList[i]);
        //Add it to parallel animation group.
        m_preferenceAnimeGroup->addAnimation(m_elementAnime[i]);
    }

    //Add opacity animation to anime group.
    m_preferenceAnimeGroup->addAnimation(m_preferenceOpacity);
    //Configure the opacity animation.
    m_preferenceOpacity->setTargetObject(m_preferenceOpacityEffect);
    //Configure the default effect.
    m_preferenceOpacityEffect->setOpacity(0.0);
}

QWidget *KNMainWindowContainer::header() const
{
    return m_elementWidget[Header];
}

void KNMainWindowContainer::setHeader(QWidget *header)
{
    //Save header widget.
    setWidget(Header, header);
    //Save the height of header widget.
    m_headerHeight=header->height();
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
    //Set the preference widget.
    setWidget(PreferencePanel, preferencePanel);
    //Set the opacity effect to the widget.
    preferencePanel->setGraphicsEffect(m_preferenceOpacityEffect);
}

void KNMainWindowContainer::showPreference()
{
    //Ignore the show request if the animation is running.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }

    //Update the header animation properties.
    m_elementAnime[Header]->setStartValue(QPoint(0,0));
    m_elementAnime[Header]->setEndValue(QPoint(0, -m_headerHeight));
    //Update the main widget animation start value property.
    m_elementAnime[MainWidget]->setStartValue(QPoint(0, m_headerHeight));
    //Update the opacity animation properties.
    m_preferenceOpacity->setStartValue(0.0);
    m_preferenceOpacity->setEndValue(1.0);
    //Update the container size related parameters.
    updateShowAnimeParameters();

    //Start the animation.
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::hidePreference()
{
    //Ignore the hide request if the animation is running.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }

    //Update the header position.
    m_elementAnime[Header]->setStartValue(QPoint(0, -m_headerHeight));
    m_elementAnime[Header]->setEndValue(QPoint(0,0));
    //Update the main widget animation end value.
    m_elementAnime[MainWidget]->setEndValue(QPoint(0, m_headerHeight));
    //Update the opacity animation properties.
    m_preferenceOpacity->setStartValue(1.0);
    m_preferenceOpacity->setEndValue(0.0);
    //Update the container size related parameters.
    updateHideAnimeParameters();

    //Start the animation.
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Resize the header widget.
    if(m_elementWidget[Header]!=nullptr)
    {
        m_elementWidget[Header]->resize(width(), m_headerHeight);
    }
    bool animationRunning=false;
    //Check the state of the animation group.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        //Set animation running flag.
        animationRunning=true;
        //Update the animation group parameters.
        //If the end value of the opacity animation is 1.0, means the animation
        //is showing the preference panel. Update the animation properies via
        //function updateShowAnimeParameters(), or else
        //updateHideAnimeParameters().
        if(m_preferenceOpacity->endValue().toReal()==1.00)
        {
            updateShowAnimeParameters();
        }
        else
        {
            updateHideAnimeParameters();
        }
    }
    //Resize the main widget and reposition the main widget if there's no
    //animation running.
    if(m_elementWidget[MainWidget]!=nullptr)
    {
        //Resize the main widget for no reason.
        m_elementWidget[MainWidget]->resize(width(), height()-m_headerHeight);
        //If the animation is not running, update the main widget position if
        //main widget is at the bottom of the window.
        if((!animationRunning) &&
                m_elementWidget[MainWidget]->y()!=m_headerHeight)
        {
            m_elementWidget[MainWidget]->move(0, height());
        }
    }
    //Resize the preference widget and resize the preference widget if it's
    //visible, and animation is not running.
    if(m_elementWidget[PreferencePanel]!=nullptr &&
            (!animationRunning) &&
            m_preferenceOpacityEffect->opacity()==1.0)
    {
        m_elementWidget[PreferencePanel]->resize(width(), height());
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

inline void KNMainWindowContainer::updateShowAnimeParameters()
{
    //Update the preference animation properties.
    m_elementAnime[PreferencePanel]->setStartValue(
                QRect(width()>>2, height()>>2, width()>>1, height()>>1));
    m_elementAnime[PreferencePanel]->setEndValue(QRect(0,0,width(),height()));
    //Update the main widget animation end value property.
    m_elementAnime[MainWidget]->setEndValue(QPoint(0, height()));
}

inline void KNMainWindowContainer::updateHideAnimeParameters()
{
    //Update the preference animation properties.
    m_elementAnime[PreferencePanel]->setStartValue(QRect(0,0,width(),height()));
    m_elementAnime[PreferencePanel]->setEndValue(
                QRect(width()>>2, height()>>2, width()>>1, height()>>1));
    //Update the main widget animation start value property.
    m_elementAnime[MainWidget]->setStartValue(QPoint(0, height()));
}

inline QPropertyAnimation *KNMainWindowContainer::generateAnime(
        const QByteArray &propertyName)
{
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    anime->setEasingCurve(QEasingCurve::InOutCirc);
    anime->setPropertyName(propertyName);
    return anime;
}
