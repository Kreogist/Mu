/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QResizeEvent>

#include "knwidgetswitcher.h"

#include <QDebug>

KNWidgetSwitcher::KNWidgetSwitcher(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Initial the animation group.
    m_movingAnimationGroup=new QParallelAnimationGroup(this);

    //Intial the moving in animation.
    m_inAnimation=new QPropertyAnimation(this);
    configureAnimation(m_inAnimation);
    m_outAnimation=new QPropertyAnimation(this);
    configureAnimation(m_outAnimation);

    //Add animations to group.
    m_movingAnimationGroup->addAnimation(m_inAnimation);
    m_movingAnimationGroup->addAnimation(m_outAnimation);
    //Add animation complete actions.
    connect(m_movingAnimationGroup, &QParallelAnimationGroup::finished,
            this, &KNWidgetSwitcher::hideMoveOutWidget);
}

void KNWidgetSwitcher::addWidget(QWidget *widget)
{
    //Check the widget
    Q_ASSERT(widget!=NULL);
    //Configure the widget.
    widget->setParent(this);
    //If this is the first widget, initial the current index and make it visible,
    //Or else hide the widget.
    if(m_widgets.isEmpty())
    {
        m_currentIndex=0;
        widget->setGeometry(0,0,width(),height());
        widget->setVisible(true);
        widget->setEnabled(true);
    }
    else
    {
        widget->setVisible(false);
        widget->setEnabled(false);
    }
    //Add the widget to widegt list.
    m_widgets.append(widget);
}

int KNWidgetSwitcher::count() const
{
    //Return the widgets count.
    return m_widgets.size();
}

int KNWidgetSwitcher::currentIndex() const
{
    return m_currentIndex;
}

QWidget *KNWidgetSwitcher::widgetAt(const int &index)
{
    return m_widgets.at(index);
}

QWidget *KNWidgetSwitcher::currentWidget() const
{
    return m_widgets.at(m_currentIndex);
}

void KNWidgetSwitcher::setCurrentIndex(int currentIndex)
{
    //Ensure the current index is in the range.
    Q_ASSERT(currentIndex>-1 && currentIndex<m_widgets.size());
    //If current index is the right index or is moving widgets, do nothing.
    if(currentIndex==m_currentIndex ||
            m_movingAnimationGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }
    //Reset animation group.
    m_movingAnimationGroup->stop();
    //Backup the out widget index and set new index.
    m_outWidgetIndex=m_currentIndex;
    m_currentIndex=currentIndex;
    //Set in and out widgets.
    m_outAnimation->setTargetObject(m_widgets.at(m_outWidgetIndex));
    m_inAnimation->setTargetObject(m_widgets.at(m_currentIndex));
    //Update the start and end value.
    updateAnimationPosition();
    //Prepare the widget.
    setWidgetVisible(m_currentIndex, true);
    m_widgets.at(currentIndex)->setFocus();
    //Start animation.
    m_movingAnimationGroup->start();
}

void KNWidgetSwitcher::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(m_currentIndex!=-1)
    {
        m_widgets.at(m_currentIndex)->resize(size());
    }
    if(m_movingAnimationGroup->state()==QPropertyAnimation::Running)
    {
        m_widgets.at(m_outWidgetIndex)->resize(size());
        updateAnimationPosition();
    }
}

void KNWidgetSwitcher::hideMoveOutWidget()
{
    //Hide the out widget, reset out widget index.
    setWidgetVisible(m_outWidgetIndex, false);
    m_outWidgetIndex=-1;
    emit movedComplete();
}

void KNWidgetSwitcher::configureAnimation(QPropertyAnimation *animation)
{
    //Set the animation properties.
    animation->setPropertyName("geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(200);
}

void KNWidgetSwitcher::setWidgetVisible(const int &index,
                                         const bool &visible)
{
    Q_ASSERT(index>-1 && index<m_widgets.size());
    QWidget *widget=m_widgets.at(index);
    widget->setEnabled(visible);
    widget->setVisible(visible);
}
