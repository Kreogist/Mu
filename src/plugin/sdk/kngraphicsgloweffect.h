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
#ifndef KNGRAPHICSGLOWEFFECT_H
#define KNGRAPHICSGLOWEFFECT_H

#include <QGraphicsEffect>

class KNGraphicsGlowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit KNGraphicsGlowEffect(QObject *parent = 0);
    void setRadius(qreal radius);
    void setGlowColor(const QColor &color);
    QRectF boundingRectFor(const QRectF &sourceRect) const;

signals:

public slots:

protected:
    void draw(QPainter *painter);
    void sourceChanged(ChangeFlags flags);

private:
    qreal m_radius;
    QColor m_color;
    bool m_insideGlow=false;
};

#endif // KNGRAPHICSGLOWEFFECT_H
