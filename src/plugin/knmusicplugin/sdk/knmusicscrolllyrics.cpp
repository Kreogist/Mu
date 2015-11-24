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

#include "knmusiclyricsbackend.h"

#include "knmusicscrolllyrics.h"

#include <QDebug>

#define MaximumDuration 200

KNMusicScrollLyrics::KNMusicScrollLyrics(QWidget *parent) :
    KNMusicLyricsBase(parent),
    m_backend(nullptr),
    m_moveToCurrentLine(new QTimeLine(MaximumDuration, this)),
    m_currentLine(-1),
    m_centerOffset(0),
    m_leftSpacing(15),
    m_spacing(2),
    m_alignment(Qt::AlignLeft)
{
    //Configure the moving time line.
    m_moveToCurrentLine->setUpdateInterval(10);
    m_moveToCurrentLine->setEasingCurve(QEasingCurve::OutCubic);
    m_moveToCurrentLine->setEndFrame(0);
    connect(m_moveToCurrentLine, &QTimeLine::frameChanged,
            this, &KNMusicScrollLyrics::onActionLyricsMoved);
}
void KNMusicScrollLyrics::reset()
{
    //Stop the time line.
    m_moveToCurrentLine->stop();
    //Reset the center offset.
    m_currentLine=-1;
    m_centerOffset=0;
    //Update the widget.
    update();
}

void KNMusicScrollLyrics::paintEvent(QPaintEvent *event)
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
    int currentLineTop=(height()>>1)+m_centerOffset, paintingLine=m_currentLine;
    QRect lineRect;
    //Check if the current line is vaild.
    if(paintingLine>-1 && paintingLine<=m_backend->lastLine())
    {
        //Draw the current line text.
        lineRect=lyricsRect(m_backend->lyricsText(paintingLine));
        currentLineTop-=(lineRect.height()>>1);
        lineRect.moveTop(currentLineTop);
        //Draw the text.
        painter.setPen(palette().color(QPalette::Highlight));
        painter.drawText(lineRect.x(),
                         lineRect.y(),
                         width()-m_leftSpacing,
                         lineRect.height(),
                         m_alignment | Qt::TextWordWrap,
                         m_backend->lyricsText(paintingLine));
    }
    else
    {
        //Draw a empty line.
        lineRect=QRect(m_leftSpacing,
                       0,
                       width()-m_leftSpacing,
                       fontMetrics().height());
        currentLineTop-=(lineRect.height()>>1);
    }
    //Draw the normal text.
    painter.setPen(palette().color(QPalette::Text));
    //Draw the lines after the current lines.
    int otherLineTop=currentLineTop+lineRect.height()+m_spacing;
    paintingLine++; //Move to the next line.
    while(otherLineTop<height() && paintingLine<=m_backend->lastLine())
    {
        //Draw the current line.
        lineRect=lyricsRect(m_backend->lyricsText(paintingLine));
        lineRect.moveTop(otherLineTop);
        painter.drawText(lineRect.x(),
                         lineRect.y(),
                         width()-m_leftSpacing,
                         lineRect.height(),
                         m_alignment | Qt::TextWordWrap,
                         m_backend->lyricsText(paintingLine));
        //Move to the next line.
        otherLineTop+=lineRect.height()+m_spacing;
        paintingLine++;
    }
    //Draw the lines before the current lines.
    paintingLine=m_currentLine-1;
    int otherLineBottom=currentLineTop;
    while(otherLineBottom>0 && paintingLine>-1)
    {
        //Draw the current line.
        lineRect=lyricsRect(m_backend->lyricsText(paintingLine));
        //MAGIC: the 'next' previous line's bottom is the current line's top,
        // moving the calculation here.
        otherLineBottom-=lineRect.height()+m_spacing;
        lineRect.moveTop(otherLineBottom);
        painter.drawText(lineRect.x(),
                         lineRect.y(),
                         width()-m_leftSpacing,
                         lineRect.height(),
                         m_alignment | Qt::TextWordWrap,
                         m_backend->lyricsText(paintingLine));
        //Move to previous line.
        paintingLine--;
    }
}

void KNMusicScrollLyrics::onActionLyricsMoved(const int &frame)
{
    //Update the current line offset.
    m_centerOffset=frame;
    //Redraw the widget.
    update();
}

Qt::Alignment KNMusicScrollLyrics::alignment() const
{
    return m_alignment;
}

void KNMusicScrollLyrics::setAlignment(const Qt::Alignment &alignment)
{
    m_alignment = alignment;
}


int KNMusicScrollLyrics::spacing() const
{
    return m_spacing;
}

void KNMusicScrollLyrics::setSpacing(int spacing)
{
    //Only save the configure when spacing is greater than 0.
    m_spacing=spacing>0?spacing:0;
}

KNMusicLyricsBackend *KNMusicScrollLyrics::backend()
{
    return m_backend;
}

void KNMusicScrollLyrics::setBackend(KNMusicLyricsBackend *backend)
{
    //Check the previous backend.
    if(m_backend)
    {
        //Remove the connection of the lyrics widget to the previous backend.
        disconnect(m_backend, &KNMusicLyricsBackend::requireMoveTo,
                   this, &KNMusicScrollLyrics::moveToLine);
        disconnect(m_backend, SIGNAL(lyricsChanged()),
                   this, SLOT(update()));
        disconnect(m_backend, &KNMusicLyricsBackend::lyricsChanged,
                   this, &KNMusicScrollLyrics::reset);
    }
    //Save the backend.
    m_backend = backend;
    //Check the backend.
    if(m_backend)
    {
        //Link the backend to this widget.
        connect(m_backend, &KNMusicLyricsBackend::requireMoveTo,
                this, &KNMusicScrollLyrics::moveToLine);
        connect(m_backend, SIGNAL(lyricsChanged()),
                this, SLOT(update()));
        connect(m_backend, &KNMusicLyricsBackend::lyricsChanged,
                this, &KNMusicScrollLyrics::reset);
    }
}

void KNMusicScrollLyrics::moveToLine(int lineIndex, const qint64 &position)
{
    Q_UNUSED(position)
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
            lyricsDisplacement+=lyricsRect(m_backend->lyricsText(i)).height();
        }
    }
    else
    {
        //Calculate the distance of the lyrics lines which will be passed.
        for(int i=lineIndex; i<m_currentLine; ++i)
        {
            //Reduce the height as the distance.
            lyricsDisplacement-=lyricsRect(m_backend->lyricsText(i)).height();
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
                qMin(qMax(m_backend->lyricsDuration(m_currentLine), 2),
                     MaximumDuration));
    //Set the start position.
    m_moveToCurrentLine->setStartFrame(lyricsDisplacement);
    //Start the animation.
    m_moveToCurrentLine->start();
}

