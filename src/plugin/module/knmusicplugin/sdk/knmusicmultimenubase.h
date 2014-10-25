/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICMULTIMENUBASE_H
#define KNMUSICMULTIMENUBASE_H

#include "knanimationmenu.h"

class KNMusicMultiMenuBase : public KNAnimationMenu
{
    Q_OBJECT
public:
    KNMusicMultiMenuBase(QWidget *parent = 0):KNAnimationMenu(parent){}

signals:
    void requireDeleteSelection();

public slots:

};

#endif // KNMUSICMULTIMENUBASE_H
