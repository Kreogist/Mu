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

#ifndef KNMOUSEDETECTLABEL_H
#define KNMOUSEDETECTLABEL_H

#include <QLabel>

/*!
 * \brief The KNMouseDetectLabel class provides a widget which will emit signals
 * when the mouse cursor enter and leave the widget.
 */
class KNMouseDetectLabel : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseDetectLabel widget.
     * \param parent The parent widget.
     */
    explicit KNMouseDetectLabel(QWidget *parent = 0);

signals:
    /*!
     * \brief When the mouse entered the area, this signal will be emitted.
     */
    void mouseEntered();

    /*!
     * \brief When the mouse leaved the area, this signal will be emitted.
     */
    void mouseLeaved();

public slots:

protected:
    /*!
     * \brief Reimplemented from QLabel::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLabel::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
};

#endif // KNMOUSEDETECTLABEL_H
