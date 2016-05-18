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
#ifndef KNACCOUNTRESETPANEL_H
#define KNACCOUNTRESETPANEL_H

#include "knaccountutil.h"

#include <QWidget>

using namespace AccountUtil;

class QLabel;
class KNImageLabel;
class KNLabelLineEdit;
class KNOpacityAnimeButton;
/*!
 * \brief The KNAccountResetPanel class provides a panel for reset the password
 * by filling the E-mail address of the Kreogist Account. This widget will
 * generate a request to sending the reset E-mail.
 */
class KNAccountResetPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountResetPanel widget.
     * \param parent The parent widget.
     */
    explicit KNAccountResetPanel(QWidget *parent = 0);

signals:
    /*!
     * \brief Ask to send a E-mail to the specific address.
     * \param emailAddress The E-mail Address.
     */
    void requireSendEmail(const QString &emailAddress);

    /*!
     * \brief When user is clicking cancel button, this signal will be emitted
     * for cancel the request.
     */
    void requireCancel();

    /*!
     * \brief When the reset E-mail has aready sent success.
     */
    void resetMailSentSuccess();

public slots:
    /*!
     * \brief When the reset mail has already be sent, this slot should be
     * called.
     */
    void onActionSentSuccess();

    /*!
     * \brief When there's an error happened when sending a reset E-mail, this
     * slot should be called.
     * \param errorCode The error code defined in the account util.
     */
    void onActionSentFailed(int errorCode);

protected:
    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionCheckEmail(const QString &content);
    void onActionSentEmail();

private:
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    QString m_errorMessage[ResetPasswordCount], m_hintText;
    QRegExp m_emailExp;
    KNImageLabel *m_resetLogo;
    QLabel *m_title, *m_hintLabel;
    KNLabelLineEdit *m_emailAddress;
    KNOpacityAnimeButton *m_okay, *m_cancel;
};

#endif // KNACCOUNTRESETPANEL_H
