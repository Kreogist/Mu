/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QBoxLayout>
#include <QSignalMapper>

#include "knroundanimebutton.h"

#include "knroundbuttonbar.h"

KNRoundButtonBar::KNRoundButtonBar(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Initial layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //All the button will be placed at right.
    m_mainLayout->addStretch();

    //Initial the button signal mapper.
    m_toolbarMappper=new QSignalMapper(this);
    connect(m_toolbarMappper, SIGNAL(mapped(int)),
            this, SIGNAL(buttonClicked(int)));
}

KNRoundButtonBar::~KNRoundButtonBar()
{

}

void KNRoundButtonBar::addButton(KNRoundAnimeButton *button)
{
    //Add the button to the layout.
    m_mainLayout->addWidget(button);
    //Link the click signal to toolbar signal mapper.
    connect(button, SIGNAL(clicked()), m_toolbarMappper, SLOT(map()));
    //Set the mapping.
    m_toolbarMappper->setMapping(button, m_buttonIndex);
    m_buttonIndex++;
}

void KNRoundButtonBar::addButton(const QPixmap &icon)
{
    //Generate a button for the icon.
    KNRoundAnimeButton *button=new KNRoundAnimeButton(this);
    button->setIcon(icon);
    //Add the button to the layout.
    addButton(button);
}

