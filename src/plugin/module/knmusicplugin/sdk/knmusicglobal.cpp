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
#include <QItemSelection>
#include <QStandardItem>

#include "preference/knpreferenceitembase.h"
#include "knpreferencewidgetspanel.h"

#include "knmusicglobal.h"

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicParser *KNMusicGlobal::m_parser=nullptr;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

KNMusicGlobal::~KNMusicGlobal()
{
    //Quit all working threads.
    m_searcherThread->quit();
    m_analysisThread->quit();

    //Wait.
    m_searcherThread->wait();
    m_analysisThread->wait();
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
    m_treeViewHeaderText[Grouping]=tr("Grouping");
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

void KNMusicGlobal::regMetaType()
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
    qRegisterMetaType<QItemSelection>("QItemSelection");
    qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
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

    m_suffixDescription<<tr("MPEG Audio Layer III (mp3)")
                           <<tr("MPEG-4 Part 14 (m4a)")
                           <<tr("Waveform Audio File Format (wav)")
                           <<tr("Free Lossless Audio Codec (flac)")
                           <<tr("Monkey's Audio (ape)")
                           <<tr("Ogg Vorbis Audio (ogg)")
                           <<tr("True Audio Codec (tta)")
                           <<tr("Audio Interchange File Format (aiff)")
                           <<tr("Audio Interchange File Format (aifc)")
                           <<tr("Audio Interchange File Format (aif)")
                           <<tr("MPEG-4 Part 14 (mp4)")
                           <<tr("MPEG Audio Layer II (mpa)")
                           <<tr("MPEG Audio Layer II (mp2)")
                           <<tr("MPEG Audio Layer I (mp1)")
                           <<tr("Musical Instrument Digital Interface (midi)")
                           <<tr("Musical Instrument Digital Interface (mid)")
                           <<tr("MPEG Audio Layer III with SBR (mp3pro)")
                           <<tr("Musepack Lossy Audio Codec (mpc)")
                           <<tr("Advanced Audio Coding (aac)")
                           <<tr("Windows Media Audio (wma)")
                           <<tr("Flash Audio (fla)")
                           <<tr("Tom's lossless Audio Kompressor (tak)")
                           <<tr("Musepack Lossy Audio Codec (mp+)")
                           <<tr("Audible Audio File (aa)")
                           <<tr("Real Audio (ra)")
                           <<tr("Monkey's Audio (mac)")
                           <<tr("Musical Instrument Digital Interface (rmi)")
                           <<tr("DTS Audio Codec (dtswav)")
                           <<tr("DTS Audio Codec (dts)")
                           <<tr("SouND Audio (snd)")
                           <<tr("Au File Format (au)")
                           <<tr("Dolby Surround Audio Coding-3 (ac3)")
                           <<tr("Fast Tracker Module (xm)")
                           <<tr("Unreal Engine 1 Music Format (umx)");

    m_listSuffixDescription<<tr("Compact Disc Audio track (cda)")
                          <<tr("Cue sheet (cue)");
}

void KNMusicGlobal::initialThreads()
{
    m_searcherThread=new QThread(this);
    m_analysisThread=new QThread(this);
    m_searcherThread->start();
    m_analysisThread->start();
}

void KNMusicGlobal::initialHeaderText()
{
    m_treeViewHeaderText[BlankData]=" ";
}

KNMusicParser *KNMusicGlobal::parser()
{
    return m_parser;
}

void KNMusicGlobal::setParser(KNMusicParser *parser)
{
    m_parser = parser;
}

KNMusicGlobal::KNMusicGlobal(QObject *parent) :
    QObject(parent)
{
    //Register music metatypes.
    regMetaType();
    //Initial music types.
    initialFileType();
    //Initial threads.
    initialThreads();
    //Initial resources.
    initialHeaderText();

    //Get the latest translation.
    retranslate();
}

void KNMusicGlobal::setPreferencePanel(KNPreferenceWidgetsPanel *preferencePanel)
{
    m_preferencePanel=preferencePanel;
}

void KNMusicGlobal::addTitle(QLabel *title, const bool &isAdvanced)
{
    m_preferencePanel->addTitle(title, isAdvanced);
}

void KNMusicGlobal::addItem(KNPreferenceItemBase *item, const bool &isAdvanced)
{
    m_preferencePanel->addItem(item, isAdvanced);
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

void KNMusicGlobal::setNoAlbumArt(const QPixmap &noAlbumArt)
{
    m_noAlbumArt = noAlbumArt;
}

QString KNMusicGlobal::treeViewHeaderText(const int &index)
{
    return m_treeViewHeaderText[index];
}
