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

#include "knmusicdetailtooltipartwork.h"

KNMusicDetailTooltipArtwork::KNMusicDetailTooltipArtwork(QWidget *parent) :
    QWidget(parent)
{

}

QPixmap KNMusicDetailTooltipArtwork::artwork() const
{
    return m_artwork;
}

void KNMusicDetailTooltipArtwork::setArtwork(const QPixmap &artwork)
{
    m_artwork = artwork.scaled(size(),
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation).transformed(m_transform,
                                                                     Qt::SmoothTransformation);
    //Update the widget.
    update();
}

void KNMusicDetailTooltipArtwork::setArtworkSize(QSize size)
{
    //Resize the artwork first.
    setFixedSize(size);

    //Set the transform.
    //Initial the shapes.
    QPolygonF shape, original;
    original<<QPointF(0,0)
            <<QPointF(width(), 0)
            <<QPointF(width(), height())
            <<QPointF(0, height());
    shape<<QPointF(0,0)
         <<QPointF(width()-m_spaceParamter, m_spaceParamter)
         <<QPointF(width()-m_spaceParamter, height()-m_spaceParamter)
         <<QPointF(0, height());
    //Generate the transform.
    QTransform::quadToQuad(original, shape, m_transform);
}

void KNMusicDetailTooltipArtwork::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Paint the shape with the translate.
    if(!m_artwork.isNull())
    {
        painter.drawPixmap(0,0,m_artwork);
    }
}
