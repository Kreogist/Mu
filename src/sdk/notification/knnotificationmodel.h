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

#include <QPixmap>
#include <QList>

#include "knnotificationutil.h"

#include <QAbstractListModel>

/*!
 * \brief The KNNotificationModel class provides a model to store all the
 * notification which could be visible from a list view.
 */
class KNNotificationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNNotificationModel with given object.
     * \param parent The parent object.
     */
    explicit KNNotificationModel(QObject *parent = 0);

    /*!
     * \brief Preppend a notification to model.
     * \param title Notification title.
     * \param content Notification content.
     * \param type Notification type. It should be one of the NotificationType
     * data.
     * \return Notificaiton item index of the notification in the model.
     */
    QModelIndex prependRow(const QString &title,
                           const QString &content,
                           int type=KNNotificationUtil::Information,
                           int iconType=KNNotificationUtil::Message);

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::removeRows().
     */
    bool removeRows(int row,
                    int count,
                    const QModelIndex &parent) Q_DECL_OVERRIDE;

    /*!
     * \brief Remove one notification from the model.
     * \param index The notification index.
     * \return If remove the notificaiton successfully, then return true;
     */
    bool removeNotification(const QModelIndex &index);

signals:

public slots:

private:
    struct NotificationData
    {
        QString title;
        QString content;
        int type;
        int iconType;
        NotificationData() :
            type(KNNotificationUtil::Information),
            iconType(KNNotificationUtil::Message)
        {
        }
    };

    QPixmap m_icon[KNNotificationUtil::NotificationIconCount];
    QList<NotificationData> m_notifications;
};

#endif // KNNOTIFICATIONMODEL_H
