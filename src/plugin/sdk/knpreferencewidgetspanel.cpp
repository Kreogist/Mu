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

#include <QDebug>

KNPreferenceWidgetsPanel::KNPreferenceWidgetsPanel(QWidget *parent) :
    QScrollArea(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    //Initial the container.
    m_container=new QWidget(this);
    //Set container properties.
    m_container->setContentsMargins(0,0,0,0);
    setWidget(m_container);

    //Initial the layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    m_container->setLayout(m_mainLayout);

    //Add strectch at the end of the layout.
    m_mainLayout->addStretch();

    //Initial the title font.
    m_titleFont=font();
    m_titleFont.setPixelSize(15);

    //Initial the title palette.
    m_titlePalette=palette();
    m_titlePalette.setColor(QPalette::WindowText, QColor(150,150,150));
}

void KNPreferenceWidgetsPanel::addTitle(const QString &titleText,
                                        const bool &isAdvanced)
{
    //Initial the title widget.
    QLabel *titleWidget=new QLabel(this);
    //Set properties.
    titleWidget->setContentsMargins(25,10,0,5);
    titleWidget->setFont(m_titleFont);
    titleWidget->setPalette(m_titlePalette);
    if(isAdvanced)
    {
        setAdvancedItem(titleWidget);
    }
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
                                       const QVariant &value,
                                       const QVariant &defaultValue,
                                       const bool &isAdvanced)
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
    item->setValue(value);
    item->setDefaultValue(defaultValue);
    if(isAdvanced)
    {
        setAdvancedItem(item);
    }
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

bool KNPreferenceWidgetsPanel::advancedMode() const
{
    return m_advancedMode;
}

void KNPreferenceWidgetsPanel::setAdvancedMode(bool advancedMode)
{
    m_advancedMode=advancedMode;
    emit requireSetAdvancedVisible(m_advancedMode);
}

void KNPreferenceWidgetsPanel::setAdvancedItem(QWidget *item)
{
    connect(this, &KNPreferenceWidgetsPanel::requireSetAdvancedVisible,
            item, &QWidget::setVisible);
    item->setVisible(m_advancedMode);
}

void KNPreferenceWidgetsPanel::setTitle(const int &index, const QString &title)
{
    m_titles.at(index)->setText(title);
}

void KNPreferenceWidgetsPanel::setNormalMode(bool normalMode)
{
    setAdvancedMode(!normalMode);
}
