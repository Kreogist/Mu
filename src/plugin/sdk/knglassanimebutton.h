/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNGLASSANIMEBUTTON_H
#define KNGLASSANIMEBUTTON_H

#include "knopacityanimebutton.h"

class KNGlassAnimeButton : public KNOpacityAnimeButton
{
    Q_OBJECT
public:
    explicit KNGlassAnimeButton(QWidget *parent = 0);
    ~KNGlassAnimeButton();

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // KNGLASSANIMEBUTTON_H
