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

#ifndef KNNOTIFICATIONMODEL_H
#define KNNOTIFICATIONMODEL_H

#include <QList>

#include <QAbstractListModel>

/*!
 * \brief The KNNotificationModel class provides a model to store all the
 * notification which could be visible from a list view.
 */
class KNNotificationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotificationRole
    {
        ContentRole = Qt::UserRole+1
    };

    /*!
     * \brief Construct a KNNotificationModel with given object.
     * \param parent The parent object.
     */
    explicit KNNotificationModel(QObject *parent = 0);

    /*!
     * \brief Preppend a notification to model.
     * \param title
     * \param content
     */
    void prependRow(const QString &title,
                    const QString &content);

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    struct NotificationData
    {
        QString title;
        QString content;
    };
    QList<NotificationData> m_notifications;
};

#endif // KNNOTIFICATIONMODEL_H
