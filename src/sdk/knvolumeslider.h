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

#ifndef KNVOLUMESLIDER_H
#define KNVOLUMESLIDER_H

#include "knabstractslider.h"

class QTimeLine;
/*!
 * \brief The KNVolumeSlider class provides a slider set for the volume. This
 * slider does not have a button like indicator. But the content of the
 * selecting value is changed according to the user's selection.
 */
class KNVolumeSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNVolumeSlider widget.
     * \param parent The parent widget.
     */
    explicit KNVolumeSlider(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNAbstractSlider::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractSlider::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractSlider::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractSlider::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractSlider::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNAbstractSlider::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onMouseInOut(int frame);

private:
    inline qint64 posToValue(int position);
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseInOut;
    QColor m_rectColor;
    bool m_pressed;
};

#endif // KNVOLUMESLIDER_H
