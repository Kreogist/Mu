/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNFFMPEGGLOBAL_H
#define KNFFMPEGGLOBAL_H

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

#include <QObject>

class KNFFMpegGlobal : public QObject
{
    Q_OBJECT
public:
    static KNFFMpegGlobal *instance();

signals:

public slots:

private:
    static KNFFMpegGlobal *m_instance;
    explicit KNFFMpegGlobal(QObject *parent = 0);
};

#endif // KNFFMPEGGLOBAL_H
