/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knffmpegglobal.h"

KNFFMpegGlobal *KNFFMpegGlobal::m_instance=nullptr;

KNFFMpegGlobal *KNFFMpegGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNFFMpegGlobal:m_instance;
}

KNFFMpegGlobal::KNFFMpegGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the FFMpeg.
    //Initialize all the muxers, demuxers and protocols of FFMpeg.
    av_register_all();
}
