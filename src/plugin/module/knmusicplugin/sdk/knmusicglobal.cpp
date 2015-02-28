/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <QUrl>
#include <QLabel>
#include <QVector>
#include <QThread>
#include <QDir>
#include <QItemSelection>
#include <QStandardItem>

#include "knmessagebox.h"
#include "knconfigure.h"
#include "knglobal.h"
#include "knpreferenceitempanel.h"

#include "knmusicnowplayingbase.h"

#include "knmusicglobal.h"

#include <QDebug>

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicParser *KNMusicGlobal::m_parser=nullptr;
KNMusicNowPlayingBase *KNMusicGlobal::m_nowPlaying=nullptr;
KNMusicSoloMenuBase *KNMusicGlobal::m_soloMenu=nullptr;
KNMusicMultiMenuBase *KNMusicGlobal::m_multiMenu=nullptr;
KNMusicSearchBase *KNMusicGlobal::m_musicSearch=nullptr;
KNMusicDetailTooltipBase *KNMusicGlobal::m_detailTooltip=nullptr;
KNMusicDetailDialogBase *KNMusicGlobal::m_detailDialog=nullptr;
QString KNMusicGlobal::m_musicLibraryPath=QString();
QString KNMusicGlobal::m_musicRowFormat=QString("org.kreogist.mu/MusicModelRow");
bool KNMusicGlobal::m_dragMusicRowTaken=false;
QList<QList<QStandardItem *> > KNMusicGlobal::m_dragMusicRow;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

KNMusicGlobal::~KNMusicGlobal()
{
    //Quit all working threads.
    m_searcherThread->quit();
    m_analysisThread->quit();
    m_lyricsThread->quit();

    //Wait.
    m_searcherThread->wait();
    m_analysisThread->wait();
    m_lyricsThread->wait();
}

QString KNMusicGlobal::msecondToString(const qint64 &msecond)
{
    qint64 second=msecond/1000;
    //Calculate the second part.
    QString secondText=QString::number(second%60);
    //If second if 0-9, add a 0 before the number.
    return secondText.length()==1?
                QString::number(second/60)+":0"+secondText:
                QString::number(second/60)+":"+secondText;
}

QString KNMusicGlobal::dateTimeToString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyy-MM-dd APhh:mm");
}

QString KNMusicGlobal::dateTimeToDataString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyyMMddHHmmss");
}

QString KNMusicGlobal::musicRowFormat()
{
    return m_musicRowFormat;
}

QDateTime KNMusicGlobal::dataStringToDateTime(const QString &text)
{
    return QDateTime::fromString(text, "yyyyMMddHHmmss");
}

bool KNMusicGlobal::isMusicFile(const QString &suffix)
{
    return (m_suffixs.indexOf(suffix.toLower())!=-1);
}

bool KNMusicGlobal::isMusicListFile(const QString &suffix)
{
    return (m_listSuffixs.indexOf(suffix.toLower())!=-1);
}

QString KNMusicGlobal::typeDescription(const QString &suffix) const
{
    int suffixIndex=m_suffixs.indexOf(suffix.toLower());
    return suffixIndex==-1?QString():m_suffixDescription.at(suffixIndex);
}

void KNMusicGlobal::retranslate()
{
    m_treeViewHeaderText[Name]=tr("Name");
    m_treeViewHeaderText[Album]=tr("Album");
    m_treeViewHeaderText[AlbumArtist]=tr("Album Artist");
    m_treeViewHeaderText[AlbumRating]=tr("Album Rating");
    m_treeViewHeaderText[Artist]=tr("Artist");
    m_treeViewHeaderText[BeatsPerMinuate]=tr("Beats Per Minuate");
    m_treeViewHeaderText[BitRate]=tr("Bit Rate");
    m_treeViewHeaderText[Category]=tr("Category");
    m_treeViewHeaderText[Comments]=tr("Comments");
    m_treeViewHeaderText[Composer]=tr("Composer");
    m_treeViewHeaderText[DateAdded]=tr("Date Added");
    m_treeViewHeaderText[DateModified]=tr("Date Modified");
    m_treeViewHeaderText[Description]=tr("Description");
    m_treeViewHeaderText[DiscCount]=tr("Disc Count");
    m_treeViewHeaderText[DiscNumber]=tr("Disc Number");
    m_treeViewHeaderText[Genre]=tr("Genre");
    m_treeViewHeaderText[Kind]=tr("Kind");
    m_treeViewHeaderText[LastPlayed]=tr("Last Played");
    m_treeViewHeaderText[Plays]=tr("Plays");
    m_treeViewHeaderText[Rating]=tr("Rating");
    m_treeViewHeaderText[SampleRate]=tr("Sample Rate");
    m_treeViewHeaderText[Size]=tr("Size");
    m_treeViewHeaderText[Time]=tr("Time");
    m_treeViewHeaderText[TrackCount]=tr("Track Count");
    m_treeViewHeaderText[TrackNumber]=tr("Track Number");
    m_treeViewHeaderText[Year]=tr("Year");
}

