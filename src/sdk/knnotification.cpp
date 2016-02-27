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

#include "knnotification.h"

KNNotification *KNNotification::m_instance=nullptr;

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

KNNotification::KNNotification(QObject *parent) :
    QObject(parent),
    m_model(new KNNotificationModel(this)),
    m_notificationWidget(nullptr),
    m_pushing(false)
{
}

KNNotificationModel *KNNotification::model() const
{
    return m_model;
}

void KNNotification::push(const QString &title, const QString &content)
{
    //Push means we need to show the notification and add to notification center
    //stack.
    pushOnly(title, content);
    //Add to stack.
    addToStack(title, content);
}

void KNNotification::pushOnly(const QString &title, const QString &content)
{
    //Construct the notification.
    PopupNotification notification;
    notification.title=title;
    notification.content=content;
    //Append to notification list.
    m_popupNotifications.append(notification);
    //Ask to push notificaiton.
    onActionPushNotification();
}

void KNNotification::addToStack(const QString &title, const QString &content)
{
    //Add notification to model.
    m_model->prependRow(title, content);
}

void KNNotification::onActionPushNextNotification()
{
    //Check the queue is empty or not.
    if(m_popupNotifications.isEmpty())
    {
        //Set the pushing flag o false.
        m_pushing=false;
        //Mission complete.
        return;
    }
    //Set the notification information to widget.
    setNotification(m_popupNotifications.takeFirst());
    //Ask main window to popup notification.
    emit requirePushNotification();
}

void KNNotification::onActionPushNotification()
{
    //Check the flag.
    if(m_pushing)
    {
        //The pushing loop is running
        return;
    }
    //Set the pushing flag.
    m_pushing=true;
    //Set the notification information to widget.
    setNotification(m_popupNotifications.takeFirst());
    //Emit the requirement signal.
    emit requirePushNotification();
}

void KNNotification::setNotification(const PopupNotification &item)
{
    //Check notification widget.
    if(!m_notificationWidget)
    {
        return;
    }
    //Set the title and content.
    m_notificationWidget->setTitle(item.title);
    m_notificationWidget->setContent(item.content);
}

void KNNotification::setNotificationWidget(
        KNNotificationWidget *notificationWidget)
{
    m_notificationWidget = notificationWidget;
}
