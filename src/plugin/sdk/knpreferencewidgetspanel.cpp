/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QBoxLayout>

//All kinds of items.
#include "preference/knpreferenceitemglobal.h"
#include "preference/knpreferenceitemswitcher.h"
#include "preference/knpreferenceitemlineedit.h"

#include "knpreferencewidgetspanel.h"

#include <QDebug>

using namespace KNPreferenceItemGlobal;

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

void KNPreferenceWidgetsPanel::addTitle(QLabel *titleWidget,
                                        const bool &isAdvanced)
{
    //Change parent relationship.
    titleWidget->setParent(this);
    //Check is advanced item. If yes, linked to advanced signal.
    if(isAdvanced)
    {
        setAdvancedItem(titleWidget);
    }
    //Add title widget.
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               titleWidget);
}

void KNPreferenceWidgetsPanel::addItem(KNPreferenceItemBase *item,
                                       const bool &isAdvanced)
{
    //Change parent relationship.
    item->setParent(this);
    //Check is advacned item. If yes, linked to advanced signal.
    if(isAdvanced)
    {
        setAdvancedItem(item);
    }
    //Add widget to layout.
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               item);
}

QString KNPreferenceWidgetsPanel::panelCaption() const
{
    return m_panelCaption;
}

void KNPreferenceWidgetsPanel::setPanelCaption(const QString &panelCaption)
{
    m_panelCaption = panelCaption;
}

bool KNPreferenceWidgetsPanel::advancedMode() const
{
    return m_advancedMode;
}

KNPreferenceItemBase *KNPreferenceWidgetsPanel::generateItem(const int &index,
                                                             const QString &valueName,
                                                             const QVariant &value,
                                                             const QVariant &defaultValue)
{
    KNPreferenceItemBase *item;
    switch(index)
    {
    case Switcher:
        //Generate a switcher.
        item=new KNPreferenceItemSwitcher;
        break;
    case LineEdit:
        //Generate a line editor.
        item=new KNPreferenceItemLineEdit;
        break;
    }
    //Set properties.
    item->setValueName(valueName);
    item->setDefaultValue(defaultValue);
    item->setValue(value.isNull()?defaultValue:value);
    //Return the item.
    return item;
}

QLabel *KNPreferenceWidgetsPanel::generateLabel(const QString &caption)
{
    //Initial the title widget.
    QLabel *titleWidget=new QLabel;
    //Set properties.
    titleWidget->setContentsMargins(25,10,0,5);
    titleWidget->setFont(m_titleFont);
    titleWidget->setPalette(m_titlePalette);
    titleWidget->setText(caption);
    //Return the widget.
    return titleWidget;
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

void KNPreferenceWidgetsPanel::setNormalMode(bool normalMode)
{
    setAdvancedMode(!normalMode);
}
