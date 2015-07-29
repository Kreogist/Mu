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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPropertyAnimation>
#include <QPainter>

#include "knanimationmenu.h"

KNAnimationMenu::KNAnimationMenu(QWidget *parent) :
    QMenu(parent),
    m_showContent(false),
    m_showAnime(new QPropertyAnimation(this, "geometry", this)),
    m_mouseDownPos(QPoint(0,0))
{
    //Set properties.
#ifndef Q_OS_MAC
    setWindowOpacity(0.85);
#endif
    //Configure the animation.
    m_showAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_showAnime->setDuration(200);
    //When the show anime is finished, show the content.
    connect(m_showAnime, &QPropertyAnimation::finished,
            [=]
            {
                //Set the show content flag.
                m_showContent=true;
                //Update the menu.
                update();
            });
}

void KNAnimationMenu::showEvent(QShowEvent *event)
{
    //Hide the content.
    m_showContent=false;
    //Configure the animation.
    //Prepare the position data.
    //Begin the height at 1px, the only reason is I don't want to see the '
    //invalid window content view size' warning under Mac OS X.
    QRect endPosition=geometry(),
          startPosition=QRect(endPosition.x(),
                              m_mouseDownPos.y(),
                              1,
                              1);
    //Set the parameter to animation.
    m_showAnime->setStartValue(startPosition);
    m_showAnime->setEndValue(endPosition);
    //Start animation.
    m_showAnime->start();
    //Do orignal show event.
    QMenu::showEvent(event);
}

void KNAnimationMenu::paintEvent(QPaintEvent *event)
{
    //Only do the original paint event when show content data is true.
    if(m_showContent)
    {
        QMenu::paintEvent(event);
    }
    //Initial a painter.
    QPainter painter(this);
    //Draw a border around the menu.
    painter.setPen(QColor(255,255,255,50));
    painter.drawRect(0, 0, width()-1, height()-1);
}

void KNAnimationMenu::setMouseDownPos(const QPoint &mouseDownPos)
{
    m_mouseDownPos = mouseDownPos;
}

void KNAnimationMenu::setSeparatorColor(const QColor &color)
{
    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba("+
                  QString::number(color.red())+
                  ", "+
                  QString::number(color.green())+
                  ", "+
                  QString::number(color.blue())+
                  ", "+
                  QString::number(color.alpha())+
                  ");"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");
}
