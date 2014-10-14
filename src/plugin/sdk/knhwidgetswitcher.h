/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNHWIDGETSWITCHER_H
#define KNHWIDGETSWITCHER_H

#include "knwidgetswitcher.h"

class KNHWidgetSwitcher : public KNWidgetSwitcher
{
    Q_OBJECT
public:
    explicit KNHWidgetSwitcher(QWidget *parent = 0);

signals:

public slots:

protected:
    void updateAnimationPosition();
};

#endif // KNHWIDGETSWITCHER_H
