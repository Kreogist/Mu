/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPREFERENCEITEMSWITCHER_H
#define KNPREFERENCEITEMSWITCHER_H

#include "knpreferenceitembase.h"

class KNPreferenceItemSwitcher : public KNPreferenceItemBase
{
    Q_OBJECT
public:
    explicit KNPreferenceItemSwitcher(QWidget *parent = 0);
    QVariant defaultValue() const;
    QVariant value() const;

signals:

public slots:
    void setDefaultValue(const QVariant &defaultValue);
    void setValue(const QVariant &value);

private:
    bool m_defaultValue=false, m_value=false;
};

#endif // KNPREFERENCEITEMSWITCHER_H
