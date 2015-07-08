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

#ifndef KNMOUSEDETECTHEADER_H
#define KNMOUSEDETECTHEADER_H

#include <QWidget>

/*!
 * \brief The KNMouseDetectHeader class is a simple mouse sense widget. When the
 * mouse pointer move in, the requireActivateWidget() signal will be emitted.
 * When the mouse pointer move out, the requireInactivateWidget() signal will be
 * emit.
 */
class KNMouseDetectHeader : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseDetectHeader widget.
     * \param parent The parent widget.
     */
    explicit KNMouseDetectHeader(QWidget *parent = 0);

signals:
    /*!
     * \brief When the mouse entered, this signal will be emitted.
     */
    void requireActivateWidget();

    /*!
     * \brief When the mouse leaveed, this signal will be emitted.
     */
    void requireInactivateWidget();

public slots:

protected:
    /*!
     * \brief Reimplement from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplement from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event);
};

#endif // KNMOUSEDETECTHEADER_H
