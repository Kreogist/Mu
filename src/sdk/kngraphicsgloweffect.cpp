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

#include "kngraphicsgloweffect.h"

// in src/qtbase/src/widgets/effects/qpixmapfilter.cpp
extern Q_DECL_IMPORT void qt_blurImage(QImage &blurImage,
                                       qreal radius,
                                       bool quality,
                                       int transposed = 0);

KNGraphicsGlowEffect::KNGraphicsGlowEffect(QObject *parent) :
    QGraphicsEffect(parent),
    m_radius(0.0),
    m_color(QColor(255,255,255,255))
{
}

qreal KNGraphicsGlowEffect::radius() const
{
    return m_radius;
}

void KNGraphicsGlowEffect::setRadius(const qreal &radius)
{
    //Save the radius size.
    m_radius = radius;
    //Update the graphics effect.
    update();
}

QColor KNGraphicsGlowEffect::color() const
{
    return m_color;
}

void KNGraphicsGlowEffect::setColor(const QColor &color)
{
    //Save the glow color.
    m_color = color;
    //Update the graphics effect.
    update();
}

QRectF KNGraphicsGlowEffect::boundingRectFor(const QRectF &sourceRect) const
{
    //The same source rect.
    return QRectF(sourceRect.x(),
                  sourceRect.y(),
                  sourceRect.width()+m_radius*2,
                  sourceRect.height()+m_radius*2);
}

void KNGraphicsGlowEffect::draw(QPainter *painter)
{
    //Check the radius before, the radius which is less than 0.001 treat as 0.0.
    //We will only draw the source content.
    if(m_radius<0.001)
    {
        //Draw original content.
        drawSource(painter);
        //Finish painting.
        return;
    }
    //Get the source pixmap.
    QPixmap source=sourcePixmap();
    //Generate the glow image.
    QImage glow(source.size(), QImage::Format_ARGB32_Premultiplied);
    //Clear the glow image data, set all the data to 0.
    glow.fill(0);
    //Generate the blured painter.
    QPainter bluredPainter(&glow);
    //Tweak the blured painter.
    bluredPainter.setRenderHints(QPainter::Antialiasing |
                                 QPainter::SmoothPixmapTransform, true);
    //Draw the source image first.
    bluredPainter.drawPixmap(QPointF(m_radius, m_radius), source);
    //Use CompositionMode_SourceIn mode to draw a m_color filled image data from
    //the source data.
    bluredPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    //Fill the glowing color.
    bluredPainter.fillRect(glow.rect(), m_color);
    //Magic, use qt_blurImage to blur the glow image, and that will make the
    //glow seems like glowing.
    QT_PREPEND_NAMESPACE(qt_blurImage(glow, m_radius, true));
    //Paint the glow first.
    painter->drawImage(QPointF(-m_radius, -m_radius), glow);
    //Paint the content.
    painter->drawPixmap(QPointF(0.0, 0.0), source);
}

void KNGraphicsGlowEffect::sourceChanged(QGraphicsEffect::ChangeFlags flags)
{
    Q_UNUSED(flags)
    //Update the bounding rect.
    updateBoundingRect();
    //Update the image.
    update();
}
