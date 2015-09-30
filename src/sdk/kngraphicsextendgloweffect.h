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

#ifndef KNGRAPHICSEXTENDGLOWEFFECT_H
#define KNGRAPHICSEXTENDGLOWEFFECT_H

#include <QGraphicsEffect>

class KNGraphicsExtendGlowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit KNGraphicsExtendGlowEffect(QObject *parent = 0);

    void setRadius(int m_radius);

    void setGlowColor(const QColor &color);

    QRectF boundingRectFor(const QRectF &sourceRect) const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    void draw(QPainter *painter) Q_DECL_OVERRIDE;
    void sourceChanged(ChangeFlags flags) Q_DECL_OVERRIDE;

private:
    int m_radius;
    QColor m_color;
};

#endif // KNGRAPHICSEXTENDGLOWEFFECT_H
