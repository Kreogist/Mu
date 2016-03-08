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
#ifndef KNACCOUNTDETAILBOX_H
#define KNACCOUNTDETAILBOX_H

#include <QJsonObject>

#include "knmessagebox.h"

class QLabel;
class KNAccountDetails;
class KNLabelLineEdit;
/*!
 * \brief The KNAccountDetailBox class provides a widget which could edit the
 * user details, including the nick name, address.
 */
class KNAccountDetailBox : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountDetailBox widget.
     * \param parent The parent widget pointer.
     */
    explicit KNAccountDetailBox(QWidget *parent = 0);

signals:
    /*!
     * \brief When user clicked okay, this signal will be emitted for asking
     * to update information.
     * \param updateData Updated data in JSON object.
     */
    void requireUpdateInfo(QJsonObject updateData);

public slots:
    /*!
     * \brief Set the account details information to this editor.
     * \param details The account details information.
     */
    void setAccountDetails(KNAccountDetails *details);

protected:
    /*!
     * \brief Reimplemented from KNMessageBox::okayPressed().
     */
    bool okayPressed() Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    enum EditRows
    {
        UserNameRow,
        NickNameRow,
        EmailAddressRow,
        EditRowCount
    };
    inline KNLabelLineEdit *generateLineEdit();

    QLabel *m_title[EditRowCount], *m_username;
    KNLabelLineEdit *m_nickName, *m_emailAddress;
};

#endif // KNACCOUNTDETAILBOX_H
