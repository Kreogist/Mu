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
    KNPreferenceItem(parent)
{
    m_switcher=new KNAnimeColorSwitcher(this);
    connect(m_switcher, &KNAnimeColorSwitcher::valueChanged,
            this, &KNPreferenceItemSwitcher::onActionValueChange);
    //Add the switcher to layout.
    mainLayout()->addWidget(m_switcher);
    mainLayout()->addStretch();
}

void KNPreferenceItemSwitcher::setDefaultValue(const QVariant &defaultValue)
{
    //Set the default value.
    KNPreferenceItem::setDefaultValue(defaultValue.toBool());
}

void KNPreferenceItemSwitcher::setValue(const QVariant &value)
{
    //Save the value.
    bool switcherValue=value.toBool();
    m_switcher->setValue(switcherValue);
    //Set the value.
    KNPreferenceItem::setValue(switcherValue);
}

void KNPreferenceItemSwitcher::onActionValueChange(bool value)
{
    setValue(value);
}