void KNMusicGlobal::onActionLibraryMoved(const QString &originalPath,
                                         const QString &currentPath)
{
    Q_UNUSED(originalPath)
    //Backup the original library path.
    QString originalLibraryPath=musicLibraryPath();
    //Set the new path of the music library.
    setMusicLibraryPath(currentPath+"/Music");
    //Emit music library changed signal.
    emit musicLibraryMoved(originalLibraryPath, musicLibraryPath());
}

bool KNMusicGlobal::renameMusicFile(const QString &originalPath,
                                    const QString &preferName)
{
    QFileInfo originalFile(originalPath);
    //If the original file is not
    if(!originalFile.exists())
    {
        //Tell the user that original file cannot find.
        KNMessageBox::information("Error",
                                  tr("Cannot find file:\n") +
                                  originalFile.absoluteFilePath());
        return false;
    }
    //Check the prefer name's availability.
    QFileInfo destinationFile(originalFile.absolutePath()+"/"+preferName);
    if(destinationFile.exists())
    {
        if(!KNMessageBox::question("Overwrite",
                                   tr("File %1 has been exist in folder:\n%2\nOverwrite?").arg(
                                       destinationFile.fileName(), destinationFile.absolutePath())))
        {
            return false;
        }
    }

    bool restoreNowPlaying=false;
    //Check is the file playing.
    if(m_nowPlaying->currentAnalaysisItem().detailInfo.filePath
            ==originalFile.absoluteFilePath())
    {
        //Backup the current playing.
        m_nowPlaying->backupCurrentPlaying();
        //Set the flag.
        restoreNowPlaying=true;
    }

    //Rename the music file.
    bool renameResult=KNGlobal::renameFile(originalFile.absoluteFilePath(),
                                           destinationFile.absoluteFilePath());
    if(renameResult)
    {
        //Emit the file name changed signal.
        emit musicFilePathChanged(originalFile.absoluteFilePath(),
                                  destinationFile.absoluteFilePath(),
                                  destinationFile.fileName());
    }

    //Restore the now playing according to the flag.
    if(restoreNowPlaying)
    {
        m_nowPlaying->restoreCurrentPlaying();
    }
    return renameResult;
}

void KNMusicGlobal::regMetaType()
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QItemSelection>("QItemSelection");
    qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
    qRegisterMetaType<KNMusicDetailInfo>("KNMusicDetailInfo");
    qRegisterMetaType<KNMusicAnalysisItem>("KNMusicAnalysisItem");
}

