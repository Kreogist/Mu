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

#ifndef KNNOTIFICATION_H
#define KNNOTIFICATION_H

#include <QObject>

#define knNotification (KNNotification::instance())

class KNNotificationModel;
/*!
 * \brief The KNNotification class is a global static object which provides the
 * notification push services for the whole application. All the message will be
 * bounded here.
 */
class KNNotification : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance of notification service object.
     * \return The global notification object pointer.
     */
    static KNNotification *instance();

    /*!
     * \brief Initial the manager, generate the instance with the given parent
     * object.\n
     * Only the first time will create a instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the notification model pointer.
     * \return The notification model pointer.
     */
    KNNotificationModel *model() const;

    /*!
     * \brief Push an notification here, and add the notification to the stack.
     * \param title The title of notification.
     * \param content The notification content.
     */
    void push(const QString &title,
              const QString &content);

    /*!
     * \brief Push an notification, but not add this notification to stack.
     * \param title The title of notification.
     * \param content The content of the notification.
     */
    void pushOnly(const QString &title,
                  const QString &content);

    /*!
     * \brief Add the notification to stack, but this notification won't pop up.
     * \param title The title of notificaiton.
     * \param content The notification content.
     */
    void addToStack(const QString &title,
                    const QString &content);

signals:
    /*!
     * \brief This signal is asking main window to push the notification.
     */
    void requirePushNotification();

public slots:
    /*!
     * \brief Main window will call this slot to push the queued notifications.
     */
    void onActionPushNextNotification();

private slots:
    void onActionPushNotification();

private:
    struct PopupNotification
    {
        QString title;
        QString content;
    };

    explicit KNNotification(QObject *parent = 0);
    //Disable the copy of the instance.
    KNNotification(const KNNotification &);
    KNNotification(KNNotification &&);
    inline void setNotification(const PopupNotification &item);

    static KNNotification *m_instance;

    QList<PopupNotification> m_popupNotifications;
    KNNotificationModel *m_model;
    bool m_pushing;
};

#endif // KNNOTIFICATION_H
