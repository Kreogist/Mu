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

#include <QPainter>

#include "knhighlightlabel.h"

#include <QDebug>

KNHighlightLabel::KNHighlightLabel(QWidget *parent) :
    QLabel(parent)
{
    //Initial the highlight gradient.
    m_highlight=QLinearGradient(QPointF(0,0), QPointF(0, height()));
    m_highlight.setColorAt(0, QColor(0xff,0xff,0xff,130));
    m_highlight.setColorAt(1, QColor(0xff,0xff,0xff,0));

    //Initial the highlight area.
    m_highLightArea[0]=QPointF(0,0);
    updateHighlightArea();
}

void KNHighlightLabel::paintEvent(QPaintEvent *event)
{
    //Draw the label contents.
    QLabel::paintEvent(event);
    //Create a antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //Set pen and brush.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_highlight);
    //Draw the high light.
    painter.drawPolygon(m_highLightArea, 3);
}

void KNHighlightLabel::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QLabel::resizeEvent(event);
    //Update the highlight.
    updateHighlightArea();
}

void KNHighlightLabel::updateHighlightArea()
{
    //Calculate the light area depend on the size.
    m_highlight.setFinalStop(0, height());
    m_highLightArea[1]=QPointF((float)width()*0.7, 0);
    m_highLightArea[2]=QPointF(0, (float)height());
}
