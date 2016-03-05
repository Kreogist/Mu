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

#ifndef KNACCOUNTDETAILPANEL_H
#define KNACCOUNTDETAILPANEL_H

#include <QWidget>

class QLabel;
class QTimeLine;
class KNAccountAvatarButton;
class KNAccountDetails;
class KNLabelButton;
class KNOpacityAnimeButton;
/*!
 * \brief The KNAccountDetailPanel class provides the detail of Kreogist Account
 * information.
 */
class KNAccountDetailPanel : public QWidget
{
    Q_OBJECT
public:
    enum StateText
    {
        OperateSuccess,
        FailedToUpdateData,
        FailedToUpdateAvatar,
        StateTextCount
    };

    /*!
     * \brief Construct a KNAccountDetailPanel widget.
     * \param parent The parent widget.
     */
    explicit KNAccountDetailPanel(QWidget *parent = 0);

signals:
    /*!
     * \brief When user is asking for logout, this signal will be emitted.
     */
    void requireLogout();

    /*!
     * \brief When user is asking for update avatar, this signal will be
     * emitted.
     * \param pixmap Avatar pixmap. Size should be 100x100 px.
     */
    void requireUpdateAvatar(QPixmap pixmap);

public slots:
    /*!
     * \brief Clear all the user information stored in detail panel.
     */
    void clearUserInfo();

    /*!
     * \brief Display a state text.
     * \param state State in StateText enumerate.
     */
    void setStateText(int state);

private slots:
    void retranslate();
    void onActionDetailUpdate();
    void onActionAvatarUpdate();

    void onActionChangeStateAlpha(int frame);
    void startFadeOut();

    void onActionEditInformation();
    void onActionChangePassword();
    void onActionSelectAvatar();

private:
    enum ControlButtons
    {
        EditInformation,
        ChangePassword,
        SetAvatar,
        Logout,
        ControlButtonCount
    };

    inline KNOpacityAnimeButton *generateButton();
    inline void disableAllControls();
    inline void enabledAllControls();

    QString m_stateText[StateTextCount];
    KNOpacityAnimeButton *m_controls[ControlButtonCount];
    QColor m_failedColor, m_successColor;
    QString m_lastDirectory;
    KNAccountAvatarButton *m_avatarImage;
    QLabel *m_nickName, *m_state;
    KNAccountDetails *m_accountDetails;
    QTimeLine *m_fadeInAnime, *m_fadeOutAnime;
    QTimer *m_stateFadeTimer;
};

#endif // KNACCOUNTDETAILPANEL_H
