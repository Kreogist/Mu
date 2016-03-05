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
#ifndef KNACCOUNTAVATARBUTTON_H
#define KNACCOUNTAVATARBUTTON_H

#include <QWidget>

/*!
 * \brief The KNRoundedImageButton class provides the notification center
 * customized button.
 */
class KNAccountAvatarButton : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNRoundedImageButton class.
     * \param parent The parent widget.
     */
    explicit KNAccountAvatarButton(QWidget *parent = 0);

    /*!
     * \brief Get the current login state.
     * \return The login state. If user has been set an avatar, then it will be
     * true. After reset the avatar, it will be false.
     */
    bool isLogin() const;

signals:
    /*!
     * \brief Ask to show the notification center widget.
     */
    void requireShowNotificationCenter();

public slots:
    /*!
     * \brief Set the account avatar pixmap.
     * \param avatar The avatar image pixmap.
     */
    void setAccountAvatar(const QPixmap &avatar);

    /*!
     * \brief Reset the account avatar, and clear up the login state.
     */
    void resetAccountAvatar();

    /*!
     * \brief Set the button size.
     * \param buttonSize The avatar button size.
     */
    void setButtonSize(int buttonSize);

    /*!
     * \brief Set the graphics margin of the button.\n
     * Notice: This function has be set first. It won't change any other
     * component.
     * \param graphicsMargin Graphics margin of the button contents.
     */
    void setGraphicsMargin(int graphicsMargin);

protected:
    /*!
     * \brief Reimplemented from KNImageLabel::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNImageLabel::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNImageLabel::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap m_anonymousPixmap, m_anonymousScaledPixmap, m_scaledPixmap;
    int m_graphicsMargin;
    bool m_pressed, m_isLogin;
};

#endif // KNACCOUNTAVATARBUTTON_H
