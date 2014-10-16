/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knpreferenceitemswitcher.h"

KNPreferenceItemSwitcher::KNPreferenceItemSwitcher(QWidget *parent) :
    KNPreferenceItemBase(parent)
{
}

QVariant KNPreferenceItemSwitcher::defaultValue() const
{
    return m_defaultValue;
}

QVariant KNPreferenceItemSwitcher::value() const
{
    return m_value;
}

void KNPreferenceItemSwitcher::setDefaultValue(const QVariant &defaultValue)
{
    m_defaultValue=defaultValue.toBool();
}

void KNPreferenceItemSwitcher::setValue(const QVariant &value)
{
    m_value=value.toBool();
}
