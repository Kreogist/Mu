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

#ifndef KNNOTIFICATIONVIEW_H
#define KNNOTIFICATIONVIEW_H

#include <QListView>

/*!
 * \brief The KNNotificationView class is provided to show the notification
 * model, and display the empty hint when there's no notification.
 */
class KNNotificationView : public QListView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNNotificationView widget.
     * \param parent The parent widget.
     */
    explicit KNNotificationView(QWidget *parent = 0);

    /*!
     * \brief Get the height which the view prefer.
     * \return The height hint.
     */
    int heightHint() const;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QListView::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    QString m_emptyHint;
};

#endif // KNNOTIFICATIONVIEW_H
