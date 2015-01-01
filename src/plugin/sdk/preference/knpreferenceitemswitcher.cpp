/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QBoxLayout>
#include <QLineEdit>

#include "knanimecolorswitcher.h"

#include "knpreferenceitemswitcher.h"

KNPreferenceItemSwitcher::KNPreferenceItemSwitcher(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
    m_switcher=new KNAnimeColorSwitcher(this);
    //Add the control widget.
    insertStretch();
    insertWidget(m_switcher);
}

QVariant KNPreferenceItemSwitcher::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemSwitcher::value() const
{
    return m_switcher->value();
}

void KNPreferenceItemSwitcher::setDefaultValue(const QVariant &defaultValue)
{
    //Save the default value.
    m_defaultValue=defaultValue.toBool();
    //Set the value.
    m_switcher->setValue(m_defaultValue);
}

void KNPreferenceItemSwitcher::setValue(const QVariant &value)
{
    m_switcher->setValue(value.toBool());
}
