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
    m_mainPlayerAnimeGroup(new QParallelAnimationGroup(this)),
    m_preferenceOpacity(generateAnime("opacity")),
    m_preferenceOpacityEffect(new QGraphicsOpacityEffect(this)),
    m_headerHeight(-1)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Reset the widgets pointer, initial the animation.
    QByteArray propertyList[ContainerAnimeCount];
    propertyList[AnimeHeader]="pos";
    propertyList[AnimeMainWidget]="pos";
    propertyList[AnimeMainPlayerHeader]="pos";
    propertyList[AnimeMainPlayer]="pos";
    propertyList[AnimePreferencePanel]="geometry";
    for(int i=0; i<ContainerWidgetCount; ++i)
    {
        //Reset the pointer.
        m_elementWidget[i]=nullptr;
    }

    //Initial the preference animations.
    for(int i=0; i<ContainerAnimeCount; ++i)
    {
        //Initial the animation.
        m_elementAnime[i]=generateAnime(propertyList[i]);
    }
    //Add animations to preference anime group.
    m_preferenceAnimeGroup->addAnimation(m_elementAnime[AnimeHeader]);
    m_preferenceAnimeGroup->addAnimation(m_elementAnime[AnimeMainWidget]);
    m_preferenceAnimeGroup->addAnimation(m_elementAnime[AnimePreferencePanel]);
    //Add opacity animation to anime group.
    m_preferenceAnimeGroup->addAnimation(m_preferenceOpacity);
    //Configure the opacity animation.
    m_preferenceOpacity->setTargetObject(m_preferenceOpacityEffect);
    //Configure the default effect.
    m_preferenceOpacityEffect->setOpacity(0.0);

    //Add animations to main player anime group.
    m_mainPlayerAnimeGroup->addAnimation(m_elementAnime[AnimeMainPlayer]);
    m_mainPlayerAnimeGroup->addAnimation(m_elementAnime[AnimeMainPlayerHeader]);
}

QWidget *KNMainWindowContainer::header() const
{
    return m_elementWidget[Header];
}

void KNMainWindowContainer::setHeader(QWidget *header)
{
    //Check the pointer.
    if(header==nullptr)
    {
        return;
    }
    //Save header widget.
    setWidget(Header, header);
    //Set the animation target.
    m_elementAnime[AnimeHeader]->setTargetObject(header);
    m_elementAnime[AnimeMainPlayerHeader]->setTargetObject(header);
    //Save the height of header widget.
    m_headerHeight=header->height();
}

QWidget *KNMainWindowContainer::mainWidget() const
{
    return m_elementWidget[MainWidget];
}

void KNMainWindowContainer::setMainWidget(QWidget *mainWidget)
{
    //Check the pointer.
    if(mainWidget==nullptr)
    {
        return;
    }
    //Save the main widget.
    setWidget(MainWidget, mainWidget);
    //Set the animation target.
    m_elementAnime[AnimeMainWidget]->setTargetObject(mainWidget);
    //Update the main widget geometry.
    mainWidget->setGeometry(0,
                            m_headerHeight,
                            width(),
                            height()-m_headerHeight);
}

QWidget *KNMainWindowContainer::preferencePanel() const
{
    return m_elementWidget[PreferencePanel];
}

void KNMainWindowContainer::setPreferencePanel(QWidget *preferencePanel)
{
    //Check the pointer.
    if(preferencePanel==nullptr)
    {
        return;
    }
    //Set the preference widget.
    setWidget(PreferencePanel, preferencePanel);
    //Set the animation target.
    m_elementAnime[AnimePreferencePanel]->setTargetObject(preferencePanel);
    //Set the opacity effect to the widget.
    preferencePanel->setGraphicsEffect(m_preferenceOpacityEffect);
}

QWidget *KNMainWindowContainer::mainPlayer() const
{
    return m_elementWidget[MainPlayer];
}

void KNMainWindowContainer::setMainPlayer(QWidget *mainPlayer)
{
    //Check the pointer.
    if(mainPlayer==nullptr)
    {
        return;
    }
    //Set the main player widget.
    setWidget(MainPlayer, mainPlayer);
    //Set the animation target.
    m_elementAnime[AnimeMainPlayer]->setTargetObject(mainPlayer);
    //Hide the main player as default.
    m_elementWidget[MainPlayer]->hide();
}

