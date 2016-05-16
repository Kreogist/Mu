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

#ifndef KNACCOUNTREGISTERPANEL_H
#define KNACCOUNTREGISTERPANEL_H

#include "knaccountutil.h"

#include <QWidget>

using namespace AccountUtil;

class QLabel;
class QCheckBox;
class KNLabelLineEdit;
class KNOpacityAnimeButton;
/*!
 * \brief The KNAccountRegisterPanel class provides a widget to register a
 * Kreogist Account. The widget style is SAO style, this is a widget dependence
 * style widget.\n
 * It will show up link to user license, and not include in the register panel.
 */
class KNAccountRegisterPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountRegisterPanel widget.
     * \param parent The parent widget pointer.
     */
    explicit KNAccountRegisterPanel(QWidget *parent = 0);

    /*!
     * \brief Get the username line edit box text.
     * \return The user preferred username.
     */
    QString username() const;

    /*!
     * \brief Get the password line edit box text.
     * \return The user preferred password.
     */
    QString password() const;

    /*!
     * \brief Get the E-mail line edit box text.
     * \return The user preferred E-mail address.
     */
    QString email() const;

signals:
    /*!
     * \brief Ask account to register the user with the current information.
     */
    void requireRegister();

    /*!
     * \brief Cancel the register process, ask to show login panel.
     */
    void cancelRegister();

public slots:
    /*!
     * \brief Clear all the input data of register panel.
     */
    void clearInputData();

    /*!
     * \brief When error is happened, this slot should be called to handle the
     * error.
     * \param errorCode The error code. It should be in the enumeration
     * KNAccountUtil::RegisterErrorCode.
     */
    void onActionRegisterError(int errorCode);

private slots:
    void retranslate();
    void onActionCancel();
    void onActionCheckValid();

private:
    inline bool isInformationValid();
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    QLabel *m_passwordHint[PasswordHintTypeCount];
    QString m_errorDescription[RegisterErrorCodeCount];
    QColor m_validColor, m_invalidColor, m_emptyColor;
    QLabel *m_title, *m_emailHint, *m_errorHint;
    QCheckBox *m_agreeLicense;
    KNLabelLineEdit *m_username, *m_password, *m_email;
    KNOpacityAnimeButton *m_okay, *m_cancel;
    int m_errorCode;
};

#endif // KNACCOUNTREGISTERPANEL_H
