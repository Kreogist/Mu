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

#include <QWidget>

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

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();

private:
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    QLabel *m_title, *m_emailHint, *m_passwordHint;
    QCheckBox *m_agreeLicense;
    KNLabelLineEdit *m_username, *m_password, *m_email;
    KNOpacityAnimeButton *m_login, *m_cancel;
};

#endif // KNACCOUNTREGISTERPANEL_H
