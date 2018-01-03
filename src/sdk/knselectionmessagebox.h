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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNSELECTIONMESSAGEBOX_H
#define KNSELECTIONMESSAGEBOX_H

#include "knmessagebox.h"

class QLabel;
class QButtonGroup;
/*!
 * \brief The KNSelectionMessageBox class provides a special message box that
 * supports for asking user to select multi-choices.
 */
class KNSelectionMessageBox : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSelectionMessageBox widget.
     * \param options All the options will be displayed.
     * \param parent The parent widget.
     */
    explicit KNSelectionMessageBox(const QStringList &options,
                                   QWidget *parent = 0);

    /*!
     * \brief Get the selection using the selection message box.
     * \param title The dialog title.
     * \param hintText The hint text to be display.
     * \param options The options text.
     * \return If user select cancel, return -1. Or else it will return the
     * index of the selection in the option list.
     */
    static int getSelection(const QString &title,
                            const QString &hintText,
                            const QStringList &options);

    /*!
     * \brief Get the message box selected item index.
     * \return The selected item index. If no item is selected, return -1.
     */
    int selectedIndex() const;

signals:

public slots:
    /*!
     * \brief Set the hint display text.
     * \param hintText The hint text string.
     */
    void setHintText(const QString &hintText);

protected:
    /*!
     * \brief Reimplemented from KNMessageBox::okayPressed().
     */
    bool okayPressed() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMessageBox::cancelPressed().
     */
    void cancelPressed() Q_DECL_OVERRIDE;

private:
    QLabel *m_hintText;
    QButtonGroup *m_optionGroup;
    int m_selection;
};

#endif // KNSELECTIONMESSAGEBOX_H
