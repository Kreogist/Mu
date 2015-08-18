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

#ifndef KNCANCELLINEEDIT_H
#define KNCANCELLINEEDIT_H

#include <QLineEdit>

/*!
 * \brief The KNCancelLineEdit class provides you a line edit which can noticed
 * ESC or enter key pressed.
 */
class KNCancelLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNCancelLineEdit widget.
     * \param parent The parent widget of the cancel line edit.
     */
    explicit KNCancelLineEdit(QWidget *parent = 0);

signals:
    /*!
     * \brief When the enter or return key pressed, this signal will be emitted.
     */
    void editingEnsure();

    /*!
     * \brief When the ESC key pressed, this signal will be emitted.
     */
    void editingCancel();

protected:
    /*!
     * \brief Reimplemented from QLineEdit::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event);

    /*!
     * \brief Reimplemented from QLineEdit::keyReleaseEvent().
     */
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // KNCANCELLINEEDIT_H
