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

#ifndef KNOPACITYBUTTON_H
#define KNOPACITYBUTTON_H

#include <QAbstractButton>

/*!
 * \brief The KNOpacityButton class provides a basic button for opacity changed
 * feedback for clicking.
 */
class KNOpacityButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNOpacityButton widget with a given widget.
     * \param parent The parent widget.
     */
    explicit KNOpacityButton(QWidget *parent = 0);

    /*!
     * \brief Get the current opacity of the opacity button.
     * \return The opacity of the button.
     */
    qreal opacity() const;

signals:

public slots:
    /*!
     * \brief Set the button display opacity value.
     * \param opacity The opacity display parameter of the button.
     */
    void setOpacity(const qreal &opacity);

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    inline qreal getPressedOpacity();
    qreal m_originalOpacity, m_opacity;
};

#endif // KNOPACITYBUTTON_H
