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
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "knmainwindowcontainer.h"

KNMainWindowContainer::KNMainWindowContainer(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //The header widget and category should be fully visible at normal.
    //The settings widget shouldn't be visible.
    m_settingsEffect=new QGraphicsOpacityEffect(this);
    m_settingsEffect->setOpacity(0.0);

    //Initial the animation groups.
    m_preferenceAnimeGroup=new QParallelAnimationGroup(this);
    //Initial the opacity animation.
    m_settingOpacityAnime=new QPropertyAnimation(this);
    m_settingOpacityAnime->setTargetObject(m_settingsEffect);
    configureOpacityAnime(m_settingOpacityAnime);
    //Initial the position anime.
    m_headerPositionAnime=new QPropertyAnimation(this);
    configureGeometryAnime(m_headerPositionAnime);
    m_categoryPositionAnime=new QPropertyAnimation(this);
    configureGeometryAnime(m_categoryPositionAnime);
    m_settingPositionAnime=new QPropertyAnimation(this);
    configureGeometryAnime(m_settingPositionAnime);
}

void KNMainWindowContainer::setHeaderWidget(QWidget *widget)
{
    if(m_header==nullptr)
    {
        //Save the widget.
        m_header=widget;
        //Change the parent relationship.
        m_header->setParent(this);
        //Set anime target.
        m_headerPositionAnime->setTargetObject(m_header);
        //Refresh geometry.
        refreshGeometry();
    }
}

void KNMainWindowContainer::setCategoryStack(QWidget *widget)
{
    if(m_categoryStack==nullptr)
    {
        //Save the widget.
        m_categoryStack=widget;
        //Change the parent and set effects.
        m_categoryStack->setParent(this);
        //Set anime target.
        m_categoryPositionAnime->setTargetObject(m_categoryStack);
        //Refresh widgets geometry.
        refreshGeometry();
    }
}

void KNMainWindowContainer::setCategorySwitcher(QWidget *widget)
{
    if(m_categorySwitcher==nullptr)
    {
        //Save the widget.
        m_categorySwitcher=widget;
        //Change the parent.
        m_categorySwitcher->setParent(this);
        //Set up.
        m_categorySwitcher->raise();
        //Hide it.
        m_categorySwitcher->hide();
    }
}

void KNMainWindowContainer::setPreferencePanel(QWidget *widget)
{
    if(m_preferencePanel==nullptr)
    {
        //Save the widget.
        m_preferencePanel=widget;
        //Change the parent.
        m_preferencePanel->setParent(this);
        m_preferencePanel->setGraphicsEffect(m_settingsEffect);
        //Set anime target.
        m_settingPositionAnime->setTargetObject(m_preferencePanel);
        //Set down.
        m_preferencePanel->lower();
    }
}

void KNMainWindowContainer::showPreference()
{
    //Stop all the animations.
    m_preferenceAnimeGroup->stop();
    //Set final value.
    refreshShowEndValue();
    //Set start value.
    refreshAnimeStartValue();
    //Start show animation
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::hidePreference()
{
    //Stop all the animations.
    m_preferenceAnimeGroup->stop();
    //Set final value.
    refreshHideEndValue();
    //Set start value.
    refreshAnimeStartValue();
    //Start show animation
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Check is the animation still playing.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        //Check is anime showing preference panel, the way is check the end
        //value of m_settingOpacityAnime.
        if(m_settingOpacityAnime->endValue().toReal()==1.0)
        {
            //Update the show animation.
            refreshShowEndValue();
        }
        else
        {
            //Update the hide animation.
            refreshHideEndValue();
        }
        return;
    }
    //Check the switcher widget.
    if(m_categorySwitcher!=nullptr)
    {
        m_categorySwitcher->resize(width(), height());
    }
    //Regeometry the widgets.
    if(m_header->y()!=0)
    {
        m_header->setGeometry(0,
                                    -m_header->height(),
                                    width(),
                                    m_header->height());
        m_categoryStack->setGeometry(0,
                                     height(),
                                     width(),
                                     height()-m_header->height());
        m_preferencePanel->setGeometry(0,
                                       0,
                                       width(),
                                       height());
    }
    else
    {
        refreshGeometry();
    }
}

void KNMainWindowContainer::refreshGeometry()
{
    int categoryWidgetY;
    if(m_header==nullptr)
    {
        categoryWidgetY=0;
    }
    else
    {
        categoryWidgetY=m_header->height();
        m_header->setGeometry(0,0,width(),categoryWidgetY);
    }
    if(m_categoryStack!=nullptr)
    {
        m_categoryStack->setGeometry(0,
                                     categoryWidgetY,
                                     width(),
                                     height()-categoryWidgetY);
    }
    if(m_preferencePanel!=nullptr)
    {
        int widthPrefer=width()>>3, heightPrefer=height()>>3;
        m_preferencePanel->setGeometry((width()-widthPrefer)>>1,
                                       (height()-heightPrefer)>>1,
                                       widthPrefer,
                                       heightPrefer);
    }
}

void KNMainWindowContainer::refreshAnimeStartValue()
{
    m_settingOpacityAnime->setStartValue(m_settingsEffect->opacity());
    m_settingPositionAnime->setStartValue(m_preferencePanel->geometry());
    m_headerPositionAnime->setStartValue(m_header->geometry());
    m_categoryPositionAnime->setStartValue(m_categoryStack->geometry());
}

void KNMainWindowContainer::refreshShowEndValue()
{
    m_settingOpacityAnime->setEndValue(1.0);
    m_settingPositionAnime->setEndValue(rect());
    m_headerPositionAnime->setEndValue(QRect(0,
                                             -m_header->height(),
                                             width(),
                                             m_header->height()));
    m_categoryPositionAnime->setEndValue(QRect(0,
                                               height(),
                                               width(),
                                               height()-m_header->height()));
}

void KNMainWindowContainer::refreshHideEndValue()
{
    m_settingOpacityAnime->setEndValue(0.0);
    int widthPrefer=width()>>3, heightPrefer=height()>>3;
    m_settingPositionAnime->setEndValue(QRect((width()-widthPrefer)>>1,
                                              (height()-heightPrefer)>>1,
                                              widthPrefer,
                                              heightPrefer));
    m_headerPositionAnime->setEndValue(QRect(0,
                                             0,
                                             width(),
                                             m_header->height()));
    m_categoryPositionAnime->setEndValue(QRect(0,
                                               m_header->height(),
                                               width(),
                                               height()-m_header->height()));
}

void KNMainWindowContainer::configureOpacityAnime(QPropertyAnimation *anime)
{
    anime->setPropertyName("opacity");
    anime->setEasingCurve(QEasingCurve::InOutCirc);
    m_preferenceAnimeGroup->addAnimation(anime);
}

void KNMainWindowContainer::configureGeometryAnime(QPropertyAnimation *anime)
{
    anime->setPropertyName("geometry");
    anime->setEasingCurve(QEasingCurve::InOutCirc);
    m_preferenceAnimeGroup->addAnimation(anime);
}
