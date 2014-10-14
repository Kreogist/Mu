/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusictagpraser.h"

KNMusicTagParser::KNMusicTagParser(QObject *parent) :
    QObject(parent)
{

}

void KNMusicTagParser::setTextData(QString &destination, const QString &source)
{
    if(!source.isEmpty())
    {
        destination=source;
    }
}
