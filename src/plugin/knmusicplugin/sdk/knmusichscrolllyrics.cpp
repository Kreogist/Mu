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
#include <QTimeLine>
#include <QPainter>

#include "knmusiclyricsbackend.h"

#include "knmusichscrolllyrics.h"

#include <QDebug>

#define MaximumDuration 200
#define MinimumWidth 10

KNMusicHScrollLyrics::KNMusicHScrollLyrics(QWidget *parent) :
    KNMusicLyricsBase(parent),
    m_backend(nullptr),
    m_moveToCurrentLine(new QTimeLine(MaximumDuration, this)),
    m_currentLine(-1),
    m_centerOffset(0),
    m_spacing(5),
    m_alignment(Qt::AlignLeft)
{
    //Configure the moving time line.
    m_moveToCurrentLine->setUpdateInterval(10);
    m_moveToCurrentLine->setEasingCurve(QEasingCurve::Linear);
    m_moveToCurrentLine->setStartFrame(0);
    connect(m_moveToCurrentLine, &QTimeLine::frameChanged,
            this, &KNMusicHScrollLyrics::onActionLyricsMoved);
}

void KNMusicHScrollLyrics::reset()
{
    //Stop the time line.
    m_moveToCurrentLine->stop();
    //Reset the center offset.
    m_currentLine=-1;
    m_centerOffset=0;
    //Update the widget.
    update();
}

void KNMusicHScrollLyrics::paintEvent(QPaintEvent *event)
{
    //Ignore the original event.
    Q_UNUSED(event);
    //Check the test list is empty or not.
    if(m_backend==nullptr || m_backend->isEmpty())
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    //Set the painter's hints.
    painter.setRenderHints(QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Initial parameters.
    int currentLineLeft=(width()>>1)-m_centerOffset, paintingLine=m_currentLine,
        lineWidth=MinimumWidth;
    //Check if the current line is vaild.
    if(paintingLine>-1 && paintingLine<=m_backend->lastLine())
    {
        //Draw the current line text.
        lineWidth=
                qMax(MinimumWidth,
                     fontMetrics().width(m_backend->lyricsText(paintingLine)));
        //Draw the text.
        painter.setPen(palette().color(QPalette::Highlight));
        painter.drawText(currentLineLeft,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         m_backend->lyricsText(paintingLine));
    }
    //Draw the normal text.
    painter.setPen(palette().color(QPalette::Text));
    //Draw the lines after the current lines.
    int otherLineLeft=currentLineLeft+lineWidth+m_spacing;
    paintingLine++; //Move to the next line.
    while(otherLineLeft<width() && paintingLine<=m_backend->lastLine())
    {
        //Draw the current line.
        lineWidth=fontMetrics().width(m_backend->lyricsText(paintingLine));
        painter.drawText(otherLineLeft,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         m_backend->lyricsText(paintingLine));
        //Move to the next line.
        otherLineLeft+=lineWidth+m_spacing;
        paintingLine++;
    }
    //Draw the lines before the current lines.
    paintingLine=m_currentLine-1;
    int otherLineRight=currentLineLeft;
    while(otherLineRight>0 && paintingLine>-1)
    {
        //Draw the current line.
        lineWidth=fontMetrics().width(m_backend->lyricsText(paintingLine));
        //MAGIC: the 'next' previous line's right is the current line's left,
        // moving the calculation here.
        otherLineRight-=lineWidth+m_spacing;
        painter.drawText(otherLineRight,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         m_backend->lyricsText(paintingLine));
        //Move to previous line.
        paintingLine--;
    }
}

void KNMusicHScrollLyrics::onActionLyricsMoved(const int &frame)
{
    //Update the current line offset.
    m_centerOffset=frame;
    //Redraw the widget.
    update();
}

Qt::Alignment KNMusicHScrollLyrics::alignment() const
{
    return m_alignment;
}

void KNMusicHScrollLyrics::setAlignment(Qt::Alignment alignment)
{
    m_alignment = alignment;
}


int KNMusicHScrollLyrics::spacing() const
{
    return m_spacing;
}

void KNMusicHScrollLyrics::setSpacing(int spacing)
{
    //Only save the configure when spacing is greater than 0.
    m_spacing=spacing>0?spacing:0;
}

KNMusicLyricsBackend *KNMusicHScrollLyrics::backend()
{
    return m_backend;
}

void KNMusicHScrollLyrics::setBackend(KNMusicLyricsBackend *backend)
{
    //Check the previous backend.
    if(m_backend)
    {
        //Remove the connection of the lyrics widget to the previous backend.
        disconnect(m_backend, &KNMusicLyricsBackend::requireMoveTo,
                   this, &KNMusicHScrollLyrics::moveToLine);
        disconnect(m_backend, SIGNAL(lyricsChanged()),
                   this, SLOT(update()));
        disconnect(m_backend, &KNMusicLyricsBackend::lyricsChanged,
                   this, &KNMusicHScrollLyrics::reset);
    }
    //Save the backend.
    m_backend = backend;
    //Check the backend.
    if(m_backend)
    {
        //Link the backend to this widget.
        connect(m_backend, &KNMusicLyricsBackend::requireMoveTo,
                this, &KNMusicHScrollLyrics::moveToLine);
        connect(m_backend, SIGNAL(lyricsChanged()),
                this, SLOT(update()));
        connect(m_backend, &KNMusicLyricsBackend::lyricsChanged,
                this, &KNMusicHScrollLyrics::reset);
    }
}

void KNMusicHScrollLyrics::moveToLine(const int &lineIndex)
{
    //If the line index is the same as the current, we won't need to move.
    if(lineIndex==m_currentLine || (m_backend==nullptr))
    {
        return;
    }
    //Stop the time line first.
    m_moveToCurrentLine->stop();
    //Calculate the displacement.
    int lyricsDisplacement=0;
    //Check the line index is greater or smaller than the current index.
    if(lineIndex>m_currentLine)
    {
        //Calculate the distance of the lyrics lines which will be passed.
        for(int i=m_currentLine+1; i<=lineIndex; ++i)
        {
            //Increase the height as the distance.
            lyricsDisplacement+=
                    (fontMetrics().width(m_backend->lyricsText(i)) + m_spacing);
        }
    }
    else
    {
        //Calculate the distance of the lyrics lines which will be passed.
        for(int i=lineIndex; i<m_currentLine; ++i)
        {
            //Reduce the width as the distance.
            lyricsDisplacement-=
                    (fontMetrics().width(m_backend->lyricsText(i)) + m_spacing);
        }
    }
    //Save the current line.
    m_currentLine=lineIndex;
    //Update the duration of the animation.
    //Policy:
    /*
     * 1. For normal case, we the lyrics duration is much larger than the
     * maximum duration, so it should be the maximum duration of the animation.
     * 2. When the lyrics is smaller than the maximum duration, set the it to
     * lyrics duration, but if the lyrics duration is to small(like 0), just set
     * it to the minimum update interval.
     */
    m_moveToCurrentLine->setDuration(
                qMax(m_backend->lyricsDuration(m_currentLine),
                     MaximumDuration));
    //Set the start position.
    m_moveToCurrentLine->setEndFrame(lyricsDisplacement);
    //Start the animation.
    m_moveToCurrentLine->start();
}
