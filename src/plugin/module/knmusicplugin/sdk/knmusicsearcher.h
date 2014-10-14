/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICSEARCHER_H
#define KNMUSICSEARCHER_H

#include "knfilesearcher.h"

class KNMusicGlobal;
class KNMusicSearcher : public KNFileSearcher
{
    Q_OBJECT
public:
    explicit KNMusicSearcher(QObject *parent = 0);

signals:

public slots:

protected:
    bool isSuffixAccept(const QString &suffix);

private:
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICSEARCHER_H
