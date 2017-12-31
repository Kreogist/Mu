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
//Notification Dependences.
#include "notification/knnotificationmodel.h"
#include "notification/knnotificationwidget.h"
#include "knnotificationdata.h"

#include "knnotification.h"

KNNotification *KNNotification::m_instance=nullptr;

KNNotification::~KNNotification()
{
    //Loop and delete all the children.
    while(!m_popupNotifications.isEmpty())
    {
        //Take the last item.
        KNNotificationData *data=m_popupNotifications.takeLast();
        //Check the data parent.
        if(data->parent()==this)
        {
            //Remove the data item.
            delete data;
        }
    }
}

KNNotification *KNNotification::instance()
{
    return m_instance;
}

void KNNotification::initial(QObject *parent)
{
    //Check the instance first.
    if(nullptr==m_instance)
    {
        //Save the current instance as the global instance.
        m_instance=new KNNotification(parent);
    }
}

KNNotificationModel *KNNotification::model() const
{
    return m_model;
}

KNNotification::KNNotification(QObject *parent) :
    QObject(parent),
    m_model(new KNNotificationModel(this)),
    m_notificationWidget(nullptr),
    m_pushing(false)
{
}

void KNNotification::pushOnly(KNNotificationData *data)
{
    //Append to notification list.
    m_popupNotifications.append(data);
    //Check the pushing notification flag.
    if(!m_pushing)
    {
        //Set the pushing flag.
        m_pushing=true;
        //Set the notification information to widget.
        pushNotification(m_popupNotifications.takeFirst());
    }
}

void KNNotification::pushOnly(const QString &title, const QString &content)
{
    //Call the function original.
    pushOnly(new KNNotificationData(title, content, -1, this));
}

QModelIndex KNNotification::push(KNNotificationData *data)
{
    //Append the data to the model.
    QModelIndex notificationIndex=m_model->appendNotification(data);
    //Push the notification.
    pushOnly(data);
    //Give back the index.
    return notificationIndex;
}

QModelIndex KNNotification::push(const QString &title, const QString &content)
{
    //Prepare the data.
    KNNotificationData *data=nullptr;
    //Append the data to the model.
    QModelIndex notificationIndex=
            m_model->appendNotification(title, content, &data);
    //Push the notification.
    pushOnly(data);
    //Give back the index.
    return notificationIndex;
}

void KNNotification::pushNext()
{
    //Check the queue is empty or not.
    if(m_popupNotifications.isEmpty())
    {
        //Set the pushing flag to false.
        m_pushing=false;
        //Mission complete.
        return;
    }
    //Set the notification information to widget.
    pushNotification(m_popupNotifications.takeFirst());
}

inline void KNNotification::pushNotification(KNNotificationData *item)
{
    //Check notification widget.
    if(!m_notificationWidget)
    {
        return;
    }
    //Set the title and content.
    m_notificationWidget->setTitle(item->title());
    m_notificationWidget->setContent(item->content());
    //Set the icon to the notification widget.
    //!FIXME: Add codes here.
    //Check the current item.
    if(item->parent()==this)
    {
        //Recover the memory.
        delete item;
    }
    //Ask main window to popup notification.
    emit requirePushNotification();
}

void KNNotification::setNotificationWidget(
        KNNotificationWidget *notificationWidget)
{
    m_notificationWidget = notificationWidget;
}
