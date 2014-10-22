/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLineEdit>

#include "knpreferenceitemlineedit.h"

KNPreferenceItemLineEdit::KNPreferenceItemLineEdit(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
    m_valueEditor=new QLineEdit(this);
    //Add the control widget.
    insertControlWidget(m_valueEditor);
}

QVariant KNPreferenceItemLineEdit::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemLineEdit::value() const
{
    return m_valueEditor->text();
}

void KNPreferenceItemLineEdit::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue=defaultValue.toString();
    //When default value changed, set the value to the default value.
    setValue(m_defaultValue);
}

void KNPreferenceItemLineEdit::setValue(const QVariant &value)
{
    m_valueEditor->setText(value.toString());
}
