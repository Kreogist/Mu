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
#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "knnotificationwidget.h"

KNNotificationWidget::KNNotificationWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedSize(270, 54);

    //Initial graphics effect.
    QGraphicsDropShadowEffect *effect=new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(20);
    effect->setOffset(0.0);
    effect->setColor(QColor(0,0,0));
    setGraphicsEffect(effect);
}

void KNNotificationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set color.
    painter.setOpacity(0.8);
    painter.setPen(QColor(100, 100, 100));
    painter.setBrush(Qt::white);
    //Draw the background.
    painter.drawRoundedRect(rect(), 10, 10);
    //Draw the content.
    painter.setOpacity(1.0);
    painter.drawText(rect(), Qt::AlignLeft | Qt::AlignTop, m_title);
}
