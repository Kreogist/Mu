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
#include <QPropertyAnimation>

#include "kncategorysettingbutton.h"

KNCategorySettingButton::KNCategorySettingButton(QWidget *parent) :
    KNAbstractCategoryButton(parent)
{
    //Initial the slide animation.
    m_slideIn=new QPropertyAnimation(this);
    initialGeometryAnime(m_slideIn);
    m_slideOut=new QPropertyAnimation(this);
    initialGeometryAnime(m_slideOut);
}

void KNCategorySettingButton::startMouseInAnime()
{
    ;
}

void KNCategorySettingButton::startMouseOutAnime()
{
    ;
}

void KNCategorySettingButton::startMouseDownAnime()
{
    ;
}

void KNCategorySettingButton::startMouseUpAnime()
{
    ;
}

void KNCategorySettingButton::startInAnime()
{
    //Stop all slide anime.
    stopSlideAnime();
    //Set parameters.
    m_slideIn->setStartValue(generateOut());
    m_slideIn->setEndValue(generateIn());
    //Start slide animation.
    m_slideIn->start();
}

void KNCategorySettingButton::startOutAnime()
{
    //Stop all slide anime.
    stopSlideAnime();
    //Set parameters.
    m_slideOut->setStartValue(generateIn());
    m_slideOut->setEndValue(generateOut());
    //Start slide animation.
    m_slideOut->start();
}

void KNCategorySettingButton::initialGeometryAnime(QPropertyAnimation *anime)
{
    anime->setPropertyName("geometry");
    anime->setEasingCurve(QEasingCurve::OutCubic);
    anime->setTargetObject(this);
}

void KNCategorySettingButton::stopSlideAnime()
{
    m_slideIn->stop();
    m_slideOut->stop();
}

QRect KNCategorySettingButton::generateOut()
{
    return QRect(0, -height(), width(), height());
}

QRect KNCategorySettingButton::generateIn()
{
    return QRect(0, 0, width(), height());
}
