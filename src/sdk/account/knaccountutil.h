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

#include <QCryptographicHash>
#include <QRegExp>
#include <QString>

#include <QDebug>

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
        ResetCannotFindEmail,
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
     * \brief Check whether the password is valid or not.
     * \param password The original password.
     * \param validResult Get the condition is valid or not result.
     * \return If the password is valid, return true.
     */
    static bool isPasswordValid(const QString &password,
                                bool *validResult=nullptr)
    {
        //Check whether the password is empty.
        if(password.isEmpty())
        {
            //Empty password is not valid.
            return false;
        }
        //Check password validation.
        bool validation[AccountUtil::PasswordHintTypeCount],
                result=true;
        validation[AccountUtil::LengthRequest]=
                (password.length()>5);
        validation[AccountUtil::NumberRequest]=
                password.contains(QRegExp("[0-9]"));
        validation[AccountUtil::LetterRequest]=
                (password.contains(QRegExp("[A-Z]")) &&
                 password.contains(QRegExp("[a-z]")));
        //Check the result pointer.
        if(validResult)
        {
            //If the result is not null, output the result.
            for(int i=0; i<AccountUtil::PasswordHintTypeCount; ++i)
            {
                //Save the result to the pointer.
                validResult[i]=validation[i];
            }
        }
        //Calculate the final result by combine all the sub parts together.
        for(int i=0; i<AccountUtil::PasswordHintTypeCount; ++i)
        {
            //Calcualte the result.
            result = result && validation[i];
        }
        //Give the result back.
        return result;
    }

    /*!
     * \brief Change a byte array into a string which is the hex text of the
     * bytes.
     * \param bytes The byte array.
     * \return The Hex string.
     */
    static QString bytesToHex(const QByteArray &bytes)
    {
        //Create the cache string.
        QString hexCache;
        //For all the bytes.
        for(int i=0; i<bytes.size(); ++i)
        {
            //Check the unsigned number of the bytes.
            quint8 byte=(quint8)bytes.at(i);
            //Get the text string of the current number.
            QString currentByte=QString::number(byte, 16);
            //If the byte is smaller than 16.
            if(byte<16)
            {
                //Add a 0 to fit the pos.
                hexCache.append("0");
            }
            //Add all byte to hex cache.
            hexCache.append(currentByte);
        }
        //Give back the hex cache.
        return hexCache;
    }

    /*!
     * \brief Get the access password by input the raw password.
     * \param rawPassword The raw password.
     * \return Give back the encrypted password using to log in or registered.
     */
    static QString accessPassword(const QString &rawPassword)
    {
        //Use MD5 and SHA-3 to combine the access password.
        return bytesToHex(QCryptographicHash::hash(
                              rawPassword.toUtf8(),
                              QCryptographicHash::Sha3_512).append
                          (QCryptographicHash::hash(
                               rawPassword.toUtf8(),
                               QCryptographicHash::Md5)));
    }

private:
    KNAccountUtil();
    KNAccountUtil(const KNAccountUtil &);
    KNAccountUtil(KNAccountUtil &&);
};

#endif // KNACCOUNTUTIL_H
