/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QApplication>
#include <QLabel>

#include "knpreferenceitem.h"

//All kinds of item.
#include "preference/knpreferenceitemswitcher.h"
#include "preference/knpreferenceitemlineedit.h"
#include "preference/knpreferenceitempathbrowser.h"
#include "preference/knpreferenceitemnumber.h"
#include "preference/knpreferenceitemfont.h"

#include "knpreferenceitemfactory.h"

QPalette KNPreferenceItemFactory::m_titlePalette=QPalette();
QFont KNPreferenceItemFactory::m_titleFont=QFont();

KNPreferenceItem *KNPreferenceItemFactory::create(int type,
                                                  const QString &valueName,
                                                  KNConfigure *configure,
                                                  const QVariant &defaultValue)
{
    //Generate the item according to the type.
    KNPreferenceItem *item=nullptr;
    switch(type)
    {
    case SwitcherItem:
        //Generate a switcher.
        item=new KNPreferenceItemSwitcher;
        break;
    case NumberItem:
        //Generate a number slider.
        item=new KNPreferenceItemNumber;
        break;
    case LineEditItem:
        //Generate a line editor.
        item=new KNPreferenceItemLineEdit;
        break;
    case PathEditItem:
        //Generate a path editor.
        item=new KNPreferenceItemPathBrowser;
        break;
    case FontItem:
        item=new KNPreferenceItemFont;
        break;
    }
    //Check whether we generate the item or not.
    if(item==nullptr)
    {
        return item;
    }
    //Set properties.
    item->setValueName(valueName);
    item->setConfigure(configure);
    if(!defaultValue.isNull())
    {
        item->setDefaultValue(defaultValue);
    }
    return item;
}

QLabel *KNPreferenceItemFactory::createTitle(const QString &caption)
{
    //Initial the label and set the properties.
    QLabel *titleLabel=new QLabel(caption);
    titleLabel->setContentsMargins(25,10,0,5);
    titleLabel->setFont(m_titleFont);
    titleLabel->setPalette(m_titlePalette);
    return titleLabel;
}

void KNPreferenceItemFactory::setTitleFont(const QFont &font)
{
    m_titleFont=font;
}

void KNPreferenceItemFactory::setTitlePalette(const QPalette &palette)
{
    m_titlePalette=palette;
}

void KNPreferenceItemFactory::initialResource()
{
    //Initial the fonts.
    m_titleFont=QApplication::font();
    m_titleFont.setPixelSize(15);

    //Initial the palette.
    m_titlePalette=QApplication::palette();
    m_titlePalette.setColor(QPalette::WindowText, QColor(150,150,150));
}
