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

#include "knnotificationmodel.h"

KNNotificationModel::KNNotificationModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void KNNotificationModel::prependRow(const QString &title,
                                     const QString &content)
{
    //Generate the notification.
    NotificationData notification;
    //Set the data.
    notification.title=title;
    notification.content=content;
    //Begin to append row.
    beginInsertRows(QModelIndex(), 0, 0);
    //Insert data to list.
    m_notifications.prepend(notification);
    //End to append row.
    endInsertRows();
}

int KNNotificationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //The row count is the notification list size.
    return m_notifications.size();
}

QVariant KNNotificationModel::data(const QModelIndex &index, int role) const
{
    //Check the validation of index.
    if(!index.isValid())
    {
        return QVariant();
    }
    //Get the target data.
    const NotificationData &targetData=m_notifications.at(index.row());
    //Check out the role.
    switch(role)
    {
    case Qt::DisplayRole:
        //Give back the title as display role.
        return targetData.title;
    case ContentRole:
        //Give back the content data.
        return targetData.content;
    default:
        return QVariant();
    }

    ;
}
