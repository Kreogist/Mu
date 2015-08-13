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

#ifndef KNGRAPHICSGLOWEFFECT_H
#define KNGRAPHICSGLOWEFFECT_H

#include <QGraphicsEffect>

class KNGraphicsGlowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    explicit KNGraphicsGlowEffect(QObject *parent = 0);

    qreal radius() const;
    void setRadius(const qreal &radius);

    QColor color() const;
    void setColor(const QColor &color);

    /*!
     * \brief Reimplement from QGraphicsEffect::boundingRectFor().
     */
    QRectF boundingRectFor(const QRectF &sourceRect) const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplement from QGraphicsEffect::draw().
     */
    void draw(QPainter *painter) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplement from QGraphicsEffect::sourceChanged().
     */
    void sourceChanged(ChangeFlags flags) Q_DECL_OVERRIDE;

private:
    qreal m_radius;
    QColor m_color;
};

#endif // KNGRAPHICSGLOWEFFECT_H
