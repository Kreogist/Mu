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
#include <QStandardItem>

#include "preference/knpreferenceitemglobal.h"

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
    ArtworkKeyRole,
    TrackFileRole,
    TrackIndexRole,
    CantPlayFlagRole
};
enum KNMusicCategoryRole
{
    CategoryItemSizeRole=Qt::UserRole,
    CategoryItemVisibleRole,
    CategoryArtworkKeyRole,
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
enum KNMusicSortFlag
{
    SortByInt,
    SortUserByInt,
    SortUserByFloat,
    SortUserByDate
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
    int rating=0;
    int trackIndex=-1;
    QString fileName;
    QString filePath;
    QString trackFilePath;
    QDateTime dateModified;
    QDateTime lastPlayed;
    QDateTime dateAdded;
    quint64 size;
    //Music properties.
    qint64 startPosition=-1;
    qint64 duration=0;
    qint64 bitRate=0;
    qint64 samplingRate=0;
    //Image hash data.
    QString coverImageHash;
    //Tag datas.
    QString textLists[MusicDataCount];
};
struct KNMusicAnalysisItem
{
    KNMusicDetailInfo detailInfo;
    //Album art data.
    QImage coverImage;
    QMap<QString, QList<QByteArray>> imageData;
};
}

using namespace KNMusic;

class QLabel;
class QThread;
class KNPreferenceItemBase;
class KNPreferenceWidgetsPanel;
class KNConfigure;
class KNGlobal;
class KNMusicParser;
class KNMusicNowPlayingBase;
class KNMusicDetailTooltipBase;
class KNMusicDetailDialogBase;
class KNMusicSoloMenuBase;
class KNMusicMultiMenuBase;
class KNMusicSearchBase;
class KNMusicProxyModel;
class KNMusicTab;
class KNMusicGlobal : public QObject
{
    Q_OBJECT
public:
    static KNMusicGlobal *instance();
    ~KNMusicGlobal();
    static QString msecondToString(const qint64 &msecond);
    static QString dateTimeToString(const QDateTime &dateTime);
    static QString dateTimeToDataString(const QDateTime &dateTime);
    static QString musicRowFormat();
    static QDateTime dataStringToDateTime(const QString &text);
    static KNMusicParser *parser();
    static void setParser(KNMusicParser *parser);
    KNConfigure *musicConfigure();
    KNPreferenceWidgetsPanel *preferencePanel();
    KNMusicNowPlayingBase *nowPlaying();
    void setNowPlaying(KNMusicNowPlayingBase *nowPlaying);
    static KNMusicSoloMenuBase *soloMenu();
    static void setSoloMenu(KNMusicSoloMenuBase *soloMenu);
    static KNMusicMultiMenuBase *multiMenu();
    static void setMultiMenu(KNMusicMultiMenuBase *multiMenu);
    static QString musicLibraryPath();
    static void setMusicLibraryPath(const QString &musicLibraryPath);
    static quint32 charToInt32(char *rawTagData);
    static quint32 inverseCharToInt32(char *rawTagData);
    bool isMusicFile(const QString &suffix);
    bool isMusicListFile(const QString &suffix);
    QString typeDescription(const QString &suffix) const;
    QPixmap noAlbumArt() const;
    QThread *searchThread();
    QThread *analysisThread();
    QThread *lyricsThread();
    void setNoAlbumArt(const QPixmap &noAlbumArt);
    QString treeViewHeaderText(const int &index);
    QString indexedGenre(const int &index);

    static QList<QList<QStandardItem *> > dragMusicRow();
    static void setDragMusicRow(const QList<QList<QStandardItem *> > &dragMusicRow);
    static void clearDragMusicRow();
    static KNMusicSearchBase *musicSearch();
    static void setMusicSearch(KNMusicSearchBase *musicSearch);
    static KNMusicDetailTooltipBase *detailTooltip();
    static void setDetailTooltip(KNMusicDetailTooltipBase *detailTooltip);
    static KNMusicDetailDialogBase *detailDialog();
    static void setDetailDialog(KNMusicDetailDialogBase *detailDialog);
    void updateItemValue(const QString &valueName);
    void insertItemInfoList(const KNPreferenceTitleInfo &listTitle,
                            const QList<KNPreferenceItemInfo> &list);
    bool renameMusicFile(const QString &originalPath,
                         const QString &preferName);

signals:
    void musicLibraryMoved(const QString &originalPath,
                           const QString &currentPath);
    void musicFilePathChanged(const QString &originalPath,
                              const QString &currentPath,
                              const QString &currentFileName);
    void requirePlayMusicRow(KNMusicProxyModel *model,
                             int row,
                             KNMusicTab *tab);

public slots:
    void retranslate();
    void onActionLibraryMoved(const QString &originalPath,
                              const QString &currentPath);

private:
    void regMetaType();
    inline void initialFileType();
    inline void initialThreads();
    inline void initialHeaderText();
    inline void initialGenreText();
    inline void initialPreference();
    static KNMusicGlobal *m_instance;
    static KNMusicParser *m_parser;
    static KNMusicNowPlayingBase *m_nowPlaying;
    static KNMusicSoloMenuBase *m_soloMenu;
    static KNMusicMultiMenuBase *m_multiMenu;
    static KNMusicSearchBase *m_musicSearch;
    static KNMusicDetailTooltipBase *m_detailTooltip;
    static KNMusicDetailDialogBase *m_detailDialog;
    static QString m_musicLibraryPath;
    static QString m_musicRowFormat;
    static QList<QList<QStandardItem *>> m_dragMusicRow;
    static bool m_dragMusicRowTaken;
    explicit KNMusicGlobal(QObject *parent = 0);
    KNConfigure *m_musicConfigure;
    QString m_treeViewHeaderText[MusicDisplayDataCount];
    QStringList m_suffixs, m_listSuffixs,
                m_suffixDescription, m_listSuffixDescription,
                m_indexedGenres;
    QPixmap m_noAlbumArt;
    QThread *m_searcherThread, *m_analysisThread, *m_lyricsThread;
    KNPreferenceWidgetsPanel *m_preferencePanel;
    KNGlobal *m_global;
};

#endif // KNMUSICGLOBAL_H