void KNMusicGlobal::initialFileType()
{
    m_suffixs<<"mp3"
             <<"m4a"
             <<"wav"
             <<"flac"
             <<"ape"
             <<"ogg"
             <<"tta"
             <<"aiff"
             <<"aifc"
             <<"aif"
             <<"mp4"
             <<"mpa"
             <<"mp2"
             <<"mp1"
             <<"midi"
             <<"mid"
             <<"mp3pro"
             <<"mpc"
             <<"aac"
             <<"wma"
             <<"fla"
             <<"tak"
             <<"mp+"
             <<"aa"
             <<"ra"
             <<"mac"
             <<"rmi"
             <<"dtswav"
             <<"dts"
             <<"snd"
             <<"au"
             <<"ac3"
             <<"xm"
             <<"umx";
    m_listSuffixs<<"cue"
                 <<"cda";

    m_suffixDescription<<"MPEG Audio Layer III (mp3)"
                       <<"MPEG-4 Part 14 (m4a)"
                       <<"Waveform Audio File Format (wav)"
                       <<"Free Lossless Audio Codec (flac)"
                       <<"Monkey's Audio (ape)"
                       <<"Ogg Vorbis Audio (ogg)"
                       <<"True Audio Codec (tta)"
                       <<"Audio Interchange File Format (aiff)"
                       <<"Audio Interchange File Format (aifc)"
                       <<"Audio Interchange File Format (aif)"
                       <<"MPEG-4 Part 14 (mp4)"
                       <<"MPEG Audio Layer II (mpa)"
                       <<"MPEG Audio Layer II (mp2)"
                       <<"MPEG Audio Layer I (mp1)"
                       <<"Musical Instrument Digital Interface (midi)"
                       <<"Musical Instrument Digital Interface (mid)"
                       <<"MPEG Audio Layer III with SBR (mp3pro)"
                       <<"Musepack Lossy Audio Codec (mpc)"
                       <<"Advanced Audio Coding (aac)"
                       <<"Windows Media Audio (wma)"
                       <<"Flash Audio (fla)"
                       <<"Tom's lossless Audio Kompressor (tak)"
                       <<"Musepack Lossy Audio Codec (mp+)"
                       <<"Audible Audio File (aa)"
                       <<"Real Audio (ra)"
                       <<"Monkey's Audio (mac)"
                       <<"Musical Instrument Digital Interface (rmi)"
                       <<"DTS Audio Codec (dtswav)"
                       <<"DTS Audio Codec (dts)"
                       <<"SouND Audio (snd)"
                       <<"Au File Format (au)"
                       <<"Dolby Surround Audio Coding-3 (ac3)"
                       <<"Fast Tracker Module (xm)"
                       <<"Unreal Engine 1 Music Format (umx)";

    m_listSuffixDescription<<"Compact Disc Audio track (cda)"
                           <<"Cue sheet (cue)";
}

void KNMusicGlobal::initialThreads()
{
    m_searcherThread=new QThread(this);
    m_analysisThread=new QThread(this);
    m_lyricsThread=new QThread(this);
    m_searcherThread->start();
    m_analysisThread->start();
    m_lyricsThread->start();
}

void KNMusicGlobal::initialHeaderText()
{
    m_treeViewHeaderText[BlankData]=" ";
}

