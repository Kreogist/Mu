/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSTANDARDBACKEND_H
#define KNMUSICSTANDARDBACKEND_H

#include "knmusicbackend.h"

class KNMusicBackendThread;
class KNMusicStandardBackend : public KNMusicBackend
{
    Q_OBJECT
public:
    explicit KNMusicStandardBackend(QObject *parent = 0);

signals:

public slots:

protected:
    void setMainThread(KNMusicBackendThread *thread);
    void setPreviewThread(KNMusicBackendThread *thread);
};

#endif // KNMUSICSTANDARDBACKEND_H
