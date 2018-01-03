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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNSAOOPTIONBUTTON_H
#define KNSAOOPTIONBUTTON_H

#include <QAbstractButton>

/*!
 * \brief The KNSaoOptionButton class provides the option buttons used for the
 * options selection dialog with Sword Art Online style.
 */
class KNSaoOptionButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSaoOptionButton widget.
     * \param parent The parent widget.
     */
    explicit KNSaoOptionButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_buttonText;
    bool m_mouseEnter;
};

#endif // KNSAOOPTIONBUTTON_H
