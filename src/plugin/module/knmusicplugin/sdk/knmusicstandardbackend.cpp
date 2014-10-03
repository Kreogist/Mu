/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "knmusicbackendthread.h"

#include "knmusicstandardbackend.h"

KNMusicStandardBackend::KNMusicStandardBackend(QObject *parent) :
    KNMusicBackend(parent)
{
    ;
}

void KNMusicStandardBackend::setMainThread(KNMusicBackendThread *thread)
{
    connect(thread, &KNMusicBackendThread::positionChanged,
            this, &KNMusicStandardBackend::positionChanged);
    connect(thread, &KNMusicBackendThread::durationChanged,
            this, &KNMusicStandardBackend::durationChanged);
    connect(thread, &KNMusicBackendThread::finished,
            this, &KNMusicStandardBackend::finished);
    connect(thread, &KNMusicBackendThread::stopped,
            this, &KNMusicStandardBackend::stopped);
    connect(thread, &KNMusicBackendThread::stateChanged,
            this, &KNMusicStandardBackend::playingStateChanged);
}

void KNMusicStandardBackend::setPreviewThread(KNMusicBackendThread *thread)
{
    connect(thread, &KNMusicBackendThread::positionChanged,
            this, &KNMusicStandardBackend::previewPositionChanged);
    connect(thread, &KNMusicBackendThread::durationChanged,
            this, &KNMusicStandardBackend::previewDurationChanged);
    connect(thread, &KNMusicBackendThread::finished,
            this, &KNMusicStandardBackend::previewFinished);
    connect(thread, &KNMusicBackendThread::stopped,
            this, &KNMusicStandardBackend::previewStopped);
    connect(thread, &KNMusicBackendThread::stateChanged,
            this, &KNMusicStandardBackend::previewPlayingStateChanged);
}
