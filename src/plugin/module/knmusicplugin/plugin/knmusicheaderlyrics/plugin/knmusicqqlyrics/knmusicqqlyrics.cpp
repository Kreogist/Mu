#include <QUrl>
#include <QTextCodec>
#include <QDomDocument>

#include "knmusicqqlyrics.h"

#include <QDebug>

KNMusicQQLyrics::KNMusicQQLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
    //Initial the GBK codec.
    m_gbkCodec=QTextCodec::codecForName("GBK");
    //
    QQFilter=QString(" _-*.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

QString KNMusicQQLyrics::downloadLyrics(const KNMusicDetailInfo &detailInfo)
{
    QString url=QQQueryString(detailInfo);
    QByteArray responseData;
    get(url, responseData);
    if(responseData.isEmpty())
    {
        return QString();
    }
    //Tencent use GBK as default codec.
    QDomDocument xmlDoc;
    xmlDoc.setContent(m_gbkCodec->toUnicode(responseData));
    QDomNodeList lrc=xmlDoc.elementsByTagName("songinfo");
    if(lrc.isEmpty())
    {
        return QString();
    }
    QStringList songid;
    for(int i=0; i<lrc.length(); i++)
    {
        songid.append(lrc.at(i).toElement().attribute("id"));
    }
    for(QStringList::iterator i=songid.begin();
        i!=songid.end();
        ++i)
    {
        get(QQRequestString(*i), responseData);
        if(responseData.isEmpty())
        {
            continue;
        }
        QString xml_text=m_gbkCodec->toUnicode(responseData);
        xmlDoc.setContent(xml_text);
        QDomNodeList lr=xmlDoc.elementsByTagName("lyric");
        if(lr.isEmpty())
        {
            continue;
        }
        QString lyricsContent=lr.at(0).childNodes().at(0).toText().data();
        if(lyricsContent.isEmpty())
        {
            continue;
        }
        return writeLyricsFile(detailInfo, lyricsContent);
    }
    return QString();
}

QString KNMusicQQLyrics::QQStringFilter(QString s)
{
    s=s.toLower();
    s=s.replace(QRegExp("\\'|·|\\$|\\&|–"), "");
    s=s.replace(QRegExp("\\(.*?\\)|\\[.*?]|{.*?}|（.*?"), "");
    s=s.replace(QRegExp("[-/:-@[-`{-~]+"), "");
    return m_gbkCodec->fromUnicode(s).toPercentEncoding();
}

QString KNMusicQQLyrics::QQQueryString(const KNMusicDetailInfo &detailInfo)
{
    return "http://qqmusic.qq.com/fcgi-bin/qm_getLyricId.fcg?name="+
            QQStringFilter(detailInfo.textLists[Name])+"&singer="+
            QQStringFilter(detailInfo.textLists[Artist])+"&from=qqplayer";
}

QString KNMusicQQLyrics::QQRequestString(const QString &id)
{
    return "http://music.qq.com/miniportal/static/lyric/" + QString::number(id.toLongLong()%100)
            + "/" + id + ".xml";
}

