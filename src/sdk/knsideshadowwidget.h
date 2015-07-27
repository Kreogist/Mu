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

#ifndef KNSIDESHADOWWIDGET_H
#define KNSIDESHADOWWIDGET_H

#include <QWidget>

/*!
 * \brief The KNSideShadowWidget class provides you a one side shadow which the
 * widget is mouse event transparent. You have to give the direction of the
 * shadow. To change the size of the shadow, just change the size of the widget.
 */
class KNSideShadowWidget : public QWidget
{
    Q_OBJECT
public:
    enum ShadowDirection
    {
        TopShadow,
        LeftShadow,
        RightShadow,
        BottomShadow
    };
    /*!
     * \brief Construct a KNSideShadowWidget with a given direction.
     * \param direction The direction of the shadow, for default it is
     * TopShadow.
     * \param parent The parent widget.
     */
    explicit KNSideShadowWidget(int direction=TopShadow,
                                QWidget *parent = 0);

    /*!
     * \brief Get the direction of the widget.
     * \return The shadow direction. It will be one value in shadow direction.
     */
    int direction() const;

    /*!
     * \brief Set the shadow direction.
     * \param direction The direction it prefer.
     */
    void setDirection(int direction);

    /*!
     * \brief Set the darkness of the shadow. Actually, it's the alpha channel
     * of the darkest part. Default it's 70.
     * \param darkness The alpha channel value of the darkest part. It should be
     * ranged from 0 to 255.
     */
    void setDarkness(int darkness);

signals:

public slots:
    /*!
     * \brief Reimplement from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event);

    /*!
     * \brief Reimplement from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

private:
    inline void updateGradient();
    int m_direction;
    QLinearGradient m_shadowGradient;
};

#endif // KNSIDESHADOWWIDGET_H
