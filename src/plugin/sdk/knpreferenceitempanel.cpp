/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QBoxLayout>

#include "knconfigure.h"
#include "knpreferenceitem.h"

#include "knpreferenceitempanel.h"

#include <QDebug>

KNPreferenceItemPanel::KNPreferenceItemPanel(QWidget *parent) :
    QScrollArea(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    //Initial the container.
    QWidget *container=new QWidget(this);
    //Set container properties.
    container->setContentsMargins(0,0,0,0);
    setWidget(container);

    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    container->setLayout(m_mainLayout);

    //Add strectch at the end of the layout.
    m_mainLayout->addStretch();
}

void KNPreferenceItemPanel::addTitle(QLabel *titleWidget, bool advanced)
{
    addWidget(titleWidget, advanced);
}

void KNPreferenceItemPanel::addItem(KNPreferenceItem *item, bool advanced)
{
    addWidget(item, advanced);
}

void KNPreferenceItemPanel::addWidget(QWidget *widget, bool advanced)
{
    //Change the parent relationship.
    widget->setParent(this);
    //Check is advacned item. If yes, linked to advanced signal.
    if(advanced)
    {
        setAdvancedWidget(widget);
    }
    //Add widget to layout.
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               widget);
}

bool KNPreferenceItemPanel::advancedMode() const
{
    return m_advancedMode;
}

void KNPreferenceItemPanel::setAdvancedMode(bool advancedMode)
{
    //Save the advanced mode.
    m_advancedMode=advancedMode;
    //Ask to change the mode.
    emit requireSetAdvancedVisible(m_advancedMode);
}

void KNPreferenceItemPanel::setAdvancedWidget(QWidget *item)
{
    //Link the item.
    connect(this, &KNPreferenceItemPanel::requireSetAdvancedVisible,
            item, &QWidget::setVisible);
    //Set the item to current advanced mode.
    item->setVisible(m_advancedMode);
}
