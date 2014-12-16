#include <QDir>
#include <QFileInfo>

#include "knmusiclyricsglobal.h"

KNMusicLyricsGlobal::KNMusicLyricsGlobal(QObject *parent) :
    QObject(parent)
{
    ;
}

QString KNMusicLyricsGlobal::m_lyricsFolderPath="";

QString KNMusicLyricsGlobal::lyricsFolderPath()
{
    return m_lyricsFolderPath;
}

void KNMusicLyricsGlobal::setLyricsFolderPath(const QString &lyricsFolderPath)
{
    QFileInfo lyricsFolderInfo(lyricsFolderPath);
    if(lyricsFolderInfo.exists())
    {
        if(lyricsFolderInfo.isDir())
        {
            m_lyricsFolderPath = lyricsFolderInfo.absoluteFilePath() + "/";
        }
    }
    else
    {
        QDir lyricsFolder(lyricsFolderInfo.absoluteFilePath());
        lyricsFolder.mkpath(lyricsFolder.absolutePath());
        if(lyricsFolder.exists())
        {
            m_lyricsFolderPath=lyricsFolder.absolutePath() + "/";
        }
    }
}


