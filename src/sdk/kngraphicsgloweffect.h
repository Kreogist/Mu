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

/*!
 * \brief The KNGraphicsGlowEffect class provides a glow effect. This class is
 * referenced from Tianchi Qt Shared Library Project
 * (https://github.com/qtcn/tianchi). This effect will increase the size of the
 * original widget.
 */
class KNGraphicsGlowEffect : public QGraphicsEffect
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNGraphicsGlowEffect graphics effect.
     * \param parent The parent object.
     */
    explicit KNGraphicsGlowEffect(QObject *parent = 0);

    /*!
     * \brief Get the radius of the glow.
     * \return The glow radius. It will be 0 as default.
     */
    qreal radius() const;

    /*!
     * \brief Set the radius of the glow.
     * \param radius The new glow radius.
     */
    void setRadius(const qreal &radius);

    /*!
     * \brief Get the glow color.
     * \return The glow color. It will be white(#FFFFFFFF) as default.
     */
    QColor color() const;

    /*!
     * \brief Set the glow color.
     * \param color The new glow color.
     */
    void setColor(const QColor &color);

    /*!
     * \brief Reimplemented from QGraphicsEffect::boundingRectFor().
     */
    QRectF boundingRectFor(const QRectF &sourceRect) const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QGraphicsEffect::draw().
     */
    void draw(QPainter *painter) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QGraphicsEffect::sourceChanged().
     */
    void sourceChanged(ChangeFlags flags) Q_DECL_OVERRIDE;

private:
    qreal m_radius;
    QColor m_color;
};

#endif // KNGRAPHICSGLOWEFFECT_H
