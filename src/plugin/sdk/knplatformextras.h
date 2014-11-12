/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNPLATFORMEXTRAS_H
#define KNPLATFORMEXTRAS_H

#include <QObject>

class QMainWindow;
class KNPlatformExtras : public QObject
{
    Q_OBJECT
public:
    KNPlatformExtras(QObject *parent = 0):QObject(parent){}
    virtual void setMainWindow(QMainWindow *mainWindow)=0;

signals:
    void requirePlay();
    void requirePause();
    void requirePlayNext();
    void requirePlayPrev();

public slots:
    virtual void setButtonIcon(const int &index, const QPixmap &icon)=0;
    virtual void onActionPlayStateChanged(const bool &isPlay)=0;
};

#endif // KNPLATFORMEXTRAS_H
