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

#include "knimagelabel.h"

KNImageLabel::KNImageLabel(QWidget *parent) :
    QWidget(parent),
    m_pixmap(QPixmap()),
    m_scaledPixmap(QPixmap()),
    m_pixmapX(0),
    m_pixmapY(0)
{
}

QPixmap KNImageLabel::pixmap() const
{
    return m_pixmap;
}

void KNImageLabel::setPixmap(const QPixmap &pixmap)
{
    //Save the raw pixmap.
    m_pixmap = pixmap;
    //Update the scaled pixmap.
    updateScaledPixmap();
}

void KNImageLabel::paintEvent(QPaintEvent *event)
{
    //Paint the widget.
    QWidget::paintEvent(event);
    //Check scaled pixmap first.
    if(!m_scaledPixmap.isNull())
    {
        //Initial the painter.
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform, true);
        //Draw the scaled pixmap.
        painter.drawPixmap(m_pixmapX, m_pixmapY, m_scaledPixmap);
    }
}

void KNImageLabel::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    QWidget::resizeEvent(event);
    //Update the scaled pixmap.
    updateScaledPixmap();
}

inline void KNImageLabel::updateScaledPixmap()
{
    //Resize the image.
    m_scaledPixmap=m_pixmap.scaled(size(),
                                   Qt::KeepAspectRatio,
                                   Qt::SmoothTransformation);
    //Update the position of pixmap.
    m_pixmapX=(width()-m_scaledPixmap.width())>>1;
    m_pixmapY=(height()-m_scaledPixmap.height())>>1;
}
