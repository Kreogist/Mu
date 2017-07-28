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

#ifndef KNPROGRESSSLIDER_H
#define KNPROGRESSSLIDER_H

#include "knabstractslider.h"

class QTimeLine;
/*!
 * \brief The KNProgressSlider class provides you a slider which might be used
 * mostly in the progress of the player.
 */
class KNProgressSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNProgressSlider class.
     * \param parent The parent widget of the slider.
     */
    explicit KNProgressSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimpelment from KNAbstractSlider::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNAbstractSlider::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNAbstractSlider::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNAbstractSlider::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNAbstractSlider::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNAbstractSlider::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    inline void updateButtonSize();
    inline qint64 posToValue(int position);
    QTimeLine *m_mouseInOut;
    QRadialGradient m_buttonGradient;
    qreal m_backOpacity;
    int m_buttonSize;
    bool m_pressed;
};

#endif // KNPROGRESSSLIDER_H
