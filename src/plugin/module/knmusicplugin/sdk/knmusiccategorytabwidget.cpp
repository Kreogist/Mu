/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPropertyAnimation>

#include "knmusiccategorytabwidget.h"

KNMusicCategoryTabWidget::KNMusicCategoryTabWidget(QWidget *parent) :
    KNCategoryTabWidget(parent)
{
    //Initial the animations.
    m_mainPlayerIn=new QPropertyAnimation(this);
    configureAnimation(m_mainPlayerIn);
    m_mainPlayerOut=new QPropertyAnimation(this);
    configureAnimation(m_mainPlayerOut);
}

KNMusicCategoryTabWidget::~KNMusicCategoryTabWidget()
{
    ;
}

void KNMusicCategoryTabWidget::resizeEvent(QResizeEvent *event)
{
    //Do the resize first.
    KNCategoryTabWidget::resizeEvent(event);
    //Resize the player and set the y().
    if(m_mainPlayer!=nullptr)
    {
        m_mainPlayer->setGeometry(QRect(0,
                                        m_mainPlayer->y()==0?0:-height(),
                                        width(),
                                        height()));
    }
    //When the hide animation is running, change the finished position.
    if(m_mainPlayerOut->state()==QAbstractAnimation::Running)
    {
        m_mainPlayerOut->setEndValue(QRect(0,-height(),width(),height()));
    }
}

inline void KNMusicCategoryTabWidget::configureAnimation(
        QPropertyAnimation *anime)
{
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setPropertyName("geometry");
}

QWidget *KNMusicCategoryTabWidget::mainPlayer() const
{
    return m_mainPlayer;
}

void KNMusicCategoryTabWidget::setMainPlayer(QWidget *mainPlayer)
{
    //Save the player.
    m_mainPlayer = mainPlayer;
    //Change the parent relationship.
    m_mainPlayer->setParent(this);
    //Set the default position of the main player.
    m_mainPlayer->setGeometry(0,
                              -1,
                              width(),
                              height());
    //Set the animation target.
    m_mainPlayerIn->setTargetObject(m_mainPlayer);
    m_mainPlayerOut->setTargetObject(m_mainPlayer);
    //Linked the hide to main player out animation.
    connect(m_mainPlayerOut, &QPropertyAnimation::finished,
            m_mainPlayer, &QWidget::hide);
}

void KNMusicCategoryTabWidget::showMainPlayer()
{
    if(m_mainPlayer!=nullptr)
    {
        //Stop all animation.
        m_mainPlayerIn->stop();
        m_mainPlayerOut->stop();
        //Set properties.
        m_mainPlayerIn->setStartValue(m_mainPlayer->geometry());
        m_mainPlayerIn->setEndValue(QRect(0,0,width(),height()));
        //Show the main player.
        m_mainPlayer->show();
        //Start animation.
        m_mainPlayerIn->start();
    }
}

void KNMusicCategoryTabWidget::hideMainPlayer()
{
    if(m_mainPlayer!=nullptr)
    {
        //Stop all animation.
        m_mainPlayerIn->stop();
        m_mainPlayerOut->stop();
        //Set properties.
        m_mainPlayerOut->setStartValue(m_mainPlayer->geometry());
        m_mainPlayerOut->setEndValue(QRect(0,-height(),width(),height()));
        //Start animation.
        m_mainPlayerOut->start();
    }
}
