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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QTimer>
#include <QPainter>

#include "knclockwheel.h"

KNClockWheel::KNClockWheel(QWidget *parent) :
    QWidget(parent)
{
    //Initial the clock image frames.
    for(int i=0; i<24; i++)
    {
        m_clockFrames[i]=QPixmap("://public/clock/b"+QString::number(i)+".png");
    }
    //Initial the tick timer.
    m_tickTimer=new QTimer(this);
    m_tickTimer->setInterval(30);
    connect(m_tickTimer, &QTimer::timeout, this, &KNClockWheel::tick);
    //Initial the default width and height.
    m_imageSize=m_clockFrames[0].width();
    m_scaledSize=m_imageSize;
    setFixedSize(m_imageSize, m_imageSize);
}

KNClockWheel::~KNClockWheel()
{

}

void KNClockWheel::setWheelSize(int width, int height)
{
    //Save the image width.
    m_scaledSize=qMin(width, height);
    //Check the size.
    if(m_scaledSize>m_imageSize)
    {
        //Update image position.
        m_imageX=(width-m_imageSize)>>1;
        m_imageY=(height-m_imageSize)>>1;
        //Reset the scaled size.
        m_scaledSize=m_imageSize;
    }
    //Resize the widget.
    setFixedSize(width, height);
    //Update the widget.
    update();
}

void KNClockWheel::startTick()
{
    m_tickTimer->start();
}

void KNClockWheel::pauseTick()
{
    m_tickTimer->stop();
}

void KNClockWheel::stopTick()
{
    m_tickTimer->stop();
    //Set to the 0 frame.
    m_frameCounter=0;
    update();
}

void KNClockWheel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the image.
    painter.drawPixmap(m_imageX,
                       m_imageY,
                       m_scaledSize,
                       m_scaledSize,
                       m_clockFrames[m_frameCounter].scaled(m_scaledSize, m_scaledSize,
                                                            Qt::IgnoreAspectRatio,
                                                            Qt::SmoothTransformation));
}

void KNClockWheel::tick()
{
    //Update the frame counter.
    m_frameCounter=(m_frameCounter+1)%24;
    //Update the image.
    update();
}

