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

#ifndef KNACCOUNTPASSWORDBOX_H
#define KNACCOUNTPASSWORDBOX_H

#include "knmessagebox.h"

class QLabel;
class KNAccountPasswordVerification;
class KNHWidgetSwitcher;
class KNLabelLineEdit;
/*!
 * \brief The KNAccountPasswordBox class provides a message box which could help
 * to input password. After validation the original password, message box will
 * display the new password input method.
 */
class KNAccountPasswordBox : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountPasswordBox message box.
     * \param parent The parent widget.
     */
    explicit KNAccountPasswordBox(QWidget *parent = 0);

signals:
    /*!
     * \brief Ask account service to change the password.
     * \param encryptedPassword Encrypted password.
     */
    void requireUpdatePassword(QString encryptedPassword);

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMessageBox::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMessageBox::okayPressed().
     */
    bool okayPressed() Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionAcceptPassword();
    void onActionPassword1Change(const QString &password);
    void checkOkayButton();

private:
    enum PasswordHintType
    {
        LengthRequest,
        NumberRequest,
        LetterRequest,
        PasswordHintTypeCount
    };

    inline KNLabelLineEdit *generatePassword();
    QLabel *m_passwordHint[PasswordHintTypeCount];
    QString m_hintText;
    QColor m_validColor, m_invalidColor, m_emptyColor;
    QLabel *m_resetTitle, *m_resetHint;
    KNHWidgetSwitcher *m_passwordContainer;
    KNAccountPasswordVerification *m_passwordVerification;
    KNLabelLineEdit *m_password1, *m_password2;
    bool m_validPassword;
};

#endif // KNACCOUNTPASSWORDBOX_H