void KNMusicGlobal::initialGenreText()
{
    m_indexedGenres<<"Blues"
                   <<"Classic Rock"
                   <<"Country"
                   <<"Dance"
                   <<"Disco"
                   <<"Funk"
                   <<"Grunge"
                   <<"Hip-Hop"
                   <<"Jazz"
                   <<"Metal"
                   <<"New Age"
                   <<"Oldies"
                   <<"Other"
                   <<"Pop"
                   <<"R&B"
                   <<"Rap"
                   <<"Reggae"
                   <<"Rock"
                   <<"Techno"
                   <<"Industrial"
                   <<"Alternative"
                   <<"Ska"
                   <<"Death Metal"
                   <<"Pranks"
                   <<"Soundtrack"
                   <<"Euro-Techno"
                   <<"Ambient"
                   <<"Trip-Hop"
                   <<"Vocal"
                   <<"Jazz+Funk"
                   <<"Fusion"
                   <<"Trance"
                   <<"Classical"
                   <<"Instrumental"
                   <<"Acid"
                   <<"House"
                   <<"Game"
                   <<"Sound Clip"
                   <<"Gospel"
                   <<"Noise"
                   <<"Alternative Rock"
                   <<"Bass"
                   <<"Soul"
                   <<"Punk"
                   <<"Space"
                   <<"Meditative"
                   <<"Instrumental Pop"
                   <<"Instrumental Rock"
                   <<"Ethnic"
                   <<"Gothic"
                   <<"Darkwave"
                   <<"Techno-Industrial"
                   <<"Electronic"
                   <<"Pop-Folk"
                   <<"Eurodance"
                   <<"Dream"
                   <<"Southern Rock"
                   <<"Comedy"
                   <<"Cult"
                   <<"Gangsta"
                   <<"Top 40"
                   <<"Christian Rap"
                   <<"Pop/Funk"
                   <<"Jungle"
                   <<"Native US"
                   <<"Cabaret"
                   <<"New Wave"
                   <<"Psychadelic"
                   <<"Rave"
                   <<"Showtunes"
                   <<"Trailer"
                   <<"Lo-Fi"
                   <<"Tribal"
                   <<"Acid Punk"
                   <<"Acid Jazz"
                   <<"Polka"
                   <<"Retro"
                   <<"Musical"
                   <<"Rock & Roll"
                   <<"Hard Rock"
                   <<"Folk"
                   <<"Folk-Rock"
                   <<"National Folk"
                   <<"Swing"
                   <<"Fast Fusion"
                   <<"Bebob"
                   <<"Latin"
                   <<"Revival"
                   <<"Celtic"
                   <<"Bluegrass"
                   <<"Avantgarde"
                   <<"Gothic Rock"
                   <<"Progressive Rock"
                   <<"Psychedelic Rock"
                   <<"Symphonic Rock"
                   <<"Slow Rock"
                   <<"Big Band"
                   <<"Chorus"
                   <<"Easy Listening"
                   <<"Acoustic"
                   <<"Humour"
                   <<"Speech"
                   <<"Chanson"
                   <<"Opera"
                   <<"Chamber Music"
                   <<"Sonata"
                   <<"Symphony"
                   <<"Booty Bass"
                   <<"Primus"
                   <<"Porn Groove"
                   <<"Satire"
                   <<"Slow Jam"
                   <<"Club"
                   <<"Tango"
                   <<"Samba"
                   <<"Folklore"
                   <<"Ballad"
                   <<"Power Ballad"
                   <<"Rhythmic Soul"
                   <<"Freestyle"
                   <<"Duet"
                   <<"Punk Rock"
                   <<"Drum Solo"
                   <<"Acapella"
                   <<"Euro-House"
                   <<"Dance Hall"
                   <<"Goa"
                   <<"Drum & Bass"
                   <<"Club House"
                   <<"Hardcore"
                   <<"Terror"
                   <<"Indie"
                   <<"BritPop"
                   <<"Negerpunk"
                   <<"Polsk Punk"
                   <<"Beat"
                   <<"Christian Gangsta Rap"
                   <<"Heavy Metal"
                   <<"Black Metal"
                   <<"Crossover"
                   <<"Contemporary Christian"
                   <<"Christian Rock"
                   <<"Merengue"
                   <<"Salsa"
                   <<"Thrash Metal"
                   <<"Anime"
                   <<"JPop"
                  <<"Synthpop";
}

KNMusicLyricsDownloadDialogBase *KNMusicGlobal::lyricsDownloadDialog() const
{
    return m_lyricsDownloadDialog;
}

void KNMusicGlobal::setLyricsDownloadDialog(KNMusicLyricsDownloadDialogBase *lyricsDownloadDialog)
{
    m_lyricsDownloadDialog = lyricsDownloadDialog;
}

KNMusicDetailDialogBase *KNMusicGlobal::detailDialog()
{
    return m_detailDialog;
}

void KNMusicGlobal::setDetailDialog(KNMusicDetailDialogBase *detailDialog)
{
    m_detailDialog = detailDialog;
}

KNMusicDetailTooltipBase *KNMusicGlobal::detailTooltip()
{
    return m_detailTooltip;
}

void KNMusicGlobal::setDetailTooltip(KNMusicDetailTooltipBase *detailTooltip)
{
    m_detailTooltip = detailTooltip;
}

KNMusicSearchBase *KNMusicGlobal::musicSearch()
{
    return m_musicSearch;
}

void KNMusicGlobal::setMusicSearch(KNMusicSearchBase *musicSearch)
{
    m_musicSearch = musicSearch;
}

QList<QList<QStandardItem *> > KNMusicGlobal::dragMusicRow()
{
    //Set the taken flag.
    m_dragMusicRowTaken=true;
    return m_dragMusicRow;
}

void KNMusicGlobal::setDragMusicRow(const QList<QList<QStandardItem *> > &dragMusicRow)
{
    //Recover the memory first.
    clearDragMusicRow();
    //Set the rows.
    m_dragMusicRow = dragMusicRow;
}

void KNMusicGlobal::clearDragMusicRow()
{
    //Remove all the data in the row.
    while(!m_dragMusicRow.isEmpty())
    {
        qDeleteAll(m_dragMusicRow.takeFirst());
    }
    //Clear the rows.
    m_dragMusicRow.clear();
    //Set the taken flag.
    m_dragMusicRowTaken=false;
}

