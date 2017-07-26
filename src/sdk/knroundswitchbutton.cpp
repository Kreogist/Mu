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

#include "kndpimanager.h"

#include "knroundswitchbutton.h"

#define SwitcherHeight  20
#define SwitcherWidth   42
#define SwitcherSmall   1
#define SwitcherTrueX   (SwitcherWidth-SwitcherHeight)
#define SwitcherFalseX  0

KNRoundSwitchButton::KNRoundSwitchButton(QWidget *parent) :
    QAbstractButton(parent),
    m_backgroundBrush(QLinearGradient(QPointF(0, 0),
                                      knDpi->posF(0, SwitcherHeight))),
    m_buttonBrush(QLinearGradient(QPointF(0, 0),
                                  knDpi->posF(0, SwitcherHeight))),
    m_movingAnime(new QTimeLine(100, this)),
    m_buttonX(0)
{
    //Set properties.
    setCheckable(true);
    setChecked(false);
    setFixedSize(knDpi->size(SwitcherWidth, SwitcherHeight));
    //Connect toggle signal to animation start slot.
    connect(this, &KNRoundSwitchButton::toggled,
            this, &KNRoundSwitchButton::startAnime);
    //Initial the paint rect.
    m_backgroundRect.moveTo(knDpi->posF(SwitcherHeight>>1, 0));
    m_backgroundRect.arcTo(knDpi->rectF(0, 0,
                                        SwitcherHeight, SwitcherHeight),
                           90, 180);
    m_backgroundRect.lineTo(knDpi->posF(SwitcherWidth-SwitcherHeight,
                                        SwitcherHeight));
    m_backgroundRect.arcTo(knDpi->rectF(SwitcherWidth-SwitcherHeight, 0,
                                        SwitcherHeight, SwitcherHeight),
                           270, 180);
    m_backgroundRect.closeSubpath();
    //Initial the button border.
    m_buttonBorder.setWidth(2);
    //Configure the timeline.
    m_movingAnime->setUpdateInterval(16);
    connect(m_movingAnime, &QTimeLine::frameChanged,
            this, &KNRoundSwitchButton::onActionMove);
}

void KNRoundSwitchButton::setForceChecked(bool checked)
{
    //Block the state.
    blockSignals(true);
    //Set the check state.
    setChecked(checked);
    //Update the state.
    onActionMove(isChecked()?SwitcherTrueX:SwitcherFalseX);
    //Release the block.
    blockSignals(false);
}

void KNRoundSwitchButton::updatePalette(const QPalette &pal)
{
    //Update the color.
    m_buttonBorder.setColor(pal.color(QPalette::WindowText));
    //Update the background brush.
    m_backgroundBrush.setColorAt(0, pal.color(QPalette::Base));
    m_backgroundBrush.setColorAt(1, pal.color(QPalette::Window));
    //Update the button brush.
    m_buttonBrush.setColorAt(0, pal.color(QPalette::Button));
    m_buttonBrush.setColorAt(1, pal.color(QPalette::ButtonText));
    //Initial the button filled color.
    m_buttonFillColor=pal.color(QPalette::Text);
    m_buttonFillColor.setAlpha(0);
    //Update the widget.
    update();
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
    painter.drawEllipse(knDpi->rectF(m_buttonX + SwitcherSmall, SwitcherSmall,
                                     SwitcherHeight-(SwitcherSmall<<1),
                                     SwitcherHeight-(SwitcherSmall<<1)));
}

void KNRoundSwitchButton::onActionMove(int frame)
{
    //Update the button x.
    m_buttonX=frame;
    //Update the filled color data.
    m_buttonFillColor.setAlpha(m_buttonX*100.0/SwitcherTrueX);
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
                                   SwitcherTrueX:
                                   //Set end frame to 0 for not checked.
                                   SwitcherFalseX);
    //Start the animation.
    m_movingAnime->start();
}
