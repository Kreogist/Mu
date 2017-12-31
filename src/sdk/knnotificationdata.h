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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNNOTIFICATIONDATA_H
#define KNNOTIFICATIONDATA_H

#include <QObject>
#include <QPixmap>

/*!
 * \brief The KNNotificationData class provides the data of one piece of the
 * notification, includes all the following data:
 *  - Icon
 *  - Title and content text
 * All these data could be fixed, or modified by the inherit classes. The parent
 * class should be responsed to manage the memory of the notification class
 * manually (for avoiding to use QObject).
 */
class KNNotificationData
{
public:
    /*!
     * \brief Construct a KNNotificationData object.
     * \param parent The parent object pointer.
     */
    explicit KNNotificationData(QObject *parent=0);
    virtual ~KNNotificationData();

    /*!
     * \brief Construct a KNNotificationData with fixed content.
     * \param title The display title text.
     * \param content The display content text.
     * \param iconIndex The default icon index, for customized icon, set -1.
     * \param parent The parent object pointer.
     */
    explicit KNNotificationData(QString title, QString content,
                                int iconIndex=-1,
                                QObject *parent=0);

    /*!
     * \brief Get the notification data parent object pointer.
     * \return The parent pointer.
     */
    QObject *parent() const;

    /*!
     * \brief Get the title string, the default implementation is to return the
     * fixed title.
     * \return The title string.
     */
    virtual QString title() const;

    /*!
     * \brief Get the content string, the default implementation is to return
     * the fixed content.
     * \return The content string.
     */
    virtual QString content() const;

    /*!
     * \brief Get the icon of the notification.
     * \return The icon pixmap of the notification.
     */
    QPixmap icon() const;

    /*!
     * \brief Set the customized icon.
     * \param icon The icon pixmap.
     */
    void setIcon(const QPixmap &icon);

private:
    QPixmap m_icon;
    QString m_fixedTitle, m_fixedContent;
    QObject *m_parent;
    int m_iconIndex;
};

#endif // KNNOTIFICATIONDATA_H
