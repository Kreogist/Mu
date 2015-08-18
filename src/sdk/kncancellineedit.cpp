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
#include <QKeyEvent>

#include "kncancellineedit.h"

KNCancelLineEdit::KNCancelLineEdit(QWidget *parent) : QLineEdit(parent)
{
}

void KNCancelLineEdit::focusOutEvent(QFocusEvent *event)
{
    //Do original focus out event.
    QLineEdit::focusOutEvent(event);
    //When lost the focus, it means the user wants to cancel the edit.
    emit editingCancel();
}

void KNCancelLineEdit::keyReleaseEvent(QKeyEvent *event)
{
    //Handle the Escape, Enter and Return key.
    switch(event->key())
    {
    //For ESC key pressed, it means cancel editing.
    case Qt::Key_Escape:
        emit editingCancel();
        return;
    //For Enter and Return, it means ensure editing.
    case Qt::Key_Enter:
    case Qt::Key_Return:
        emit editingEnsure();
        return;
    //For others, just input.
    default:
        QLineEdit::keyReleaseEvent(event);
    }
}

