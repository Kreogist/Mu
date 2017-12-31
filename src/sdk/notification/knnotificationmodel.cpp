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

#include "knnotificationdata.h"

#include "knnotificationmodel.h"

#define ImageSize 35

KNNotificationModel::KNNotificationModel(QObject *parent) :
    QAbstractListModel(parent),
    m_notifications(QList<KNNotificationData *>())
{
    //Initial the icon image.
    m_icon[Message]=generateIcon("://public/notification_default.png");
}

KNNotificationModel::~KNNotificationModel()
{
    //Loop and check all the item.
    while(!m_notifications.isEmpty())
    {
        //Take the item from the notifications.
        KNNotificationData *data=m_notifications.takeLast();
        //Check the item.
        if(data->parent()==this)
        {
            //Delete the item.
            delete data;
        }
    }
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
    const KNNotificationData *targetData=m_notifications.at(index.row());
    //Check out the role.
    switch(role)
    {
    case Qt::DisplayRole:
        //Give back the title as display role.
        return targetData->title();
    case Qt::DecorationRole:
        //Give back the icon.
        return m_icon[Message];
    case ContentRole:
        //Give back the content data.
        return targetData->content();
    default:
        return QVariant();
    }
}

bool KNNotificationModel::removeRows(int row,
                                     int count,
                                     const QModelIndex &parent)
{
    Q_UNUSED(parent)
    //Check the data.
    if(m_notifications.isEmpty())
    {
        //No need to remove data.
        return true;
    }
    //Start to remove the row.
    beginRemoveRows(QModelIndex(), row, row+count-1);
    //Remove those datas from the list.
    while(count--)
    {
        //Take away the detail info, and remove the duration.
        KNNotificationData *data=m_notifications.takeAt(row);
        //Check the data.
        if(data->parent()==this)
        {
            //Delete the object to recover the memory.
            delete data;
        }
    }
    //As the documentation said, called this after remove rows.
    endRemoveRows();
    //Complete.
    return true;
}

QModelIndex KNNotificationModel::appendNotification(KNNotificationData *data)
{
    //Calculate the insert position.
    int insertPosition=m_notifications.size();
    //Begin to append the data.
    beginInsertRows(QModelIndex(), insertPosition, insertPosition);
    //Simply append the data to the end of the list.
    m_notifications.append(data);
    //End append the data.
    endInsertRows();
    //Calculate the model index.
    return index(0, insertPosition);
}

QModelIndex KNNotificationModel::appendNotification(const QString &title,
                                                    const QString &content,
                                                    KNNotificationData **data)
{
    //Create the notificaiton data.
    KNNotificationData *notification=
            new KNNotificationData(title, content, -1, this);
    //Check the output pointer.
    if(data)
    {
        //Save the notification object to the pointer.
        (*data)=notification;
    }
    //Construct and append the notificaiton.
    return appendNotification(notification);
}

bool KNNotificationModel::removeNotification(const QModelIndex &index)
{
    //Check index validation.
    if(index.isValid())
    {
        //Failed to remove.
        return false;
    }
    //Remove the specific row.
    return removeRow(index.row());
}

inline QPixmap KNNotificationModel::generateIcon(const QString &iconPath)
{
    //Return the scaled icon.
    return QPixmap(iconPath).scaled(ImageSize, ImageSize,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation);
}
