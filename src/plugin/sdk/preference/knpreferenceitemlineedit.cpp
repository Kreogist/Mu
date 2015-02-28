/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLineEdit>
#include <QBoxLayout>

#include "knpreferenceitemlineedit.h"

KNPreferenceItemLineEdit::KNPreferenceItemLineEdit(QWidget *parent) :
    KNPreferenceItem(parent)
{
    //Initial editor.
    m_editor=new QLineEdit(this);
    m_editor->setMinimumWidth(260);

    //Add editor to layout.
    mainLayout()->addWidget(m_editor, 1);
}

void KNPreferenceItemLineEdit::setValue(const QVariant &value)
{
    //Simply set the text.
    QString text=value.toString();
    m_editor->setText(text);
    //Set the value.
    KNPreferenceItem::setValue(text);
}

void KNPreferenceItemLineEdit::setDefaultValue(const QVariant &defaultValue)
{
    KNPreferenceItem::setDefaultValue(defaultValue.toString());
}
