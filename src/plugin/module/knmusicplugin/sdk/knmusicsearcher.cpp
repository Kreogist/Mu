/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicglobal.h"

#include "knmusicsearcher.h"

KNMusicSearcher::KNMusicSearcher(QObject *parent) :
    KNFileSearcher(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
}

bool KNMusicSearcher::isSuffixAccept(const QString &suffix)
{
    return m_musicGlobal->isMusicFile(suffix) ||
            m_musicGlobal->isMusicListFile(suffix);
}
