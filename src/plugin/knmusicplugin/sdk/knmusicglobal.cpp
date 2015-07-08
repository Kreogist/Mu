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

#include "knmusicparser.h"
#include "knmusicdetaildialog.h"
#include "knmusicutil.h"

#include "knmusicglobal.h"

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

QString KNMusicGlobal::typeDescription(const QString &suffix) const
{
    //Search the suffix if the suffix list.
    int suffixIndex=m_suffixs.indexOf(suffix.toLower());
    //If we cannot find the suffix, return a emtpy string.
    return suffixIndex==-1?QString():m_suffixDescription.at(suffixIndex);
}

void KNMusicGlobal::retranslate()
{
    ;
}

KNMusicGlobal::KNMusicGlobal(QObject *parent) :
    QObject(parent),
    m_parentWidget(static_cast<QWidget *>(parent)),
    m_detailDialog(new KNMusicDetailDialog(m_parentWidget)),
    m_parser(new KNMusicParser),
    m_searcherThread(new QThread(this)),
    m_analysisThread(new QThread(this))
{
    //Initial the file type.
    initialFileType();
    //Initial the genere.
    initialGenre();

    //Move the parser to analysis thread.
    m_parser->moveToThread(m_analysisThread);

    //Start threads.
    m_searcherThread->start();
    m_analysisThread->start();
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
