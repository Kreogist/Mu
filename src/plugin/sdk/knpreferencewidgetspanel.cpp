/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QLabel>
#include <QBoxLayout>

#include "knglobal.h"
#include "preference/knpreferenceitembase.h"

#include "knpreferencewidgetspanel.h"

#include <QDebug>

KNPreferenceWidgetsPanel::KNPreferenceWidgetsPanel(QWidget *parent) :
    QScrollArea(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setWidgetResizable(true);

    //Initial global instance.
    m_global=KNGlobal::instance();
    m_preferenceItemGlobal=KNPreferenceItemGlobal::instance();

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
    m_mainLayout->insertWidget(m_mainLayout->count()-1, item);
}

void KNPreferenceWidgetsPanel::insertItemInfoList(const KNPreferenceTitleInfo &listTitle,
                                                  const QList<KNPreferenceItemInfo> &list)
{
    //If the info list has contains the title, means that it has been updated before.
    //Update the item list.
    if(m_titleList.contains(listTitle.titleIdentifier))
    {
        updateItemFromInfoList(listTitle, list);
        return;
    }
    //Or else this is a new list, add to list and genreate controls.
    generateItemFromInfoList(listTitle, list);
}

void KNPreferenceWidgetsPanel::savePanelData()
{
    for(auto i=m_itemList.begin();
        i!=m_itemList.end();
        ++i)
    {
        m_global->setCustomData(m_panelName,
                                (*i)->valueName(),
                                (*i)->value());
    }
}

QString KNPreferenceWidgetsPanel::panelName() const
{
    return m_panelName;
}

void KNPreferenceWidgetsPanel::setPanelName(const QString &panelCaption)
{
    m_panelName = panelCaption;
}

void KNPreferenceWidgetsPanel::updateItemValue(const QString &valueName)
{
    //Find the item.
    KNPreferenceItemBase *currentItem=m_itemList.value(valueName, nullptr);
    //If we find the item successful, update the value.
    if(currentItem!=nullptr)
    {
        currentItem->setValue(m_global->customData(m_panelName,
                                                   currentItem->valueName(),
                                                   currentItem->value()));
    }
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

KNPreferenceItemBase *KNPreferenceWidgetsPanel::itemFromValueName(const QString &valueName)
{
    return m_itemList.value(valueName, nullptr);
}

void KNPreferenceWidgetsPanel::generateItemFromInfoList(const KNPreferenceTitleInfo &listTitle,
                                                        const QList<KNPreferenceItemInfo> &list)
{
    //Generate the title first.
    QLabel *currentTitle=m_preferenceItemGlobal->generateLabel(listTitle.title);
    //Add the title to list.
    m_titleList.insert(listTitle.titleIdentifier, currentTitle);
    addTitle(currentTitle, listTitle.advanced);

    //Generate the items.
    for(QList<KNPreferenceItemInfo>::const_iterator i=list.begin();
        i!=list.end();
        ++i)
    {
        //Generate the item via item info.
        KNPreferenceItemBase *currentItem=m_preferenceItemGlobal->generateItem((*i));
        //Add the item to the list.
        m_itemList.insert((*i).valueName, currentItem);
        addItem(currentItem, (*i).advanced);
    }
}

void KNPreferenceWidgetsPanel::updateItemFromInfoList(const KNPreferenceTitleInfo &listTitle,
                                                      const QList<KNPreferenceItemInfo> &list)
{
    //Get the title.
    QLabel *currentTitle=m_titleList.value(listTitle.titleIdentifier);
    //Update the title.
    currentTitle->setText(listTitle.title);

    //Update the item in the list.
    for(QList<KNPreferenceItemInfo>::const_iterator i=list.begin();
        i!=list.end();
        ++i)
    {
        //Update the caption.
        m_itemList.value((*i).valueName)->setCaption((*i).title);
    }
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
