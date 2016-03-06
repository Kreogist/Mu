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

#ifndef KNACCOUNTPASSWORDVERIFICATION_H
#define KNACCOUNTPASSWORDVERIFICATION_H

#include <QWidget>

class QLabel;
class QTimeLine;
class KNLabelLineEdit;
class KNOpacityAnimeButton;
/*!
 * \brief The KNAccountPasswordVerification class provides a widget which could
 * check whether the user input a correct or incorrect password as the current
 * one.\n
 * Before you use this widget, ensure there's a cached password in the user
 * detail.
 */
class KNAccountPasswordVerification : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNAccountPasswordVerification widget.
     * \param parent The parent widget pointer.
     */
    explicit KNAccountPasswordVerification(QWidget *parent = 0);

signals:
    /*!
     * \brief When user click the validate button, and password pass the check,
     * this signal will be emitted.
     */
    void passwordCorrect();

    /*!
     * \brief When user click the validate button, and password didn't pass the
     * check, this signal will be emitted.
     */
    void passwordWrong();

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionUpdateAlpha(int frame);
    void onActionCheckPassword();

private:
    inline QTimeLine *generateTimeLine();
    QLabel *m_authTitle, *m_authHint, *m_errorHint;
    KNLabelLineEdit *m_passwordEdit;
    KNOpacityAnimeButton *m_checkButton;
    QTimeLine *m_errorFadeIn, *m_errorFadeOut;
    QTimer *m_displayDelay;
};

#endif // KNACCOUNTPASSWORDVERIFICATION_H
