/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICCATEGORYTABWIDGET_H
#define KNMUSICCATEGORYTABWIDGET_H

#include "kncategorytabwidget.h"

class QPropertyAnimation;
class KNMusicCategoryTabWidget : public KNCategoryTabWidget
{
public:
    explicit KNMusicCategoryTabWidget(QWidget *parent = 0);
    ~KNMusicCategoryTabWidget();
    QWidget *mainPlayer() const;
    void setMainPlayer(QWidget *mainPlayer);

public slots:
    void showMainPlayer();
    void hideMainPlayer();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    inline void configureAnimation(QPropertyAnimation *anime);
    QWidget *m_mainPlayer=nullptr;
    QPropertyAnimation *m_mainPlayerIn, *m_mainPlayerOut;
};

#endif // KNMUSICCATEGORYTABWIDGET_H
