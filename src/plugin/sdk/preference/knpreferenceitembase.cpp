/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QBoxLayout>

#include "knpreferenceitembase.h"

#define ItemFixedHeight 27

KNPreferenceItemBase::KNPreferenceItemBase(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFixedHeight(ItemFixedHeight);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the label.
    m_caption=new QLabel(this);

    //Add widget to layout.
    mainLayout->addWidget();
    mainLayout->addWidget(m_caption, 1);
}

bool KNPreferenceItemBase::isAdvanced() const
{
    return m_isAdvanced;
}

QString KNPreferenceItemBase::caption() const
{
    return m_caption->text();
}

void KNPreferenceItemBase::setAdvanced(const bool &isAdvanced)
{
    m_isAdvanced = isAdvanced;
}

void KNPreferenceItemBase::setCaption(const QString &caption)
{
    m_caption->setText(caption);
}
