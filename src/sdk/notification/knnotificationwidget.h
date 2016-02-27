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
#ifndef KNNOTIFICATIONWIDGET_H
#define KNNOTIFICATIONWIDGET_H

#include <QWidget>

/*!
 * \brief The KNNotificationWidget class provides the notification widget which
 * could popup to show the notification in the main window.
 */
class KNNotificationWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNNotificationWidget widget.
     * \param parent The paretn widget.
     */
    explicit KNNotificationWidget(QWidget *parent = 0);

    /*!
     * \brief Get the title of the notification.
     * \return The title text.
     */
    QString title() const;

    /*!
     * \brief Get the content of the notification.
     * \return The content text.
     */
    QString content() const;

    /*!
     * \brief Get the current icon of the notification.
     * \return The notificaiton icon pixmap.
     */
    QPixmap currentIcon() const;

signals:
    /*!
     * \brief When the widget is clicked, this signal will be emitted.
     */
    void requireHideNotification();

public slots:
    /*!
     * \brief Set the notification widget current icon.
     * \param currentIcon The notification icon. If the icon is null, will use
     * the default icon instead. But it won't replace the current icon property.
     */
    void setCurrentIcon(const QPixmap &currentIcon);

    /*!
     * \brief Reset the pressed flag when the hide animation start.
     */
    void resetPressedFlag();

    /*!
     * \brief Set the title of the notification.
     * \param title Notification title text.
     */
    void setTitle(const QString &title);

    /*!
     * \brief Set the content of the notification.
     * \param content Notification content text.
     */
    void setContent(const QString &content);

protected:
    /*!
     * \brief Reimplemented from KNNotificationWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNNotificationWidget::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNNotificationWidget::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap m_defaultIcon, m_currentIcon;
    QString m_title, m_content;
    bool m_pressed;
};

#endif // KNNOTIFICATIONWIDGET_H
