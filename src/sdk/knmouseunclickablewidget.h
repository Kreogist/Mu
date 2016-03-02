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

#ifndef KNMOUSEUNCLICKABLEWIDGET_H
#define KNMOUSEUNCLICKABLEWIDGET_H

#include <QWidget>

/*!
 * \brief The KNMouseUnclickableWidget class provides the widget which doesn't
 * access mouse event.
 */
class KNMouseUnclickableWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseUnclickableWidget widget with given parent
     * widget.
     * \param parent The parent widge pointer.
     */
    KNMouseUnclickableWidget(QWidget *parent = 0):QWidget(parent){}

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // KNMOUSEUNCLICKABLEWIDGET_H
