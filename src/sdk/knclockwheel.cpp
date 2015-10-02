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
#include <QTimer>
#include <QPainter>

#include "knclockwheel.h"

#include <QDebug>

QIcon KNClockWheel::m_clockFrames[24]={
    QIcon("://public/clock/b0.png"),
    QIcon("://public/clock/b1.png"),
    QIcon("://public/clock/b2.png"),
    QIcon("://public/clock/b3.png"),
    QIcon("://public/clock/b4.png"),
    QIcon("://public/clock/b5.png"),
    QIcon("://public/clock/b6.png"),
    QIcon("://public/clock/b7.png"),
    QIcon("://public/clock/b8.png"),
    QIcon("://public/clock/b9.png"),
    QIcon("://public/clock/b10.png"),
    QIcon("://public/clock/b11.png"),
    QIcon("://public/clock/b12.png"),
    QIcon("://public/clock/b13.png"),
    QIcon("://public/clock/b14.png"),
    QIcon("://public/clock/b15.png"),
    QIcon("://public/clock/b16.png"),
    QIcon("://public/clock/b17.png"),
    QIcon("://public/clock/b18.png"),
    QIcon("://public/clock/b19.png"),
    QIcon("://public/clock/b20.png"),
    QIcon("://public/clock/b21.png"),
    QIcon("://public/clock/b22.png"),
    QIcon("://public/clock/b23.png")};

KNClockWheel::KNClockWheel(QWidget *parent) :
    QWidget(parent),
    m_tickTimer(new QTimer(this)),
    m_imageX(0),
    m_imageY(0),
    m_imageSize(0),
    m_frameCounter(0)
{
    //Set properties.
    setFixedSize(36, 36);
    //Configure the tick timer.
    m_tickTimer->setInterval(20);
    connect(m_tickTimer, &QTimer::timeout, this, &KNClockWheel::tick);
}

void KNClockWheel::startTick()
{
    //Start the ticking timer.
    m_tickTimer->start();
}

void KNClockWheel::pauseTick()
{
    //Stop the ticking timer.
    m_tickTimer->stop();
}

void KNClockWheel::stopTick()
{
    //Stop the ticking.
    m_tickTimer->stop();
    //Set to the 0 frame.
    m_frameCounter=0;
    update();
}

void KNClockWheel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Get the prefer size of the icon.
    int preferSize=qMin(width(), height());
    //Get the actual size it can be, set as image size..
    m_imageSize=m_clockFrames[0].pixmap(preferSize).width();
    //Calculate the image x and y position.
    m_imageX=(width()-m_imageSize)>>1;
    m_imageY=(height()-m_imageSize)>>1;
    qDebug()<<m_imageX<<m_imageY<<m_imageSize<<preferSize;
}

void KNClockWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the render hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the image at the counter.
    painter.drawPixmap(m_imageX,
                       m_imageY,
                       m_clockFrames[m_frameCounter].pixmap(m_imageSize));
}

void KNClockWheel::tick()
{
    //Update the frame counter.
    m_frameCounter=(m_frameCounter+1)>23?0:m_frameCounter+1;
    //Update the image.
    update();
}

