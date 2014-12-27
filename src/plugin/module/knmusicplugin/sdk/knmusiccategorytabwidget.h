/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICCATEGORYTABWIDGET_H
#define KNMUSICCATEGORYTABWIDGET_H

#include "kncategorytabwidget.h"

class KNMusicCategoryTabWidget : public KNCategoryTabWidget
{
public:
    explicit KNMusicCategoryTabWidget(QWidget *parent = 0);
    ~KNMusicCategoryTabWidget();
    QWidget *mainPlayer() const;
    void setMainPlayer(QWidget *mainPlayer);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QWidget *m_mainPlayer=nullptr;
};

#endif // KNMUSICCATEGORYTABWIDGET_H