void KNMainWindowContainer::showPreference()
{
    //Ignore the show request if the animation is running.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }

    //Update the header animation properties.
    m_elementAnime[AnimeHeader]->setStartValue(QPoint(0,0));
    m_elementAnime[AnimeHeader]->setEndValue(QPoint(0, -m_headerHeight));
    //Update the main widget animation start value property.
    m_elementAnime[AnimeMainWidget]->setStartValue(QPoint(0, m_headerHeight));
    //Update the opacity animation properties.
    m_preferenceOpacity->setStartValue(0.0);
    m_preferenceOpacity->setEndValue(1.0);
    //Update the container size related parameters.
    updateShowPreference();

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
    m_elementAnime[AnimeHeader]->setStartValue(QPoint(0, -m_headerHeight));
    m_elementAnime[AnimeHeader]->setEndValue(QPoint(0,0));
    //Update the main widget animation end value.
    m_elementAnime[AnimeMainWidget]->setEndValue(QPoint(0, m_headerHeight));
    //Update the opacity animation properties.
    m_preferenceOpacity->setStartValue(1.0);
    m_preferenceOpacity->setEndValue(0.0);
    //Update the container size related parameters.
    updateHidePreference();

    //Start the animation.
    m_preferenceAnimeGroup->start();
}

void KNMainWindowContainer::showMainPlayer()
{
    //Ignore the show request if the animation is running.
    if(m_elementWidget[MainPlayer]==nullptr ||
            m_mainPlayerAnimeGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }

    //Update the main player animation end value.
    m_elementAnime[AnimeMainPlayer]->setEndValue(QPoint(0, 0));
    //Update the header animation end value.
    m_elementAnime[AnimeMainPlayerHeader]->setStartValue(QPoint(0, 0));
    //Update the container size related parameters.
    updateShowMainPlayer();

    //Show the main player.
    m_elementWidget[MainPlayer]->show();
    //Disconnect the animation finished signal.
    disconnect(m_mainPlayerAnimeGroup, &QParallelAnimationGroup::finished,
               m_elementWidget[MainPlayer], &QWidget::hide);
    connect(m_mainPlayerAnimeGroup, &QParallelAnimationGroup::finished,
            m_elementWidget[Header], &QWidget::hide);
    //Start the animation.
    m_mainPlayerAnimeGroup->start();
}

void KNMainWindowContainer::hideMainPlayer()
{
    //Ignore the show request if the animation is running.
    if(m_elementWidget[MainPlayer]==nullptr ||
            m_mainPlayerAnimeGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }

    //Update the main player animation end value.
    m_elementAnime[AnimeMainPlayer]->setStartValue(QPoint(0, 0));
    //Update the header animation end value.
    m_elementAnime[AnimeMainPlayerHeader]->setEndValue(QPoint(0, 0));
    //Update the container size related parameters.
    updateHideMainPlayer();

    //Show the header widget first.
    m_elementWidget[Header]->show();
    //Disconnect the header hide.
    disconnect(m_mainPlayerAnimeGroup, &QParallelAnimationGroup::finished,
               m_elementWidget[Header], &QWidget::hide);
    //Connect the animation finished signal to the hide slot of the main player.
    connect(m_mainPlayerAnimeGroup, &QParallelAnimationGroup::finished,
            m_elementWidget[MainPlayer], &QWidget::hide);
    //Start the animation.
    m_mainPlayerAnimeGroup->start();
}

void KNMainWindowContainer::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Initial the state flag for animations.
    bool preferenceRunning=false, mainPlayerRunning=false;
    //Check the state of the animation group.
    if(m_preferenceAnimeGroup->state()==QAbstractAnimation::Running)
    {
        //Set animation running flag.
        preferenceRunning=true;
        //Update the animation group parameters.
        //If the end value of the opacity animation is 1.0, means the animation
        //is showing the preference panel. Update the animation properies via
        //function updateShowPreference(), or else updateHidePreference().
        if(m_preferenceOpacity->endValue().toReal()==1.00)
        {
            updateShowPreference();
        }
        else
        {
            updateHidePreference();
        }
    }
    if(m_mainPlayerAnimeGroup->state()==QAbstractAnimation::Running)
    {
        //Set the animation running flag.
        mainPlayerRunning=true;
        //Updat the animation group parameters.
        //If the y of the end value of the header widget animation is 0, means
        //the animation is hiding the main player. Update the animation
        //properties via updateHideMainPlayer(), or else updateShowMainPlayer().
        if(m_elementAnime[AnimeMainPlayerHeader]->endValue().toPoint().y()==0)
        {
            updateHideMainPlayer();
        }
        else
        {
            updateShowMainPlayer();
        }
    }
    //Resize the header widget.
    if(m_elementWidget[Header]!=nullptr)
    {
        //Resize the header widget.
        m_elementWidget[Header]->resize(width(), m_headerHeight);
        //If the animation is not running, update the header position if the
        //header is at the bottom of the window.
        if((!mainPlayerRunning) &&
                m_elementWidget[Header]->y()>0)
        {
            m_elementWidget[Header]->move(0, height());
        }
    }
    //Resize the main player.
    if(m_elementWidget[MainPlayer]!=nullptr)
    {
        //Resize the main player.
        m_elementWidget[MainPlayer]->resize(width(), height());
    }
    //Resize the main widget and reposition the main widget if there's no
    //animation running.
    if(m_elementWidget[MainWidget]!=nullptr)
    {
        //Resize the main widget for no reason.
        m_elementWidget[MainWidget]->resize(width(), height()-m_headerHeight);
        //If the animation is not running, update the main widget position if
        //main widget is at the bottom of the window.
        if((!preferenceRunning) &&
                m_elementWidget[MainWidget]->y()!=m_headerHeight)
        {
            m_elementWidget[MainWidget]->move(0, height());
        }
    }
    //Resize the preference widget and resize the preference widget if it's
    //visible, and animation is not running.
    if(m_elementWidget[PreferencePanel]!=nullptr &&
            (!preferenceRunning) &&
            m_preferenceOpacityEffect->opacity()==1.0)
    {
        m_elementWidget[PreferencePanel]->resize(width(), height());
    }
}

