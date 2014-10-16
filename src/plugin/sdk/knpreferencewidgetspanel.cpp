/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QBoxLayout>

//All kinds of items.
#include "preference/knpreferenceitemswitcher.h"

#include "knpreferencewidgetspanel.h"

KNPreferenceWidgetsPanel::KNPreferenceWidgetsPanel(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(20,0,20,20);

    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Add strectch at the end of the layout.
    m_mainLayout->addStretch();

    //Initial the title font.
    m_titleFont=font();
    m_titleFont.setPixelSize(20);

    //Initial the title palette.
    m_titlePalette=palette();
    m_titlePalette.setColor(QPalette::WindowText, QColor(255,255,255));
}

void KNPreferenceWidgetsPanel::addTitle(const QString &titleText)
{
    //Initial the title widget.
    QLabel *titleWidget=new QLabel(this);
    //Set properties.
    titleWidget->setContentsMargins(5,10,0,0);
    titleWidget->setFont(m_titleFont);
    titleWidget->setPalette(m_titlePalette);
    //Set text.
    titleWidget->setText(titleText);
    //Save the title
    m_titles.append(titleWidget);
    //Add title widget.
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               titleWidget);
}

void KNPreferenceWidgetsPanel::addItem(const int &index,
                                       const QString &caption,
                                       const bool &isAdvanced,
                                       const QVariant &value,
                                       const QVariant &defaultValue)
{
    KNPreferenceItemBase *item;
    switch(index)
    {
    case Switcher:
        //Generate a switcher.
        item=new KNPreferenceItemSwitcher(this);
        break;
    }
    //Set properties.
    item->setCaption(caption);
    item->setAdvanced(isAdvanced);
    item->setValue(value);
    item->setDefaultValue(defaultValue);
    //Add the preference widget.
    addPreferenceWidget(item);
}

void KNPreferenceWidgetsPanel::addPreferenceWidget(KNPreferenceItemBase *widget)
{
    //Add widget to layout.
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               widget);
    //Save the widget.
    m_widgets.append(widget);
}

void KNPreferenceWidgetsPanel::setTitle(const int &index, const QString &title)
{
    m_titles.at(index)->setText(title);
}
