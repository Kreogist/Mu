/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QLabel>

#include "knpreferenceitembase.h"

//All kinds of item.
#include "knpreferenceitemswitcher.h"
#include "knpreferenceitemlineedit.h"
#include "knpreferenceitempathbrowser.h"

#include "knpreferenceitemglobal.h"

KNPreferenceItemGlobal *KNPreferenceItemGlobal::m_instance=nullptr;

KNPreferenceItemGlobal *KNPreferenceItemGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNPreferenceItemGlobal:m_instance;
}

QLabel *KNPreferenceItemGlobal::generateLabel(const QString &caption)
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

KNPreferenceItemBase *KNPreferenceItemGlobal::generateItem(const int &index,
                                                           const QString &valueName,
                                                           const QVariant &value,
                                                           const QVariant &defaultValue)
{
    KNPreferenceItemBase *item=nullptr;
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
    case PathEdit:
        //Generate a path editor.
        item=new KNPreferenceItemPathBrowser;
        break;
    }
    if(item==nullptr)
    {
        return item;
    }
    //Set properties.
    item->setValueName(valueName);
    item->setDefaultValue(defaultValue);
    item->setValue(value.isNull()?defaultValue:value);
    //Return the item.
    return item;
}

KNPreferenceItemInfo KNPreferenceItemGlobal::generateInfo(int type,
                                                          const QString &title,
                                                          const QString &valueName,
                                                          const QVariant &value,
                                                          const QVariant &defaultValue,
                                                          bool advanced)
{
    KNPreferenceItemInfo currentItem;
    //Set the data.
    currentItem.type=type;
    currentItem.title=title;
    currentItem.valueName=valueName;
    currentItem.value=value;
    currentItem.defaultValue=defaultValue;
    currentItem.advanced=advanced;
    return currentItem;
}

void KNPreferenceItemGlobal::updateTitleCaption(QLabel *title,
                                                const QString &caption)
{
    title->setText(caption);
}

void KNPreferenceItemGlobal::updateItemCaption(KNPreferenceItemBase *item,
                                               const QString &caption)
{
    item->setCaption(caption);
}

QVariant KNPreferenceItemGlobal::itemData(KNPreferenceItemBase *item)
{
    return item->value();
}

KNPreferenceItemGlobal::KNPreferenceItemGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the title font.
    m_titleFont=QApplication::font();
    m_titleFont.setPixelSize(15);

    //Initial the title palette.
    m_titlePalette=QApplication::palette();
    m_titlePalette.setColor(QPalette::WindowText, QColor(150,150,150));
}
