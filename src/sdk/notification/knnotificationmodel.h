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

using namespace NotificationUtil;

class KNNotificationData;
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
    ~KNNotificationModel();

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
     * \brief Append the notification to the end of the notification model.
     * \param data The notification data pointer.
     * \return The notification item index.
     */
    QModelIndex appendNotification(KNNotificationData *data);

    /*!
     * \brief This is an reimplementation function. By providing the fixed
     * content, the notification will be add to the stack.
     * \param title The title of the notification.
     * \param content The content of the notification.
     * \param data The pointer of the notification object.
     * \return The notification index in the model.
     */
    QModelIndex appendNotification(const QString &title,
                                   const QString &content,
                                   KNNotificationData **data=nullptr);

    /*!
     * \brief Remove one notification from the model.
     * \param index The notification index.
     * \return If remove the notificaiton successfully, then return true;
     */
    bool removeNotification(const QModelIndex &index);

signals:

public slots:

private:
    inline QPixmap generateIcon(const QString &iconPath);
    QPixmap m_icon[NotificationIconCount];
    QList<KNNotificationData *> m_notifications;
};

#endif // KNNOTIFICATIONMODEL_H
