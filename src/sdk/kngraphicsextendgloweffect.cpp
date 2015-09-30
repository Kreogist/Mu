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

#include "kngraphicsextendgloweffect.h"

extern Q_DECL_IMPORT void qt_blurImage(QImage &blurImage, qreal radius,
        bool quality, int transposed = 0);

KNGraphicsExtendGlowEffect::KNGraphicsExtendGlowEffect(QObject *parent) :
    QGraphicsEffect(parent),
    m_radius(0),
    m_color(QColor(255, 255, 255, 255))
{

}

void KNGraphicsExtendGlowEffect::setRadius(int radius)
{
    m_radius = radius;
    updateBoundingRect();
    update();
}

void KNGraphicsExtendGlowEffect::setGlowColor(const QColor &color)
{
    m_color = color;
    update();
}

QRectF KNGraphicsExtendGlowEffect::boundingRectFor(const QRectF &sourceRect) const
{
    QRectF tmp(sourceRect);
    tmp.setBottomRight(tmp.bottomRight() +
                       QPointF(m_radius * 2, m_radius * 2));
    return tmp;
}

void KNGraphicsExtendGlowEffect::draw(QPainter *painter)
{
    if (!m_radius)
    {
        drawSource(painter);
        return;
    }
    QPixmap source = sourcePixmap();
    QImage sourceBlured(source.size() + QSize(m_radius * 2, m_radius * 2),
                        QImage::Format_ARGB32_Premultiplied);
    sourceBlured.fill(0);
    QPainter tmpPainter(&sourceBlured);
    tmpPainter.drawPixmap(m_radius, m_radius, source);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(sourceBlured.rect(), m_color);
    tmpPainter.end();
    qt_blurImage(sourceBlured, m_radius, true);
    painter->drawImage(0, 0, sourceBlured);
    painter->drawImage(0, 0, sourceBlured);
    painter->drawPixmap(m_radius, m_radius, source);
}

void KNGraphicsExtendGlowEffect::sourceChanged(QGraphicsEffect::ChangeFlags flags)
{
    updateBoundingRect();
    update();
}

