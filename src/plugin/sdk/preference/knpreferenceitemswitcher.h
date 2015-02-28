/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMSWITCHER_H
#define KNPREFERENCEITEMSWITCHER_H

#include "knpreferenceitem.h"

class KNAnimeColorSwitcher;
class KNPreferenceItemSwitcher : public KNPreferenceItem
{
    Q_OBJECT
public:
    explicit KNPreferenceItemSwitcher(QWidget *parent = 0);

signals:

public slots:
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

protected:

private:
    KNAnimeColorSwitcher *m_switcher;
};

#endif // KNPREFERENCEITEMSWITCHER_H
