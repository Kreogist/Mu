/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
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
