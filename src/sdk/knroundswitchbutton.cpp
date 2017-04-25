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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimeLine>
#include <QPainter>

#include "knroundswitchbutton.h"

#define SwitcherHeight  20
#define SwitcherWidth   42
#define SwitcherSmall   1

KNRoundSwitchButton::KNRoundSwitchButton(QWidget *parent) :
    QAbstractButton(parent),
    m_movingAnime(new QTimeLine(100, this)),
    m_buttonX(0)
{
    //Set properties.
    setCheckable(true);
    setChecked(false);
    setFixedSize(SwitcherWidth, SwitcherHeight);
    //Connect toggle signal to animation start slot.
    connect(this, &KNRoundSwitchButton::toggled,
            this, &KNRoundSwitchButton::startAnime);
    //Initial the paint rect.
    m_backgroundRect.moveTo(SwitcherHeight>>1, 0);
    m_backgroundRect.arcTo(0, 0,
                           SwitcherHeight, SwitcherHeight,
                           90, 180);
    m_backgroundRect.lineTo(SwitcherWidth-SwitcherHeight,
                            SwitcherHeight);
    m_backgroundRect.arcTo(SwitcherWidth-SwitcherHeight, 0,
                           SwitcherHeight, SwitcherHeight,
                           270, 180);
    m_backgroundRect.closeSubpath();
    //Initial the backgroud brush.
    m_backgroundBrush.setStart(0,0);
    m_backgroundBrush.setFinalStop(0, SwitcherHeight);
    m_backgroundBrush.setColorAt(0, QColor(0x2a, 0x2a, 0x2a));
    m_backgroundBrush.setColorAt(1, QColor(0x67, 0x67, 0x67));
    //Initial the button border.
    m_buttonBorder.setColor(QColor(0,0,0,100));
    m_buttonBorder.setWidth(2);
    //Initial the button brush.
    m_buttonBrush.setStart(0,0);
    m_buttonBrush.setFinalStop(0, SwitcherHeight);
    m_buttonBrush.setColorAt(0, QColor(0xe8,0xe8,0xe8));
    m_buttonBrush.setColorAt(1, QColor(0xcd,0xcd,0xcd));
    //Initial the button filled color.
    m_buttonFillColor.setRgb(0xF7, 0xCF, 0x3D, 0);
    //Configure the timeline.
    m_movingAnime->setUpdateInterval(16);
    connect(m_movingAnime, &QTimeLine::frameChanged,
            this, &KNRoundSwitchButton::onActionMove);
}

void KNRoundSwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the background.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_backgroundBrush);
    painter.drawPath(m_backgroundRect);
    //Paint the filled color.
    painter.setBrush(m_buttonFillColor);
    painter.drawPath(m_backgroundRect);
    //Paint the button.
    painter.setPen(m_buttonBorder);
    painter.setBrush(m_buttonBrush);
    painter.drawEllipse(m_buttonX + SwitcherSmall, SwitcherSmall,
                        SwitcherHeight-(SwitcherSmall<<1),
                        SwitcherHeight-(SwitcherSmall<<1));
}

void KNRoundSwitchButton::onActionMove(int frame)
{
    //Update the button x.
    m_buttonX=frame;
    //Update the filled color data.
    m_buttonFillColor.setAlpha(m_buttonX*100.0/(SwitcherWidth-SwitcherHeight));
    //Update the widget.
    update();
}

void KNRoundSwitchButton::startAnime()
{
    //Stop all animation.
    m_movingAnime->stop();
    //Set the start pos.
    m_movingAnime->setStartFrame(m_buttonX);
    //Check the running animation.
    m_movingAnime->setEndFrame(isChecked()?
                                   //When the button is at right, which is means
                                   //checked. Set the end frame to right.
                                   (SwitcherWidth-SwitcherHeight):
                                   //Set end frame to 0 for not checked.
                                   0);
    //Start the animation.
    m_movingAnime->start();
}
