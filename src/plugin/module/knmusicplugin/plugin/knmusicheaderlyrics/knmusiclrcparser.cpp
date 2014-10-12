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
#include <QTextStream>
#include <QStringList>
#include <QLinkedList>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include "knmusiclrcparser.h"

#include <QDebug>

KNMusicLRCParser::KNMusicLRCParser(QObject *parent) :
    QObject(parent)
{
    //Set frame catch regexp.
    m_frameCatch.setPattern("\\[[^\\]]*\\]");
    //Set the header text.
    m_headerText.append("ti");
    m_headerText.append("ar");
    m_headerText.append("al");
    m_headerText.append("by");
}

void KNMusicLRCParser::parseFile(const QString &filePath,
                                 QMap<int, QString> &properties,
                                 QList<qint64> &positions,
                                 QStringList &lyricsText)
{
    //-------Clear the data---------
    positions.clear();
    lyricsText.clear();
    //-------Read the file---------
    QStringList lyricsRawData;
    //Open the lyric file.
    QFile lyricsFile(filePath);
    if(!lyricsFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //Using the text stream read all data.
    QTextStream lyricsStream(&lyricsFile);
    QString lineCache;
    while(!(lineCache=lyricsStream.readLine()).isNull())
    {
        lineCache=lineCache.simplified();
        if(!lineCache.isEmpty())
        {
            lyricsRawData.append(lineCache);
        }
    }
    //Close the file.
    lyricsFile.close();
    //-------Parse the file---------
    //Clear the data.
    QList<LRCFrame> lyricsData;
    //Process each line.
    while(!lyricsRawData.isEmpty())
    {
        //Take one line and remove all spaces.
        QString currentLine=lyricsRawData.takeFirst();
        //Using a linked list to storage all the frames.
        QLinkedList<QString> currentFrames;
        //Catch the frame data.
        QRegularExpressionMatchIterator matchIterator=
                m_frameCatch.globalMatch(currentLine);
        int lastPos=0;
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match=matchIterator.next();
            if(match.capturedStart()!=lastPos)
            {
                //Means it's not in the head of the current line.
                break;
            }
            currentFrames.append(currentLine.mid(match.capturedStart(),
                                                 match.capturedLength()));
            lastPos=match.capturedEnd();
        }
        //Remove all the frames.
        currentLine.remove(0, lastPos);
        //-------Prase the line------
        while(!currentFrames.isEmpty())
        {
            parseFrame(currentFrames.takeFirst(),
                       currentLine,
                       lyricsData,
                       properties);
        }
    }
    //-----------Sort the lyrics line-----------
    //- Why stable sort?
    //  Because there might be some frames at the same time. Display them.
    qStableSort(lyricsData.begin(), lyricsData.end(), frameLessThan);
    //Combine the lyrics.
    QMap<qint64, QString> lyricsMap;
    for(int i=0; i<lyricsData.size(); i++)
    {
        lyricsMap[lyricsData[i].position]=
                (lyricsMap[lyricsData[i].position].isEmpty()?"":lyricsMap[lyricsData[i].position]+'\n')+
                lyricsData[i].text;
    }
    //Export the position and the text.
    positions=lyricsMap.keys();
    lyricsText=lyricsMap.values();
}

void KNMusicLRCParser::parseFrame(const QString &frame,
                                  const QString &lineData,
                                  QList<LRCFrame> &lyricsData,
                                  QMap<int, QString> &properties)
{
    //Remove the first '[' and ']'.
    QString frameData=frame.mid(1, frame.length()-2);
    //Get the data before the first colon, judge it's number or not.
    int colonPos=frameData.indexOf(':');
    if(colonPos==-1)
    {
        //Cannot find ':', this is frame is obsolete.
        return;
    }
    //Get the data before the first colon.
    QString testData=frameData.left(colonPos).toLower();
    //Search header text in header, if has find.
    int frameIndex=m_headerText.indexOf(testData);
    if(frameIndex!=-1)
    {
        properties[frameIndex]=frameData.mid(colonPos+1);
        return;
    }
    //Judge the test data is number or not.
    int secondChar=frameData.indexOf(QRegularExpression("[^0-9]"), colonPos+1);
    //If it's not a number, means it cannot be a time, obsolete.
    if(secondChar==-1)
    {
        return;
    }
    LRCFrame currentFrame;
    currentFrame.position=testData.toLongLong()*60000+
            frameData.mid(colonPos+1, secondChar-colonPos-1).toLongLong()*1000+
            frameData.mid(secondChar+1).toLongLong()*10;
    currentFrame.text=lineData;
    lyricsData.append(currentFrame);
}

bool KNMusicLRCParser::frameLessThan(const LRCFrame &frameLeft,
                                     const LRCFrame &frameRight)
{
    return frameLeft.position<frameRight.position;
}
