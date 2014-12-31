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
#include <QPixmap>

#include "kngraphicsgloweffect.h"

QT_BEGIN_NAMESPACE
// in src/qtbase/src/widgets/effects/qpixmapfilter.cpp
extern Q_DECL_IMPORT void qt_blurImage(QImage &blurImage,
                                       qreal radius,
                                       bool quality,
                                       int transposed = 0);
QT_END_NAMESPACE

KNGraphicsGlowEffect::KNGraphicsGlowEffect(QObject *parent) :
    QGraphicsEffect(parent)
{
    //Initial the parameters.
    m_radius=5.0;
    m_color=QColor(255, 255, 255, 255);
}

void KNGraphicsGlowEffect::setRadius(qreal radius)
{
    //Set the glow radius and update effects.
    m_radius=radius;
    //Because of the change of radius, we need to update rect.
    updateBoundingRect();
    update();
}

void KNGraphicsGlowEffect::setGlowColor(const QColor &color)
{
    //Set the new color.
    m_color=color;
    //Only need to repaint the new color.
    update();
}

QRectF KNGraphicsGlowEffect::boundingRectFor(const QRectF &sourceRect) const
{
    return QRectF(sourceRect.left(),
                  sourceRect.top(),
                  sourceRect.width(),
                  sourceRect.height());
}

void KNGraphicsGlowEffect::draw(QPainter *painter)
{
    //If we disabled radius (radius is 0), then do nothing.
    if (m_radius==0)
    {
        drawSource(painter);
        return;
    }
    QPixmap source=sourcePixmap();
    QImage sourceBlured(source.size(), QImage::Format_ARGB32_Premultiplied);
    sourceBlured.fill(0);
    QPainter blurPainter(&sourceBlured);
    blurPainter.setRenderHints(QPainter::Antialiasing |
                               QPainter::TextAntialiasing |
                               QPainter::SmoothPixmapTransform, true);
    blurPainter.drawPixmap(0, 0, source);
    blurPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    blurPainter.fillRect(sourceBlured.rect(), m_color);
    blurPainter.end();
    QT_PREPEND_NAMESPACE(qt_blurImage(sourceBlured, m_radius, true));
    painter->drawImage(0, 0, sourceBlured);
    painter->drawImage(0, 0, sourceBlured);
    painter->drawPixmap(0, 0, source);
}

void KNGraphicsGlowEffect::sourceChanged(QGraphicsEffect::ChangeFlags flags)
{
    Q_UNUSED(flags)
    updateBoundingRect();
    update();
}