QString KNMusicGlobal::musicLibraryPath()
{
    return m_musicLibraryPath;
}

void KNMusicGlobal::setMusicLibraryPath(const QString &musicLibraryPath)
{
    //Set the path.
    m_musicLibraryPath=KNGlobal::ensurePathAvaliable(musicLibraryPath);
}

quint32 KNMusicGlobal::charToInt32(char *rawTagData)
{
    return (((quint32)rawTagData[0]<<24) & 0b11111111000000000000000000000000) +
           (((quint32)rawTagData[1]<<16) & 0b00000000111111110000000000000000) +
           (((quint32)rawTagData[2]<<8)  & 0b00000000000000001111111100000000) +
           ( (quint32)rawTagData[3]      & 0b00000000000000000000000011111111);
}

quint32 KNMusicGlobal::inverseCharToInt32(char *rawTagData)
{
    return (((quint32)rawTagData[3]<<24) & 0b11111111000000000000000000000000) +
           (((quint32)rawTagData[2]<<16) & 0b00000000111111110000000000000000) +
           (((quint32)rawTagData[1]<<8)  & 0b00000000000000001111111100000000) +
           ( (quint32)rawTagData[0]      & 0b00000000000000000000000011111111);
}

KNMusicMultiMenuBase *KNMusicGlobal::multiMenu()
{
    return m_multiMenu;
}

void KNMusicGlobal::setMultiMenu(KNMusicMultiMenuBase *multiMenu)
{
    m_multiMenu = multiMenu;
}

KNMusicParser *KNMusicGlobal::parser()
{
    return m_parser;
}

void KNMusicGlobal::setParser(KNMusicParser *parser)
{
    m_parser = parser;
}

KNConfigure *KNMusicGlobal::musicConfigure()
{
    return m_musicConfigure;
}

void KNMusicGlobal::setMusicConfigure(KNConfigure *musicConfigure)
{
    m_musicConfigure=musicConfigure;
}

KNMusicNowPlayingBase *KNMusicGlobal::nowPlaying()
{
    return m_nowPlaying;
}

void KNMusicGlobal::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    m_nowPlaying=nowPlaying;
    //Link the playing request and play slot.
    connect(this, SIGNAL(requirePlayMusicRow(KNMusicProxyModel*,int,KNMusicTab*)),
            m_nowPlaying, SLOT(playMusicRow(KNMusicProxyModel*,int,KNMusicTab*)));
}

KNMusicSoloMenuBase *KNMusicGlobal::soloMenu()
{
    return m_soloMenu;
}

void KNMusicGlobal::setSoloMenu(KNMusicSoloMenuBase *soloMenu)
{
    m_soloMenu = soloMenu;
}

KNMusicGlobal::KNMusicGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial global instance.
    m_global=KNGlobal::instance();
    //Register music metatypes.
    regMetaType();
    //Initial music types.
    initialFileType();
    //Initial threads.
    initialThreads();
    //Initial resources.
    initialHeaderText();
    initialGenreText();

    //Set the library path.
    setMusicLibraryPath(m_global->libraryPath()+"/Music");
    //Link the library changed request.
    connect(KNGlobal::instance(), &KNGlobal::libraryMoved,
            this, &KNMusicGlobal::onActionLibraryMoved);

    //Connect retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicGlobal::retranslate);
    //Get the latest translation.
    retranslate();
}

QPixmap KNMusicGlobal::noAlbumArt() const
{
    return m_noAlbumArt;
}

QThread *KNMusicGlobal::searchThread()
{
    return m_searcherThread;
}

QThread *KNMusicGlobal::analysisThread()
{
    return m_analysisThread;
}

QThread *KNMusicGlobal::lyricsThread()
{
    return m_lyricsThread;
}

void KNMusicGlobal::setNoAlbumArt(const QPixmap &noAlbumArt)
{
    m_noAlbumArt = noAlbumArt;
}

QString KNMusicGlobal::treeViewHeaderText(const int &index)
{
    return m_treeViewHeaderText[index];
}

QString KNMusicGlobal::indexedGenre(const int &index)
{
    return (index>0 && index<m_indexedGenres.size())?
                    m_indexedGenres.at(index):"Unknown";
}
