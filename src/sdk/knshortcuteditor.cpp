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
#include <QKeyEvent>
#include <QLineEdit>

#include "knshortcuteditor.h"
#include "knthememanager.h"

#include <QDebug>

KNShortcutEditor::KNShortcutEditor(QWidget *parent) :
    QKeySequenceEdit(parent),
    m_editing(false)
{
    //Set the properties.
    setFocusPolicy(Qt::StrongFocus);
    //Get the line editor.
    //Find the line edit from the children.
    for(auto i : children())
    {
        //Check the object's name.
        if(i->objectName()=="qt_keysequenceedit_lineedit")
        {
            //Get the line edit.
            QLineEdit *lineEdit=static_cast<QLineEdit *>(i);
            //Change the place holder text.
            lineEdit->setPlaceholderText(tr("Press your shortcuts"));
            //Configure the palette of the line edit.
            lineEdit->setPalette(knTheme->getPalette("PreferenceItemSpinBox"));
        }
    }
}

void KNShortcutEditor::startEditing()
{
    //Set the edit flag.
    m_editing=true;
}

void KNShortcutEditor::keyPressEvent(QKeyEvent *event)
{
    //When the editor is not editing, ignore all the key pressed event.
    if(!m_editing)
    {
        //Ignore the event.
        event->ignore();
        return;
    }
    //Checking for escape.
    if(event->modifiers()==Qt::NoModifier && event->key()==Qt::Key_Escape)
    {
        //Cancel the editing.
        cancelEdit();
        //Ignore the edit event.
        event->ignore();
        return;
    }
    //Checking for removing.
    if(event->modifiers()==Qt::NoModifier && event->key()==Qt::Key_Backspace)
    {
        //Ignore the edit event.
        event->ignore();
        //Emit the clear signal.
        emit clearShortcut();
        return;
    }
    //Do original event.
    QKeySequenceEdit::keyPressEvent(event);
}

void KNShortcutEditor::cancelEdit()
{
    //Set the editing state to false.
    m_editing=false;
    //Emit the cancel signal.
    emit cancel();
}

bool KNShortcutEditor::isEditing() const
{
    return m_editing;
}
