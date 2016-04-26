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
#include <QTimer>
#include <QPainter>

#include "knwaitingwheel.h"

#define WaitingWheelSize 13

KNWaitingWheel::KNWaitingWheel(QWidget *parent) :
    QWidget(parent),
    m_tickTimer(new QTimer(this)),
    m_frameCounter(0)
{
    //Set properties.
    setFixedSize(WaitingWheelSize, WaitingWheelSize);
    //Initial the clock frame image.
    for(int i=0; i<8; ++i)
    {
        //Load the image.
        m_clockFrames[i]=QPixmap("://public/waiting/w" + QString::number(i) +
                                 ".png").scaled(WaitingWheelSize,
                                                WaitingWheelSize,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);
    }
    //Configure the tick timer.
    m_tickTimer->setInterval(33);
    connect(m_tickTimer, &QTimer::timeout, this, &KNWaitingWheel::tick);
}

void KNWaitingWheel::startTick()
{
    //Start the ticking timer.
    m_tickTimer->start();
}

void KNWaitingWheel::pauseTick()
{
    //Stop the ticking timer.
    m_tickTimer->stop();
}

void KNWaitingWheel::stopTick()
{
    //Stop the ticking.
    m_tickTimer->stop();
    //Set to the 0 frame.
    m_frameCounter=0;
    update();
}

void KNWaitingWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the image at the counter.
    painter.drawPixmap(0, 0, m_clockFrames[m_frameCounter]);
}

void KNWaitingWheel::tick()
{
    //Update the frame counter.
    m_frameCounter=(m_frameCounter+1)>7?0:m_frameCounter+1;
    //Update the image.
    update();
}
