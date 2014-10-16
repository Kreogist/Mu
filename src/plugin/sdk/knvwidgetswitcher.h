/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNVWIDGETSWITCHER_H
#define KNVWIDGETSWITCHER_H

#include "knwidgetswitcher.h"

class KNVWidgetSwitcher : public KNWidgetSwitcher
{
    Q_OBJECT
public:
    explicit KNVWidgetSwitcher(QWidget *parent = 0);

signals:

public slots:

protected:
    void updateAnimationPosition();
};

#endif // KNVWIDGETSWITCHER_H
