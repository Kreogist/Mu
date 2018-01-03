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
#include <QPainter>

#include "knsaooptionbutton.h"

KNSaoOptionButton::KNSaoOptionButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseEnter(false)
{
    //Set properties.
    setCheckable(true);
    setFixedHeight(fontMetrics().height()<<1);
}

void KNSaoOptionButton::enterEvent(QEvent *event)
{
    //Do orignal event.
    QAbstractButton::enterEvent(event);
    //Update the button.
    m_mouseEnter=true;
    update();
}

void KNSaoOptionButton::leaveEvent(QEvent *event)
{
    //Do orignal leave event.
    QAbstractButton::leaveEvent(event);
    //Update the button.
    m_mouseEnter=false;
    update();
}

void KNSaoOptionButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Draw the button content.
    QPainter painter(this);
    //Fill the white background first.
    painter.fillRect(rect(), QColor(255, 255, 255, 200));
    if(isChecked())
    {
        //Paint the background.
        painter.fillRect(rect(), QColor(0xf7, 0xcf, 0x3d));
    }
    else if(m_mouseEnter)
    {
        //Paint the half opacity background.
        painter.fillRect(rect(), QColor(0xf7, 0xcf, 0x3d, 127));
    }
    //Draw the text.
    painter.drawText(rect(), Qt::AlignCenter, text());
}
