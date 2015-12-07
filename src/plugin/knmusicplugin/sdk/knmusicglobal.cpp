/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QThread>

#include "knglobal.h"
#include "knutil.h"
#include "knlocalemanager.h"
#include "knconfigure.h"

#include "knmusicparser.h"
#include "knmusictagparser.h"
#include "knmusicdetaildialog.h"
#include "knmusiclyricsmanager.h"
#include "knmusiclyricsdownloader.h"

#include "knmusicglobal.h"

#include <QDebug>

//Initial the instance pointer to null.
KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicGlobal::~KNMusicGlobal()
{
    //Stop the threads.
    m_searcherThread->quit();
    m_analysisThread->quit();
    //Wait the threads.
    m_searcherThread->wait();
    m_analysisThread->wait();
    //Delete the parser.
    delete m_parser;
}

KNMusicGlobal *KNMusicGlobal::instance()
{
    //Return the instance pointer.
    return m_instance;
}

void KNMusicGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(m_instance==nullptr)
    {
        m_instance=new KNMusicGlobal(parent);
    }
}

QString KNMusicGlobal::indexedGenre(const int &index)
{
    return (index>0 && index<m_indexedGenres.size())?
                m_indexedGenres.at(index):"Unknown";
}

int KNMusicGlobal::genreIndex(const QString &genre)
{
    return m_indexedGenres.indexOf(genre);
}

QString KNMusicGlobal::typeDescription(const QString &suffix) const
{
    //Search the suffix if the suffix list.
    int suffixIndex=m_suffixs.indexOf(suffix.toLower());
    //If we cannot find the suffix, return a emtpy string.
    return suffixIndex==-1?QString():m_suffixDescription.at(suffixIndex);
}

QString KNMusicGlobal::treeViewHeaderText(const int &index) const
{
    return index<MusicDataCount?
                m_treeViewHeaderText[index]:
                QString();
}

void KNMusicGlobal::startThreads()
{
    //All the multi-thread things should be processed here.
    //Move the parser to analysis thread.
    m_parser->moveToThread(m_analysisThread);

    //Start threads.
    m_searcherThread->start();
    m_analysisThread->start();
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
    m_treeViewHeaderText[ISRC]=tr("ISRC");
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

KNMusicGlobal::KNMusicGlobal(QObject *parent) :
    QObject(parent),
    m_suffixs(QStringList()),
    m_listSuffixs(QStringList()),
    m_suffixDescription(QStringList()),
    m_listSuffixDescription(QStringList()),
    m_indexedGenres(QStringList()),
    m_noAlbumArt(QPixmap(":/plugin/music/public/noalbum.png")),
    m_musicLibPath(QString()),
    m_parentWidget(static_cast<QWidget *>(parent)),
    m_detailDialog(new KNMusicDetailDialog(knGlobal->mainWindow())),
    m_lyricsManager(new KNMusicLyricsManager(this)),
    m_parser(new KNMusicParser),
    m_soloMenu(nullptr),
    m_multiMenu(nullptr),
    m_search(nullptr),
    m_backend(nullptr),
    m_nowPlaying(nullptr),
    m_detailTooltip(nullptr),
    m_lyricsDownloadDialog(nullptr),
    m_searcherThread(new QThread(this)),
    m_analysisThread(new QThread(this)),
    m_musicConfigure(knGlobal->userConfigure()->getConfigure("Music"))
{
    //Initial the file type.
    initialFileType();
    //Initial the genere.
    initialGenre();
    //Register the queue arguments.
    qRegisterMetaType<KNMusicAnalysisItem>("KNMusicAnalysisItem");
    qRegisterMetaType<KNMusicDetailInfo>("KNMusicDetailInfo");
    qRegisterMetaType<QList<KNMusicLyricsDownloader::KNMusicLyricsDetails>>(
                "QList<KNMusicLyricsDownloader::KNMusicLyricsDetails>");

    //Set the library path.
    setMusicLibPath(knGlobal->dirPath(KNGlobal::LibraryDir) + "/Music");

    //Update the lyrics path.
    m_lyricsManager->setLyricsDirectory(musicLibraryPath() + "/Lyrics");

    //Link to retranslate.
    knI18n->link(this, &KNMusicGlobal::retranslate);
    retranslate();
}

inline void KNMusicGlobal::initialFileType()
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

    m_listSuffixs<<"cue"
                 <<"cda";
    m_listSuffixDescription<<"Cue sheet (cue)"
                           <<"Compact Disc Audio track (cda)";
}

inline void KNMusicGlobal::initialGenre()
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

KNMusicLyricsDownloadDialogBase *KNMusicGlobal::lyricsDownloadDialog()
{
    return m_lyricsDownloadDialog;
}

void KNMusicGlobal::setLyricsDownloadDialog(
        KNMusicLyricsDownloadDialogBase *lyricsDownloadDialog)
{
    m_lyricsDownloadDialog = lyricsDownloadDialog;
}

KNMusicLyricsManager *KNMusicGlobal::lyricsManager()
{
    return m_lyricsManager;
}

KNMusicMultiMenuBase *KNMusicGlobal::multiMenu() const
{
    return m_multiMenu;
}

void KNMusicGlobal::setMultiMenu(KNMusicMultiMenuBase *multiMenu)
{
    m_multiMenu = multiMenu;
}

KNMusicDetailTooltipBase *KNMusicGlobal::detailTooltip() const
{
    return m_detailTooltip;
}

void KNMusicGlobal::setDetailTooltip(KNMusicDetailTooltipBase *detailTooltip)
{
    m_detailTooltip = detailTooltip;
}

KNMusicBackend *KNMusicGlobal::backend() const
{
    return m_backend;
}

void KNMusicGlobal::setBackend(KNMusicBackend *backend)
{
    m_backend = backend;
}

QPixmap KNMusicGlobal::noAlbumArt() const
{
    return m_noAlbumArt;
}

KNMusicSoloMenuBase *KNMusicGlobal::soloMenu() const
{
    return m_soloMenu;
}

void KNMusicGlobal::setSoloMenu(KNMusicSoloMenuBase *soloMenu)
{
    m_soloMenu = soloMenu;
}

QStringList KNMusicGlobal::suffixList() const
{
    return m_suffixs;
}

QStringList KNMusicGlobal::listSuffixList() const
{
    return m_listSuffixs;
}

QStringList KNMusicGlobal::genreList() const
{
    return m_indexedGenres;
}

KNMusicNowPlayingBase *KNMusicGlobal::nowPlaying() const
{
    return m_nowPlaying;
}

void KNMusicGlobal::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    m_nowPlaying = nowPlaying;
}

KNMusicSearchBase *KNMusicGlobal::search() const
{
    return m_search;
}

void KNMusicGlobal::setSearch(KNMusicSearchBase *search)
{
    m_search = search;
}

KNConfigure *KNMusicGlobal::configure()
{
    return m_musicConfigure;
}

QString KNMusicGlobal::musicLibraryPath() const
{
    return m_musicLibPath;
}

void KNMusicGlobal::setMusicLibPath(const QString &musicLibPath)
{
    //Save and ensure the path should be exist.
    m_musicLibPath=KNUtil::ensurePathValid(musicLibPath);
}
