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
#include <QMouseEvent>

#include "kncancellineedit.h"

#include "kneditablelabel.h"

KNEditableLabel::KNEditableLabel(QWidget *parent) :
    QLabel(parent),
    m_editor(new KNCancelLineEdit(this))
{
    //Configure the editor.
    m_editor->setFrame(false);
    m_editor->hide();
    //Link the editor's signal.
    connect(m_editor, &KNCancelLineEdit::editingEnsure,
            [=]
            {
                //Set the data of the editor.
                setText(m_editor->text());
                //Hide the editor.
                hideEdior();
                //Emit content changed signal.
                emit contentChanged();
            });
    connect(m_editor, &KNCancelLineEdit::editingCancel,
            this, &KNEditableLabel::hideEdior);
}

bool KNEditableLabel::isEditing() const
{
    return m_editor->isVisible();
}

void KNEditableLabel::mousePressEvent(QMouseEvent *event)
{
    //Do the previous pressed event.
    QLabel::mousePressEvent(event);
    //Mark mouse down.
    m_pressed=true;
}

void KNEditableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original release event.
    QLabel::mouseReleaseEvent(event);
    //Check has been mark or not.
    if(m_pressed)
    {
        //Set mark back to false.
        m_pressed=false;
        //If mouse release in the widget, show editor.
        if(rect().contains(event->pos()))
        {
            showEditor();
        }
    }
}

void KNEditableLabel::showEditor()
{
    //Configure the editor.
    m_editor->setEnabled(true);
    m_editor->resize(size());
    m_editor->setAlignment(alignment());
    m_editor->setTextMargins(0,0,0,0);
    m_editor->setText(text());
    m_editor->selectAll();
    //Show edit widget.
    m_editor->show();
    //Set focus on the editor.
    m_editor->setFocus();
}

void KNEditableLabel::hideEdior()
{
    //Disable the editor.
    m_editor->setEnabled(false);
    //Hide edit widget.
    m_editor->hide();
}
