/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QTimeLine>

#include "knopacityanimebutton.h"

KNOpacityAnimeButton::KNOpacityAnimeButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    //Initial the animations.
    m_mouseDown=generateTimeLine(1000);
    m_mouseIn=generateTimeLine(m_inoutOpacity);
    m_mouseUp=generateTimeLine(m_inoutOpacity);
    m_mouseOut=generateTimeLine(m_startOpacity);
}

void KNOpacityAnimeButton::enterEvent(QEvent *event)
{
    KNAbstractButton::enterEvent(event);
    //Stop and start the mouse in animation.
    stopAllAnimations();
    startAnimation(m_mouseIn);
}

void KNOpacityAnimeButton::leaveEvent(QEvent *event)
{
    KNAbstractButton::leaveEvent(event);
    //Stop and start the mouse out animation.
    stopAllAnimations();
    startAnimation(m_mouseOut);
}

void KNOpacityAnimeButton::resizeEvent(QResizeEvent *event)
{
    KNAbstractButton::resizeEvent(event);
    //Update the icon.
    updateScaledIcon();
}

void KNOpacityAnimeButton::focusInEvent(QFocusEvent *event)
{
    KNAbstractButton::focusInEvent(event);
    //Stop and start the mouse in animation.
    stopAllAnimations();
    startAnimation(m_mouseIn);
}

void KNOpacityAnimeButton::focusOutEvent(QFocusEvent *event)
{
    KNAbstractButton::focusOutEvent(event);
    //Stop and start the mouse out animation.
    stopAllAnimations();
    startAnimation(m_mouseOut);
}

void KNOpacityAnimeButton::mousePressEvent(QMouseEvent *event)
{
    //Stop in out animations.
    stopAllAnimations();
    if(event->button()==Qt::LeftButton)
    {
        //Set mouse down flags.
        m_isPressed=true;
        //Set mouse down parameters.
        startAnimation(m_mouseDown);
    }
    KNAbstractButton::mousePressEvent(event);
}

void KNOpacityAnimeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Stop in out animations.
    stopAllAnimations();
    //Ensure this button has been mouse down.
    if(m_isPressed &&
            event->button()==Qt::LeftButton &&
            rect().contains(event->pos()))
    {
        //Accept event.
        event->accept();
        //Start mouse up animation.
        startAnimation(m_mouseUp);
        //Emit clicked signals.
        emit clicked();
    }
    m_isPressed=false;
    KNAbstractButton::mouseReleaseEvent(event);
}

void KNOpacityAnimeButton::onActionOpacityChanged(const int &opacity)
{
    //Save the opacity.
    m_currentOpacity=opacity;
    //Update the opacity.
    m_opacity=opacity/1000.0;
    //Update the widget.
    update();
}

inline QTimeLine *KNOpacityAnimeButton::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(100, this);
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(5);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNOpacityAnimeButton::onActionOpacityChanged);
    return timeLine;
}

inline void KNOpacityAnimeButton::updateScaledIcon()
{
    m_scaledIcon=m_originalIcon.scaled(size(),
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
}

inline void KNOpacityAnimeButton::stopAllAnimations()
{
    m_mouseDown->stop();
    m_mouseUp->stop();
    m_mouseIn->stop();
    m_mouseOut->stop();
}

void KNOpacityAnimeButton::startAnimation(QTimeLine *timeline)
{
    timeline->setStartFrame(m_currentOpacity);
    timeline->start();
}

QPixmap KNOpacityAnimeButton::icon() const
{
    return m_originalIcon;
}

QSize KNOpacityAnimeButton::sizeHint() const
{
    return m_originalIcon.size();
}

void KNOpacityAnimeButton::setIcon(const QPixmap &icon)
{
    m_originalIcon=icon;
    //Update the scaled icon.
    updateScaledIcon();
    //Repaint the widget mannally.
    update();
}

void KNOpacityAnimeButton::paintEvent(QPaintEvent *event)
{
    KNAbstractButton::paintEvent(event);
    if(m_scaledIcon.isNull())
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    //Set painter render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw pixmap in the specific opacity.
    painter.setOpacity(m_opacity);
    painter.drawPixmap(0, 0, m_scaledIcon);
}
