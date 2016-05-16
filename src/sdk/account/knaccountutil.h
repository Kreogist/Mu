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

#ifndef KNACCOUNTUTIL_H
#define KNACCOUNTUTIL_H

#include <QRegExp>
#include <QString>

namespace AccountUtil
{
    enum LoginErrorCode
    {
        LoginConnectionError,
        InfoIncorrect,
        LoginErrorCodeCount
    };

    enum RegisterErrorCode
    {
        UnknownRegisterError,
        UserNameAlreadyTaken,
        EmailAlreadyTaken,
        RegisterErrorCodeCount
    };

    enum ResetPasswordErrorCode
    {
        ResetConnectionError,
        ResetUnknownError,
        ResetPasswordCount
    };

    enum PasswordHintType
    {
        LengthRequest,
        NumberRequest,
        LetterRequest,
        PasswordHintTypeCount
    };
}

/*!
 * \brief The KNAccountUtil class is used to provide several account
 * enumerations, especially for error codes. It will also provides some public
 * static functions for account.
 */
class KNAccountUtil
{
public:
    /*!
     * \brief isPasswordValid
     * \param password
     * \param passwordValidResult
     * \return
     */
    static bool isPasswordValid(const QString &password,
                                bool *passwordValidResult=nullptr)
    {
        //Check whether the password is empty.
        if(password.isEmpty())
        {
            //Empty password is not valid.
            return false;
        }
        //Check password validation.
        bool passwordValidation[AccountUtil::PasswordHintTypeCount],
                validResult=true;
        passwordValidation[AccountUtil::LengthRequest]=
                (password.length()>5),
        passwordValidation[AccountUtil::NumberRequest]=
                password.contains(QRegExp("[0-9]")),
        passwordValidation[AccountUtil::LetterRequest]=
                (password.contains(QRegExp("[A-Z]")) &&
                 password.contains(QRegExp("[a-z]")));
        //Check the result pointer.
        if(!passwordValidResult)
        {
            //If the result is not null, output the result.
            for(int i=0; i<AccountUtil::PasswordHintTypeCount; ++i)
            {
                //Save the result to the pointer.
                passwordValidResult[i]=passwordValidation[i];
            }
        }
        //Calculate the final result by combine all the sub parts together.
        for(int i=0; i<AccountUtil::PasswordHintTypeCount; ++i)
        {
            //Calcualte the result.
            validResult = validResult && passwordValidation[i];
        }
        //Give the result back.
        return validResult;
    }

private:
    KNAccountUtil();
    KNAccountUtil(const KNAccountUtil &);
    KNAccountUtil(KNAccountUtil &&);
};

#endif // KNACCOUNTUTIL_H
