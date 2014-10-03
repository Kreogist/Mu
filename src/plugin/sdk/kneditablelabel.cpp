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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QMouseEvent>

#include "kncancellineedit.h"

#include "kneditablelabel.h"

KNEditableLabel::KNEditableLabel(QWidget *parent) :
    QLabel(parent)
{
    //Initial edit widget.
    m_edit=new KNCancelLineEdit(this);
    m_edit->setFrame(false);
    m_edit->hide();
    //Connect editor's finish signal.
    connect(m_edit, &KNCancelLineEdit::editingEnsure,
            this, &KNEditableLabel::onActionEditFinished);
    connect(m_edit, &KNCancelLineEdit::editingCancel,
            this, &KNEditableLabel::onActionEditCancel);
}

void KNEditableLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    //Mark mouse down.
    m_pressed=true;
}

void KNEditableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    //Check has been mark or not.
    if(m_pressed)
    {
        //Set mark back to false.
        m_pressed=false;
        //If mouse release in the widget, show edit widget.
        if(rect().contains(event->pos()))
        {
            showEdit();
        }
    }
}

void KNEditableLabel::onActionEditFinished()
{
    //Set data.
    setText(m_edit->text());
    hideEdit();
    //Emit signal.
    emit editingFinished();
}

void KNEditableLabel::onActionEditCancel()
{
    //Hide the edit.
    hideEdit();
}

void KNEditableLabel::showEdit()
{
    //Set flags.
    m_editing=true;
    //Enable editor and set contents.
    m_edit->setEnabled(true);
    m_edit->resize(size());
    m_edit->setAlignment(alignment());
    m_edit->setTextMargins(0,0,0,0);
    m_edit->setText(text());
    //Show edit widget.
    m_edit->show();
    //Set focus.
    m_edit->setFocus();
}

void KNEditableLabel::hideEdit()
{
    //Set flags.
    m_editing=false;
    //Disable editor.
    m_edit->setEnabled(false);
    //Hide edit widget.
    m_edit->hide();
}

bool KNEditableLabel::editing() const
{
    return m_editing;
}

void KNEditableLabel::setEditing(bool editing)
{
    m_editing=editing;
    if(m_editing)
    {
        showEdit();
        return;
    }
}
