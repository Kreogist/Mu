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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QDomDocument>
#include <QTextCodec>
#include <QHash>

#include "knmusicttplayerlyrics.h"

#include <QDebug>

KNMusicTTPlayerLyrics::KNMusicTTPlayerLyrics(QObject *parent) :
    KNMusicLyricsDownloader(parent)
{
    m_utf16LE=QTextCodec::codecForName("UTF-16LE");
}

KNMusicTTPlayerLyrics::~KNMusicTTPlayerLyrics()
{

}

QString KNMusicTTPlayerLyrics::downloaderName()
{
    return tr("TTPlayer");
}

void KNMusicTTPlayerLyrics::downloadLyrics(
        const KNMusicDetailInfo &detailInfo,
        QList<KNMusicLyricsDetails> &lyricsList)
{
    //Another address: http://ttlrccnc.qianqian.com
    QString queryUrl="http://ttlrcct.qianqian.com"
                     "/dll/lyricsvr.dll?sh?Artist="+
                     utf16LEHex(
                       processKeywords(detailInfo.textLists[Artist].toString()))
                     + "&Title="+
                     utf16LEHex(
                        processKeywords(detailInfo.textLists[Name].toString())) +
                     "&Flags=0";
    //Get the xml data from the url.
    QByteArray responseData;
    get(queryUrl, responseData);
    //Found song lyrics info.
    QDomDocument songInfoDocument;
    songInfoDocument.setContent(responseData);
    QDomNodeList lyrics=
            songInfoDocument.documentElement().elementsByTagName("lrc");
    if(lyrics.isEmpty())
    {
        return;
    }
    //Save the ids.
    QList<QHash<QString, QString>> lyricsInfoList;
    for(int i=0; i<lyrics.size(); i++)
    {
        QDomElement currentLyrics=lyrics.at(i).toElement();
        //Generate the lyrics info.
        QHash<QString, QString> lyricsInfo;
        lyricsInfo.insert("title", currentLyrics.attribute("title"));
        lyricsInfo.insert("artist", currentLyrics.attribute("artist"));
        lyricsInfo.insert("id", currentLyrics.attribute("id"));
        //Add to lyrics info.
        lyricsInfoList.append(lyricsInfo);
    }
    //Download lyrics.
    for(QList<QHash<QString, QString>>::iterator i=lyricsInfoList.begin();
        i!=lyricsInfoList.end();
        ++i)
    {
        QString downloadUrl="http://ttlrcct.qianqian.com"
                            "/dll/lyricsvr.dll?dl?Id=" +
                            (*i).value("id") +
                            "&Code="+
                            generateCode(*i);
        //Download data.
        get(downloadUrl, responseData);
        if(!responseData.isEmpty() &&
                !responseData.contains("errmsg"))
        {
            //Generate the lyrics details data.
            KNMusicLyricsDetails currentDetails;
            currentDetails.title=(*i).value("title");
            currentDetails.artist=(*i).value("artist");
            saveLyrics(detailInfo, responseData, currentDetails, lyricsList);
        }
    }
}

inline QString KNMusicTTPlayerLyrics::generateCode(
        const QHash<QString, QString> &info)
{
    QString utf8hex=utf8HexText(info.value("artist")+info.value("title"));
    QList<quint8> code;
    int len=utf8hex.length()/2;
    for(int i=0; i<utf8hex.size(); i+=2)
    {
        code.append(utf8hex.mid(i, 2).toInt(0, 16));
    }
    qint32 id=info.value("id").toLongLong(),
           t1=(id & 0x0000FF00) >> 8,
           t2=0,
           t3=((id & 0x00FF0000) == 0)?
                   (0x000000FF & ~t1):
                   (0x000000FF & ((id & 0x00FF0000) >> 16));
    t3 = t3 | ((0x000000FF & id) << 8);
    t3 = t3 << 8;
    t3 = t3 | (0x000000FF & t1);
    t3 = t3 << 8;
    if ((id & 0xFF000000) == 0)
    {
        t3 = t3 | (0x000000FF & (~id));
    }
    else
    {
        t3 = t3 | (0x000000FF & (id >> 24));
    }
    int j=len-1;
    while(j>=0)
    {
        qint8 c=(qint8)code.at(j);
        if((quint8)c>=(quint8)0x80)
        {
            c-=(qint8)0x100;
        }
        t1 = (((qint32)c + t2) & 0xFFFFFFFF);
        t2 = ((t2 << (j % 2 + 4)) & 0xFFFFFFFF);
        t2 = ((t1 + t2) & 0xFFFFFFFF);
        j--;
    }
    j = 0;
    t1 = 0;
    while(j<len)
    {
        qint8 c=(qint8)code.at(j);
        if((quint8)c>=(quint8)128)
        {
            c-=(qint8)256;
        }
        qint32 t4=(((qint32)c+t1)& 0xFFFFFFFF);
        t1 = ((t1 << (j % 2 + 3)) & 0xFFFFFFFF);
        t1 = ((t1 + t4) & 0xFFFFFFFF);
        j++;
    }
    qint64 t5=conv((qint64)t2 ^ (qint64)t3);
    t5 = conv(t5 + (t1 | id));
    t5 = conv(t5 * (t1 | t3));
    t5 = conv(t5 * (t2 ^ id));
    if (t5 > (qint32)0x80000000)
    {
        t5 = (t5 - 0x100000000) & 0xFFFFFFFF;
    }
    return QString::number(t5, 10);
}

QString KNMusicTTPlayerLyrics::utf8HexText(const QString &original)
{
    QByteArray utf8Array=original.toUtf8();
    //Read the data, and translate to string.
    char *utf8Data=utf8Array.data();
    QString utf8Text;
    for(int i=0; i<utf8Array.size(); i++)
    {
        if((quint8)utf8Data[i]<16)
        {
            utf8Text.append('0');
        }
        utf8Text.append(QString::number((quint8)utf8Data[i], 16).toUpper());
    }
    return utf8Text;
}

QString KNMusicTTPlayerLyrics::utf16LEHex(const QString &original)
{
    //Get the UTF-16 LE data of the original text.
    QByteArray leArray=m_utf16LE->fromUnicode(original);
    //Remove the first two bytes(BOM).
    if(leArray.size()<2)
    {
        return QString();
    }
    leArray.remove(0, 2);
    //Read the data, and translate to string.
    char *leData=leArray.data();
    QString leText;
    for(int i=0; i<leArray.size(); i++)
    {
        if((quint8)leData[i]<16)
        {
            leText.append('0');
        }
        leText.append(QString::number((quint8)leData[i], 16).toUpper());
    }
    return leText;
}

inline qint64 KNMusicTTPlayerLyrics::conv(qint64 i)
{
    i&=0xFFFFFFFF;
    qint64 r=(qint64)i%0x100000000;
    if (i >= 0 && r > 0x80000000)
    {
        r = r - 0x100000000;
    }
    if (i < 0 && r < 0x80000000)
    {
        r = r + 0x100000000;
    }
    return r & 0xFFFFFFFF;
}