inline void KNMainWindowContainer::updateTheStackRelationship()
{
    //The stuck position of those widget will be like this:
    /*
     *         +        +                  +        +
     *        /|       /|                 /|       /|
     *       / |      / |                / |      / |
     *      /  |     /  |               /  +     /  |
     *     /   |    /   |        +     /  /     /   |
     *    +    |   +    |       /|    +  /     +    |
     *    |    |   |    |      / |    | /      |    |
     *    |    |   |    |     /  |    |/       |    |
     *    |    |   |    |    /   |    |        |    |
     *    |    |   |    |   +    |    |        |    |
     *    |    +   |    +   |    +    |        |    +
     *    |   /    |   /    |   /     |        |   /
     *    |  /     |  /     |  /      |        |  /
     *    | /      | /      | /       |        | /
     *    |/       |/       |/        |        |/
     *    +        +        +         |        +
     *    |        |        |         |        |
     *    |        |        |         |        Main Player
     *    |        |        |         Header
     *    |        |        Main Widget
     *    |        Preference Panel
     *    Base(Main Window)
     */
    if(m_elementWidget[PreferencePanel]!=nullptr)
    {
        m_elementWidget[PreferencePanel]->lower();
    }
    if(m_elementWidget[Header]!=nullptr)
    {
        m_elementWidget[Header]->raise();
    }
    if(m_elementWidget[MainPlayer]!=nullptr)
    {
        m_elementWidget[MainPlayer]->raise();
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

inline void KNMainWindowContainer::updateShowPreference()
{
    //Update the preference animation properties.
    m_elementAnime[AnimePreferencePanel]->setStartValue(
                QRect(width()>>2, height()>>2, width()>>1, height()>>1));
    m_elementAnime[AnimePreferencePanel]->setEndValue(
                QRect(0,0,width(),height()));
    //Update the main widget animation end value property.
    m_elementAnime[AnimeMainWidget]->setEndValue(QPoint(0, height()));
}

inline void KNMainWindowContainer::updateHidePreference()
{
    //Update the preference animation properties.
    m_elementAnime[AnimePreferencePanel]->setStartValue(
                QRect(0,0,width(),height()));
    m_elementAnime[AnimePreferencePanel]->setEndValue(
                QRect(width()>>2, height()>>2, width()>>1, height()>>1));
    //Update the main widget animation start value property.
    m_elementAnime[AnimeMainWidget]->setStartValue(QPoint(0, height()));
}

inline void KNMainWindowContainer::updateShowMainPlayer()
{
    //Update the main player animation start value.
    m_elementAnime[AnimeMainPlayer]->setStartValue(QPoint(0, -height()));
    //Update the header end position.
    m_elementAnime[AnimeMainPlayerHeader]->setEndValue(QPoint(0, height()));
}

inline void KNMainWindowContainer::updateHideMainPlayer()
{
    //Update the main player animation end value.
    m_elementAnime[AnimeMainPlayer]->setEndValue(QPoint(0, -height()));
    //Update the header start position.
    m_elementAnime[AnimeMainPlayerHeader]->setStartValue(QPoint(0, height()));
}

inline QPropertyAnimation *KNMainWindowContainer::generateAnime(
        const QByteArray &propertyName)
{
    //Generate the animation.
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    //Set properties.
    anime->setEasingCurve(QEasingCurve::InOutCirc);
    anime->setPropertyName(propertyName);
    //Give back the anime.
    return anime;
}
