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
#ifndef KNNOTIFICATIONWIDGET_H
#define KNNOTIFICATIONWIDGET_H

#include <QWidget>

/*!
 * \brief The KNNotificationWidget class provides the notification widget which
 * could popup to show the notification in the main window.
 */
class KNNotificationWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNNotificationWidget widget.
     * \param parent The paretn widget.
     */
    explicit KNNotificationWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNNotificationWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_title, m_content;
};

#endif // KNNOTIFICATIONWIDGET_H
