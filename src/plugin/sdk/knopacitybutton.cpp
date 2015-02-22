/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QMouseEvent>
#include <QSizePolicy>
#include <QPainter>

#include "knopacitybutton.h"

KNOpacityButton::KNOpacityButton(QWidget *parent) :
    KNAbstractButton(parent)
{
    //Set as strong focus.
    setFocusPolicy(Qt::StrongFocus);
    //Set toolbutton size policy.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::ToolButton));
}

QPixmap KNOpacityButton::icon() const
{
    return m_icon;
}

void KNOpacityButton::setIcon(const QPixmap &icon)
{
    //Save the icon, update the scaled icon.
    m_icon=icon;
    updateScaledIcon();
    //Redraw the widget.
    update();
}

void KNOpacityButton::mousePressEvent(QMouseEvent *event)
{
    KNAbstractButton::mousePressEvent(event);
    //Set flag.
    m_pressed=true;
    //Set opacity.
    m_opacity=getPressedOpacity();
    //Update the widget.
    update();
}

void KNOpacityButton::mouseReleaseEvent(QMouseEvent *event)
{
    KNAbstractButton::mouseReleaseEvent(event);
    //Recover opacity.
    m_opacity=m_originalOpacity;
    update();
    //Judge is pressed?
    if(m_pressed)
    {
        m_pressed=false;
        //Judge is mouse in.
        if(rect().contains(event->pos()))
        {
            emit clicked();
        }
    }
}

void KNOpacityButton::paintEvent(QPaintEvent *event)
{
    if(m_scaledIcon.isNull())
    {
        return;
    }
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing, true);
    //Set opacity.
    painter.setOpacity(m_opacity);
    //Paint the contents.
    painter.drawPixmap(0,0,width(),height(),m_scaledIcon);
    //Paint other things.
    KNAbstractButton::paintEvent(event);
}

void KNOpacityButton::updateScaledIcon()
{
    if(m_icon.isNull())
    {
        m_scaledIcon=QPixmap();
        return;
    }
    //Scaled the icon.
    m_scaledIcon=m_icon.scaled(size(),
                               Qt::KeepAspectRatio,
                               Qt::SmoothTransformation);
}

inline qreal KNOpacityButton::getPressedOpacity()
{
    //Set opacity.
    qreal opacity=m_originalOpacity-m_opacityGap;
    //If the opacity is less than 0, means this settings wants to increase the
    //opacity.
    return (opacity<0)?m_originalOpacity+m_opacityGap:opacity;
}

qreal KNOpacityButton::opacity() const
{
    return m_opacity;
}

void KNOpacityButton::setOpacity(const qreal &opacity)
{
    //Save the opacity.
    m_originalOpacity=opacity;
    //Calculate the opacity.
    m_opacity=m_pressed?getPressedOpacity():m_originalOpacity;
    //Update the widget.
    update();
}

void KNOpacityButton::resizeEvent(QResizeEvent *event)
{
    //Do the resize first.
    KNAbstractButton::resizeEvent(event);
    //Update the icon.
    updateScaledIcon();
}

qreal KNOpacityButton::opacityGap() const
{
    return m_opacityGap;
}

void KNOpacityButton::setOpacityGap(const qreal &pressedOpacity)
{
    m_opacityGap=pressedOpacity;
}

