/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNMUSICGLOBAL_H
#define KNMUSICGLOBAL_H

#include <QImage>
#include <QPixmap>
#include <QMap>
#include <QDateTime>
#include <QVariant>
#include <QStringList>

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
struct KNMusicListTrackDetailInfo
{
    //Track index.
    int index;
    //Track time.
    qint64 startPosition=-1;
    qint64 trackDuration=-1;
    //Metadata map.
    QMap<int, QString> metaData;
};
struct KNMusicListDetailInfo
{
    //Properties.
    QString musicFilePath;
    //Metadata map.
    QMap<int, QString> metaData;
    //Track list.
    QList<KNMusicListTrackDetailInfo> trackList;
};
struct KNMusicDetailInfo
{
    //Properties.
    QString fileName;
    QString filePath;
    quint64 size;
    QDateTime dateModified;
    QDateTime lastPlayed;
    QDateTime dateAdded;
    //Music properties.
    qint64 startPosition=-1;
    qint64 duration=0;
    qint64 bitRate=0;
    qint64 samplingRate=0;
    //Album art data.
    QImage coverImage;
    QString coverImageHash;
    QMap<QString, QList<QByteArray>> imageData;
    //Tag datas.
    QString textLists[MusicDataCount];
    int rating=0;
};
}

using namespace KNMusic;

class QLabel;
class QThread;
class KNPreferenceItemBase;
class KNPreferenceWidgetsPanel;
class KNMusicParser;
class KNMusicNowPlayingBase;
class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    ~KNMusicGlobal();
    static QString msecondToString(const qint64 &msecond);
    static QString dateTimeToString(const QDateTime &dateTime);
    static QString dateTimeToDataString(const QDateTime &dateTime);
    static QDateTime dataStringToDateTime(const QString &text);
    static KNMusicParser *parser();
    static void setParser(KNMusicParser *parser);
    static KNMusicNowPlayingBase *nowPlaying();
    static void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);
    bool isMusicFile(const QString &suffix);
    bool isMusicListFile(const QString &suffix);
    QString typeDescription(const QString &suffix) const;
    QPixmap noAlbumArt() const;
    QThread *searchThread();
    QThread *analysisThread();
    void setNoAlbumArt(const QPixmap &noAlbumArt);
    QString treeViewHeaderText(const int &index);
    void setPreferencePanel(KNPreferenceWidgetsPanel *preferencePanel);
    void addTitle(QLabel *title,
                  const bool &isAdvanced=false);
    void addItem(KNPreferenceItemBase *item,
                 const bool &isAdvanced=false);

signals:

public slots:
    void retranslate();

private:
    void regMetaType();
    void initialFileType();
    void initialThreads();
    void initialHeaderText();
    static KNMusicGlobal *m_instance;
    static KNMusicParser *m_parser;
    static KNMusicNowPlayingBase *m_nowPlaying;
    explicit KNMusicGlobal(QObject *parent = 0);
    QString m_treeViewHeaderText[MusicDisplayDataCount];
    QStringList m_suffixs, m_listSuffixs,
                    m_suffixDescription, m_listSuffixDescription,
                    m_indexedGenres;
    QPixmap m_noAlbumArt;
    QThread *m_searcherThread, *m_analysisThread;
    KNPreferenceWidgetsPanel *m_preferencePanel;
};

#endif // KNMUSICGLOBAL_H
