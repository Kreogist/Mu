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

#include "knmusiclyricsbackend.h"

#include <QDebug>

KNMusicLyricsBackend::KNMusicLyricsBackend(QObject *parent) :
    QObject(parent)
{
}

int KNMusicLyricsBackend::lyricsDuration(const int &index)
{
    //Check if the position list contains less than two lines.
    if(m_positions.size()<2)
    {
        return 0;
    }
    //There're three kinds of index: invaild, normal and the last.
    //--Invaild lines--
    //If the index is smaller than 0, treat it as 0.
    if(index<0)
    {
        return m_positions.at(1)-m_positions.at(0);
    }
    //--The last line and other invaild lines--
    if(index>=m_lastLine)
    {
        return 0;
    }
    //--Normal lines--
    return m_positions.at(index+1)-m_positions.at(index);
}

qint64 KNMusicLyricsBackend::lyricsPosition(const int &index)
{
    //Check the index range.
    if(index>-1 && index<m_positions.size())
    {
        //Give back the position.
        return m_positions.at(index);
    }
    //Give an invalid value when the index is invalid.
    return -1;
}

QString KNMusicLyricsBackend::lyricsText(const int &index)
{
    //Check and ensure the index is valid.
    if(index>-1 && index<m_positions.size())
    {
        //For all the valid index, return the data.
        return m_texts.at(index);
    }
    //For all the invalid index, return a null string.
    return QString();
}

void KNMusicLyricsBackend::setPosition(const qint64 &position)
{
    //If the lyrics list is empty,.do nothing.
    //If the lyrics is not empty, check the current line. If it's out of range,
    //just update the widget.
    if(m_positions.isEmpty() || m_currentLine>m_lastLine)
    {
        return;
    }
    //If it's smaller than 0, check it with the first position.
    if(m_currentLine<0)
    {
        //If the position is not still start, just update the lyrics.
        if(position<m_positions.at(0))
        {
            //Require update all the widget.
            //!FIXME: called all the update() function;
            return;
        }
        //Or else, now it should be start at the first line.
        m_currentLine=0;
        //Ask lyrics widget to move the first line of lyrics to the center.
        emit requireMoveTo(0, position);
        //Finish check.
        return;
    }
    //Now the line should be in the range.
    //Sept 5th, 2015: DO NOT change the order of the following lines.
    //The position may is much before the current one(like when user rewinding
    //the music).
    //Check the position is before the current timestamp.
    if(position<m_positions.at(m_currentLine))
    {
        //Find the lyrics one by one until to the first line.
        while(m_currentLine>-1 && position<=m_positions.at(m_currentLine))
        {
            //Move the previous line.
            m_currentLine--;
        }
        //Now we should find the right line, start the animation to move to it.
        emit requireMoveTo(m_currentLine, position);
        //Finish check.
        return;
    }
    //Check whether we need to move the lyrics.
    //When the current line is the last, or the position is not larger than the
    //next line, we will do nothing.
    if(m_currentLine==m_lastLine || position<m_positions.at(m_currentLine+1))
    {
        return;
    }
    //Find the next position.
    //Why use while to find? User may seek the position a lot.
    while(m_currentLine<m_lastLine && position>m_positions.at(m_currentLine+1))
    {
        //Move to the next line.
        m_currentLine++;
    }
    //Start moving lyrics.
    emit requireMoveTo(m_currentLine, position);
}

bool KNMusicLyricsBackend::isEmpty() const
{
    return m_positions.isEmpty();
}

int KNMusicLyricsBackend::lastLine() const
{
    return m_lastLine;
}

void KNMusicLyricsBackend::reset()
{
    //Clear lyrics data.
    m_positions.clear();
    m_texts.clear();
    //Reset lyrics status data.
    m_currentLine=-1;
    m_lastLine=-1;
    //Emit lyrics changed data to sync the data.
    emit lyricsChanged();
}

void KNMusicLyricsBackend::setLyricsData(const QList<qint64> &positions,
                                          const QStringList &texts)
{
    //Reset the lyrics data.
    reset();
    //Check the validation of the position and text list: the size of position
    //list and text list should be the same.
    if(positions.size()!=texts.size())
    {
        return;
    }
    //Save the position and texts.
    m_positions=positions;
    m_texts=texts;
    //Update the last line parameters.
    m_lastLine=m_positions.size()-1;
    //Emit lyrics changed signal to synchronise the data.
    emit lyricsChanged();
}
