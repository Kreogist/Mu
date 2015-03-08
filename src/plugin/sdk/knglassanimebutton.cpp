/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>
#include <QLinearGradient>

#include "knglassanimebutton.h"

KNGlassAnimeButton::KNGlassAnimeButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set properties.
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));

    //Initial the line gradient.
    m_lineGradient.setStart(0,0);
    m_lineGradient.setColorAt(0, QColor(255,255,255,0));
    m_lineGradient.setColorAt(1, QColor(255,255,255,140));

    m_backGradient.setStart(0,0);
    m_backGradient.setColorAt(0, QColor(255,255,255,0));
    m_backGradient.setColorAt(1, QColor(255,255,255,0));

    //Initial the animations.
    m_mouseIn=generateTimeLine(50);
    m_mouseOut=generateTimeLine(0);
    m_mouseDown=generateTimeLine(25);
    m_mouseUp=generateTimeLine(50);
}

KNGlassAnimeButton::~KNGlassAnimeButton()
{

}

void KNGlassAnimeButton::enterEvent(QEvent *event)
{
    KNAbstractButton::enterEvent(event);
    //Stop and start the mouse in animation.
    stopAllAnimations();
    startAnimation(m_mouseIn);
}

void KNGlassAnimeButton::leaveEvent(QEvent *event)
{
    KNAbstractButton::leaveEvent(event);
    //Stop and start the mouse out animation.
    stopAllAnimations();
    startAnimation(m_mouseOut);
}

void KNGlassAnimeButton::paintEvent(QPaintEvent *event)
{
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backGradient);
    painter.drawRect(rect());
    painter.setBrush(m_lineGradient);
    painter.drawRect(width()-1,0,1,height());
    if(m_leftline)
    {
        painter.drawRect(0,0,1,height());
    }
    //Draw the icon.
    if(!m_scaledIcon.isNull())
    {
        painter.drawPixmap(rect(), m_scaledIcon);
    }
}

void KNGlassAnimeButton::resizeEvent(QResizeEvent *event)
{
    //Resize the button.
    KNAbstractButton::resizeEvent(event);
    //Update the line gradient.
    m_lineGradient.setFinalStop(0,height());
    m_backGradient.setFinalStop(0,height());
    //Update the icon.
    updateScaledIcon();
}

void KNGlassAnimeButton::focusInEvent(QFocusEvent *event)
{
    KNAbstractButton::focusInEvent(event);
    //Stop and start the mouse in animation.
    stopAllAnimations();
    startAnimation(m_mouseIn);
}

void KNGlassAnimeButton::focusOutEvent(QFocusEvent *event)
{
    KNAbstractButton::focusOutEvent(event);
    //Stop and start the mouse out animation.
    stopAllAnimations();
    startAnimation(m_mouseOut);
}

void KNGlassAnimeButton::mousePressEvent(QMouseEvent *event)
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

void KNGlassAnimeButton::mouseReleaseEvent(QMouseEvent *event)
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

void KNGlassAnimeButton::onActionHighlightChanged(const int &highlight)
{
    m_currentHighlight=highlight;
    //Change the graident color.
    m_backGradient.setColorAt(1, QColor(255,255,255,highlight));
    //Update the widget.
    update();
}

inline QTimeLine *KNGlassAnimeButton::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(100, this);
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(16);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNGlassAnimeButton::onActionHighlightChanged);
    return timeLine;
}

inline void KNGlassAnimeButton::updateScaledIcon()
{
    int iconSize=qMin(qMin(width(), height()),
                      qMin(m_originalIcon.width(), m_originalIcon.height()));
    m_scaledIcon=m_originalIcon.scaled(iconSize, iconSize,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
}

inline void KNGlassAnimeButton::stopAllAnimations()
{
    m_mouseDown->stop();
    m_mouseUp->stop();
    m_mouseIn->stop();
    m_mouseOut->stop();
}

void KNGlassAnimeButton::startAnimation(QTimeLine *timeline)
{
    timeline->setStartFrame(m_currentHighlight);
    timeline->start();
}

QPixmap KNGlassAnimeButton::icon() const
{
    return m_originalIcon;
}

QSize KNGlassAnimeButton::sizeHint() const
{
    return m_originalIcon.size();
}

void KNGlassAnimeButton::setIcon(const QPixmap &icon)
{
    m_originalIcon=icon;
    //Update the scaled icon.
    updateScaledIcon();
    //Repaint the widget mannally.
    update();
}

void KNGlassAnimeButton::setLeftLineVisible(const bool &visible)
{
    m_leftline=visible;
}
