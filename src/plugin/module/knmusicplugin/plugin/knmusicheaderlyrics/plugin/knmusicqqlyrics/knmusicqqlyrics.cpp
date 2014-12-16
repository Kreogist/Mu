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
    QString url=generateQueryUrl(detailInfo);
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
        get(generateRequestString(*i), responseData);
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

QString KNMusicQQLyrics::processKeywordsToGBK(QString s)
{
    return m_gbkCodec->fromUnicode(processKeywords(s)).toPercentEncoding();
}

QString KNMusicQQLyrics::generateQueryUrl(const KNMusicDetailInfo &detailInfo)
{
    return "http://qqmusic.qq.com/fcgi-bin/qm_getLyricId.fcg?name="+
            processKeywordsToGBK(detailInfo.textLists[Name])+"&singer="+
            processKeywordsToGBK(detailInfo.textLists[Artist])+"&from=qqplayer";
}

QString KNMusicQQLyrics::generateRequestString(const QString &id)
{
    return "http://music.qq.com/miniportal/static/lyric/" +
            QString::number(id.toLongLong()%100) +
            "/" +
            id +
            ".xml";
}

