/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMFACTORY_H
#define KNPREFERENCEITEMFACTORY_H

#include <QPalette>
#include <QVariant>
#include <QHash>

namespace KNPreferenceItemList
{
enum KNPreferenceItemType
{
    SwitcherItem,
    NumberItem,
    LineEditItem,
    PathEditItem,
    FontItem
};
}

using namespace KNPreferenceItemList;

class QLabel;
class KNConfigure;
class KNPreferenceItem;
class KNPreferenceItemFactory
{
public:
    static KNPreferenceItem *create(int type,
                                    const QString &valueName,
                                    KNConfigure *configure);
    static QLabel *createTitle(const QString &caption=QString());
    static void setTitleFont(const QFont &font);
    static void setTitlePalette(const QPalette &palette);

    static void initialResource();

private:
    static QFont m_titleFont;
    static QPalette m_titlePalette;
};

#endif // KNPREFERENCEITEMFACTORY_H
