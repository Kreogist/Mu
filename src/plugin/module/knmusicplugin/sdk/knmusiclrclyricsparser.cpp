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
#include <QFile>
#include <QLinkedList>
#include <QTextCodec>

#include "knglobal.h"

#include "knmusiclrclyricsparser.h"

KNMusicLRCLyricsParser::KNMusicLRCLyricsParser(QObject *parent) :
    QObject(parent)
{
    //Initial text codecs.
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localeCodec=KNGlobal::localeDefaultCodec();
    //Set the regexp.
    m_frameCatchRegExp.setPattern("\\[[^\\]]*\\]");
    m_noneNumberRegExp.setPattern("[^0-9]");
}

KNMusicLRCLyricsParser::~KNMusicLRCLyricsParser()
{

}

bool KNMusicLRCLyricsParser::parseFile(const QString &filePath,
                                       QList<qint64> &positionList,
                                       QStringList &textList)
{
    //Read the file contents.
    QFile lyricsFile(filePath);
    if(!lyricsFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray lyricsContent=lyricsFile.readAll();
    lyricsFile.close();
    //Try to parse the lyrics data via UTF-8 codecs.
    QTextCodec::ConverterState convState;
    QString lyricsTextData=m_utf8Codec->toUnicode(lyricsContent.constData(),
                                                  lyricsContent.size(),
                                                  &convState);
    //If we can't decode it, try to use default codec.
    if(convState.invalidChars>0)
    {
        lyricsTextData=m_localeCodec->toUnicode(lyricsContent.constData(),
                                                lyricsContent.size(),
                                                &convState);
        //If we still cannot decode it, ask user to select a codec.
        if(convState.invalidChars>0)
        {
            //!FIXME: add ask user to choice the text codec.
            ;
        }
    }
    //Parse the lyrics data.
    return parseData(lyricsTextData, positionList, textList);
}

bool KNMusicLRCLyricsParser::parseData(const QString &lyricsTextData,
                                       QList<qint64> &positionList,
                                       QStringList &textList)
{
    //Clear the position list and text list.
    positionList.clear();
    textList.clear();
    //Split the lyrics text data.
    QStringList lyricsRawData=lyricsTextData.split(QRegExp("\n"),
                                                   QString::SkipEmptyParts);
    QList<LyricsLine> lyricsLineList;
    //Remove the same line in the lyrics raw data.
    lyricsRawData.removeDuplicates();
    //Parse the lyrics raw data.
    while(!lyricsRawData.isEmpty())
    {
        //Get the first line of the current list and remove all spaces.
        QString currentLine=lyricsRawData.takeFirst().simplified();
        //Find frames in the current line.
        QRegularExpressionMatchIterator frameMatcher=
                m_frameCatchRegExp.globalMatch(currentLine);
        int lastPosition=0;
        QLinkedList<QString> frameLists;
        //Get all the frames.
        while(frameMatcher.hasNext())
        {
            QRegularExpressionMatch matchedFrame=frameMatcher.next();
            //Check is the current matched frame is at the last position.
            //An example is:
            //  [00:00:01] Won't chu kiss me![00:00:03]Saikou no
            if(matchedFrame.capturedStart()!=lastPosition)
            {
                //Then we should pick out the data before the current start and
                //last position.
                //Like 'Won't chu kiss me!' in the example.
                QString text=currentLine.mid(lastPosition,
                                             matchedFrame.capturedStart()-lastPosition);
                //Parse the datas to frame lists.
                while(!frameLists.isEmpty())
                {
                    parseFrames(frameLists.takeFirst(),
                                text,
                                lyricsLineList);
                }
            }
            //Add current frame to frame list.
            frameLists.append(currentLine.mid(matchedFrame.capturedStart(),
                                              matchedFrame.capturedLength()));
            //Update the last position.
            lastPosition=matchedFrame.capturedEnd();
        }
        //Remove the previous datas, and parse the left datas to frame lists.
        currentLine.remove(0, lastPosition);
        while(!frameLists.isEmpty())
        {
            parseFrames(frameLists.takeFirst(),
                        currentLine,
                        lyricsLineList);
        }
    }
    //Check is the lyrics line list is empty or not, if it's empty, means we
    //can't parse it.
    if(lyricsLineList.isEmpty())
    {
        return false;
    }
    //Sorr the lyrics line.
    //- Why stable sort?
    //  Because there might be some frames at the same time. Display them with
    //their exist order.
    qStableSort(lyricsLineList.begin(), lyricsLineList.end(), frameLessThan);
    //Combine the same timestamp lyrics.
    QMap<qint64, QString> lyricsCombineMap;
    for(QList<LyricsLine>::iterator i=lyricsLineList.begin();
        i!=lyricsLineList.end();
        ++i)
    {
        lyricsCombineMap.insert((*i).position,
                                lyricsCombineMap.contains((*i).position)?
                                    lyricsCombineMap.value((*i).position)+'\n'+(*i).text:
                                    (*i).text);
    }
    //Export the position and the text.
    positionList=lyricsCombineMap.keys();
    textList=lyricsCombineMap.values();
    return true;
}

void KNMusicLRCLyricsParser::parseFrames(QString frame,
                                         const QString &text,
                                         QList<LyricsLine> &positionList)
{
    //Remove the first '[' and the last ']'.
    frame=frame.mid(1, frame.length()-2);
    //Find the first colon(':'), and test the data before colon is number or not.
    int colonPos=frame.indexOf(':');
    if(-1==colonPos)
    {
        return;
    }
    //Tried to translate the data before colon to a number.
    bool translateResult=false;
    qint64 minuatePart=frame.left(colonPos).toLongLong(&translateResult);
    if(!translateResult)
    {
        return;
    }
    LyricsLine currentFrame;
    currentFrame.position=minuatePart*60000;
    //Find the second char, and there are three types of LRC frames:
    // [MM:ss.xx]
    // [MM:ss]
    // [MM:ss:xx]
    //So we need to find the second char, and tried to translate the second part
    //of the frame.
    int secondCharPos=frame.indexOf(m_noneNumberRegExp, colonPos+1);
    if(secondCharPos==-1)
    {
        //This format might be [MM:ss], translate the remain parts to a number.
        qint64 secondPart=frame.mid(colonPos+1).toLongLong(&translateResult);
        //You can never find a second more than 59.
        if(!translateResult || secondPart>59)
        {
            return;
        }
        //Add the second part to frame position.
        currentFrame.position+=secondPart*1000;
    }
    else
    {
        //This must be a time like [MM:ss.xx] or [MM:ss:xx]
        qint64 secondPart=frame.mid(colonPos+1, secondCharPos-colonPos-1).toLongLong(&translateResult);
        if(!translateResult || secondPart>59)
        {
            return;
        }
        qint64 thirdPart=frame.mid(secondCharPos+1).toLongLong(&translateResult);
        //No millisecond is larger than 999. Due to the xx can only be to 990.
        if(!translateResult || thirdPart>99)
        {
            return;
        }
        //Add the second part and third part to frame position.
        currentFrame.position+=secondPart*1000+thirdPart*10;
    }
    //Set the text and add the lyrics line to the list.
    currentFrame.text=text;
    positionList.append(currentFrame);
}

bool KNMusicLRCLyricsParser::frameLessThan(const LyricsLine &frameLeft,
                                           const LyricsLine &frameRight)
{
    return frameLeft.position<frameRight.position;
}

