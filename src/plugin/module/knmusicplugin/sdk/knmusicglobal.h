/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QObject>

namespace KNMusic
{
enum MusicDatas
{
    Name,
    Album,
    AlbumArtist,
    AlbumRating,
    Artist,
    BeatsPerMinuate,
    BitRate,
    Category,
    Comments,
    Composer,
    DateAdded,
    DateModified,
    Description,
    DiscCount,
    DiscNumber,
    Genre,
    Grouping,
    Kind,
    LastPlayed,
    Plays,
    Rating,
    SampleRate,
    Size,
    Time,
    TrackCount,
    TrackNumber,
    Year,
    MusicDataCount
};
enum MusicDisplayData
{
    BlankData=MusicDataCount,
    MusicDisplayDataCount
};
enum KNMusicModelRole
{
    FilePathRole=Qt::UserRole,
    FileNameRole,
    StartPositionRole,
    ArtworkKeyRole
};
enum KNMusicPlaybackOrder
{
    NoRepeat,
    RepeatAll,
    RepeatTrack,
    LoopCount
};
enum KNMusicPlayingState
{
    StoppedState,
    PlayingState,
    PausedState
};
}

using namespace KNMusic;

class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    static QString msecondToTime(const qint64 &msecond);

signals:

public slots:

private:
    static KNMusicGlobal *m_instance;
    explicit KNMusicGlobal(QObject *parent = 0);

};

#endif // KNMUSICGLOBAL_H
