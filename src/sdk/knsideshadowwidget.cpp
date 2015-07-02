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
#include <QPainter>

#include "knsideshadowwidget.h"

KNSideShadowWidget::KNSideShadowWidget(int direction,
                                       QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    //Initial the shadow gradient.
    m_shadowGradient.setColorAt(0, QColor(0,0,0,70));
    m_shadowGradient.setColorAt(1, QColor(0,0,0,0));
    //Set the direction.
    setDirection(direction);
}

int KNSideShadowWidget::direction() const
{
    return m_direction;
}

void KNSideShadowWidget::setDirection(int direction)
{
    //Ensure the direction is valid.
    if(direction>-1 && direction<4)
    {
        //Save the direction.
        m_direction = direction;
        //Update the gradient.
        updateGradient();
        //Repaint the widget.
        update();
    }
}

void KNSideShadowWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set gradient.
    painter.fillRect(QRect(-1, -1, width()+2, height()+2),
                     m_shadowGradient);
}

void KNSideShadowWidget::resizeEvent(QResizeEvent *event)
{
    //Resize the widget first.
    QWidget::resizeEvent(event);
    //Update the gradient.
    updateGradient();
}

void KNSideShadowWidget::updateGradient()
{
    //Generate the start and stop position.
    switch(m_direction)
    {
    case TopShadow:
        //Shadow should be from top to bottom.
        m_shadowGradient.setStart(0,0);
        m_shadowGradient.setFinalStop(0,height());
        break;
    case BottomShadow:
        //Shadow should be from bottom to top.
        m_shadowGradient.setStart(0,height());
        m_shadowGradient.setFinalStop(0,0);
        break;
    case LeftShadow:
        //Shadow should be from left to right.
        m_shadowGradient.setStart(0,0);
        m_shadowGradient.setFinalStop(width(),0);
        break;
    case RightShadow:
        //Shadow should be from right to left.
        m_shadowGradient.setStart(width(),0);
        m_shadowGradient.setFinalStop(0,0);
        break;
    }
}


