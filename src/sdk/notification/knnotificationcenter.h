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

#ifndef KNNOTIFICATIONCENTER_H
#define KNNOTIFICATIONCENTER_H

#include <QFrame>

class QLabel;
class QPropertyAnimation;
class KNAccountPanel;
class KNAccountAvatarButton;
class KNNotificationView;
class KNNotificationWidget;
/*!
 * \brief The KNNotificationCenter class provides the ability to show all the
 * information in a single place.
 */
class KNNotificationCenter : public QFrame
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNNotificationCenter widget.
     * \param parent The parent widget.
     */
    explicit KNNotificationCenter(QWidget *parent = 0);

    /*!
     * \brief Get the notification header button.
     * \return The button widget pointer.
     */
    KNAccountAvatarButton *headerButton();

    /*!
     * \brief Get the indicator widget of the notification center.
     * \return The indicator widget pointer.
     */
    QWidget *indicator();

    /*!
     * \brief The prefer height of the notification height.
     */
    int heightHint();

    /*!
     * \brief Get the notification widget.
     * \return The notification widget pointer.
     */
    KNNotificationWidget *notificationWidget();

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QFrame::showEvent().
     */
    void showEvent(QShowEvent *event)  Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QFrame::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QFrame::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void resizeNotificationCenter();

private:
    QLabel *m_notificationIndicator;
    QPropertyAnimation *m_resizeAnime;
    KNAccountAvatarButton *m_button;
    KNNotificationView *m_notificationView;
    KNNotificationWidget *m_notificationWidget;
    KNAccountPanel *m_accountPanel;
    int m_heightHint;
};

#endif // KNNOTIFICATIONCENTER_H
