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
#include <QTimeLine>
#include <QPainter>

#include "knmusicscrolllyrics.h"

#include <QDebug>

KNMusicScrollLyrics::KNMusicScrollLyrics(QWidget *parent) :
    KNMusicLyricsBase(parent)
{
    //Initial the time line.
    m_moveToCurrentLine=new QTimeLine(m_maximumDuration, this);
    m_moveToCurrentLine->setUpdateInterval(2);
    m_moveToCurrentLine->setEasingCurve(QEasingCurve::OutCubic);
    m_moveToCurrentLine->setEndFrame(0);
    connect(m_moveToCurrentLine, &QTimeLine::frameChanged,
            this, &KNMusicScrollLyrics::onActionLyricsMoved);
}

void KNMusicScrollLyrics::onActionPositionChange(const qint64 &position)
{
    //If the lyrics list is empty,.do nothing.
    if(m_positions.isEmpty())
    {
        return;
    }
    //Now the positions shouldn't be empty, and text shouldn't be emtpy as well.
    //Check the current line. If it's out of range, just update the widget.
    if(m_currentLine>m_lastLine)
    {
        update();
        return;
    }
    //If it's smaller than 0, check it with the first position.
    if(m_currentLine<0)
    {
        if(position<m_positions.at(0))
        {
            update();
            return;
        }
        m_currentLine=0;
        update();
        return;
    }
    //Now the line should be in the range.
    int lyricsDisplacement=0;
    //Check the position is before the current timestamp or not.
    if(position<m_positions.at(m_currentLine))
    {
        //Find the lyrics.
        while(m_currentLine>-1 && position<=m_positions.at(m_currentLine))
        {
            lyricsDisplacement-=lyricsSize(m_texts.at(m_currentLine)).height();
            m_currentLine--;
        }
        //Start moving lyrics.
        startMove(lyricsDuration(m_currentLine),
                  lyricsDisplacement);
        return;
    }
    //Check whether we need to move the lyrics.
    //When the current line is the last, or the position is not larger than the
    //next line, we will do nothing.
    if(m_currentLine==m_lastLine ||
            position<m_positions.at(m_currentLine+1))
    {
        return;
    }
    //Find the next position.
    //Why use while to find? User may seek the position a lot.
    while(m_currentLine<m_lastLine && position>m_positions.at(m_currentLine+1))
    {
        lyricsDisplacement+=lyricsSize(m_texts.at(m_currentLine)).height();
        m_currentLine++;
    }
    //Start moving lyrics.
    startMove(lyricsDuration(m_currentLine),
              lyricsDisplacement);
}

void KNMusicScrollLyrics::onActionLyricsReset()
{
    //Reset the current information first.
    resetLyricsData();
    //Update the panel.
    update();
}

void KNMusicScrollLyrics::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Check the test list is empty or not.
    if(m_texts.isEmpty())
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing, true);
    //Initial parameters.
    int currentLineTop=(height()>>1)+m_centerOffset,
        paintingLine=m_currentLine;
    QSize lineSize;
    //Check if the current line is vaild.
    if(paintingLine>-1 && paintingLine<=m_lastLine)
    {
        //Draw the current line text.
        lineSize=lyricsSize(m_texts.at(paintingLine));
        currentLineTop-=(lineSize.height()>>1);
        //Draw the text.
        painter.setPen(m_highlightColor);
        painter.drawText(m_leftSpacing,
                         currentLineTop,
                         width(),
                         lineSize.height(),
                         Qt::AlignLeft,
                         m_texts.at(paintingLine));
    }
    else
    {
        //Draw a empty line.
        lineSize=QSize(0, fontMetrics().height());
        currentLineTop-=(lineSize.height()>>1);
    }
    painter.setPen(m_normalText);
    //Draw the lines after the current lines.
    int otherLineTop=currentLineTop+lineSize.height()+m_spacing;
    paintingLine++; //Move to the next line.
    while(otherLineTop<height() && paintingLine<=m_lastLine)
    {
        //Draw the current line.
        lineSize=lyricsSize(m_texts.at(paintingLine));
        painter.drawText(m_leftSpacing,
                         otherLineTop,
                         width(),
                         lineSize.height(),
                         Qt::AlignLeft,
                         m_texts.at(paintingLine));
        //Move to the next line.
        otherLineTop+=lineSize.height()+m_spacing;
        paintingLine++;
    }
    //Draw the lines before the current lines.
    paintingLine=m_currentLine-1;
    int otherLineBottom=currentLineTop;
    while(otherLineBottom>0 && paintingLine>-1)
    {
        //Draw the current line.
        lineSize=lyricsSize(m_texts.at(paintingLine));
        //MAGIC: the 'next' previous line's bottom is the current line's top,
        // moving the calculation here.
        otherLineBottom-=lineSize.height()+m_spacing;
        painter.drawText(m_leftSpacing,
                         otherLineBottom,
                         width(),
                         lineSize.height(),
                         Qt::AlignLeft,
                         m_texts.at(paintingLine));
        //Move to previous line.
        paintingLine--;
    }
}

void KNMusicScrollLyrics::setLyricsData(const QList<qint64> positions,
                                        const QStringList texts)
{
    //Reset the lyrics data.
    resetLyricsData();
    //Check the size of position list and text list are the same or not.
    if(positions.size()!=texts.size())
    {
        return;
    }
    //Save the position and texts.
    m_positions=positions;
    m_texts=texts;
    //Update the parameters.
    m_lastLine=m_positions.size()-1;
    //Update the viewer.
    update();
}

void KNMusicScrollLyrics::onActionLyricsMoved(const int &frame)
{
    //Update the current line offset, redraw the widget.
    m_centerOffset=frame;
    update();
}

inline void KNMusicScrollLyrics::resetLyricsData()
{
    //Clear lyrics data.
    m_positions.clear();
    m_texts.clear();
    //Reset lyrics status data.
    m_currentLine=-1;
    m_lastLine=-1;
    m_centerOffset=0;
}

inline void KNMusicScrollLyrics::startMove(const int &lineDuration,
                                           const int &displacement)
{
    //Stop the time line first.
    m_moveToCurrentLine->stop();
    //Update the duration of the animation.
    //Policy:
    /*
     * 1. For normal case, we the lyrics duration is much larger than the
     * maximum duration, so it should be the maximum duration of the animation.
     * 2. When the lyrics is smaller than the maximum duration, set the it to
     * lyrics duration, but if the lyrics duration is to small(like 0), just set
     * it to the minimum update interval.
     */
    m_moveToCurrentLine->setDuration(qMin(qMax(lineDuration, 2),
                                          m_maximumDuration));
    m_moveToCurrentLine->setStartFrame(displacement);
    //Start the animation.
    m_moveToCurrentLine->start();
}

inline int KNMusicScrollLyrics::lyricsDuration(const int &index)
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
int KNMusicScrollLyrics::spacing() const
{
    return m_spacing;
}

void KNMusicScrollLyrics::setSpacing(int spacing)
{
    m_spacing = spacing;
}

