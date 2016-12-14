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

#include "knmusicalbumlabel.h"

#include <QDebug>

#define SpaceParamter 10

KNMusicAlbumLabel::KNMusicAlbumLabel(QWidget *parent) :
    QWidget(parent)
{
}

QPixmap KNMusicAlbumLabel::pixmap() const
{
    return m_originalArtwork;
}

void KNMusicAlbumLabel::setPixmap(const QPixmap &artwork)
{
    //Save the original artwork.
    m_originalArtwork=artwork;
    //Clear the scaled artwork.
    m_artwork=QPixmap();
    //Update the artwork.
    updateArtwork();
    //Update the label.
    update();
}

void KNMusicAlbumLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Set the rendering hints.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Paint the shape with the translate.
    if(!m_artwork.isNull())
    {
        painter.drawPixmap(0 , 0, m_artwork);
    }
}

void KNMusicAlbumLabel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Update the transform.
    //Initial the polygon shapes.
    QPolygonF original, target;
    //Get the original rect.
    original.append(QPointF(0,0));
    original.append(QPointF(width(), 0));
    original.append(QPointF(width(), height()));
    original.append(QPointF(0, height()));
    //Get the target shape.
    target.append(QPointF(0,0));
    target.append(QPointF(width()-SpaceParamter, SpaceParamter));
    target.append(QPointF(width()-SpaceParamter, height()-SpaceParamter));
    target.append(QPointF(0, height()));
    //Generate the transform.
    QTransform::quadToQuad(original, target, m_transform);

    //Update the artwork.
    updateArtwork();
}

inline void KNMusicAlbumLabel::updateArtwork()
{
    //Check original artwork.
    if(m_originalArtwork.isNull())
    {
        //No need to operate.
        return;
    }
    //Initial a whole size of pixmap, scaled the artwork.
    QPixmap originalArtwork(size()),
            scaledArtwork(m_originalArtwork.scaled(size(),
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
    //Fill up the pixmap with transparent pixels.
    originalArtwork.fill(QColor(0, 0, 0, 0));
    //Paint the scaled data on the original artwork.
    QPainter rawArtworkPainter(&originalArtwork);
    //Paint the data on the raw artwork.
    rawArtworkPainter.setRenderHints(QPainter::Antialiasing |
                                     QPainter::SmoothPixmapTransform, true);
    //Paint the pixmap;
    rawArtworkPainter.drawPixmap((width()-scaledArtwork.width())>>1,
                                 (height()-scaledArtwork.height())>>1,
                                 scaledArtwork);
    //Save the artwork with translation.
    m_artwork=originalArtwork.transformed(m_transform,
                                          Qt::SmoothTransformation);
}

