/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICDETAILDIALOGBASE_H
#define KNMUSICDETAILDIALOGBASE_H

#include <QObject>

class QDialog;
class KNMusicDetailDialogBase : public QObject
{
    Q_OBJECT
public:
    KNMusicDetailDialogBase(QObject *parent = 0):QObject(parent){}

signals:

public slots:
    virtual void showDialog(const QString &filePath)=0;

};

#endif // KNMUSICDETAILDIALOGBASE_H
