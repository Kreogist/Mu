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
#include "knmusicglobal.h"
#include "knmusicbackend.h"

#include "knmusichscrolllyrics.h"

#include <QDebug>

#define MinimumDuration 50
#define MinimumWidth 10

KNMusicHScrollLyrics::KNMusicHScrollLyrics(QWidget *parent) :
    KNMusicLyricsBase(parent),
    m_backend(nullptr),
    m_moveToCurrentLine(new QTimeLine(MinimumDuration, this)),
    m_currentLine(-1),
    m_centerOffset(0),
    m_spacing(10),
    m_alignment(Qt::AlignLeft)
{
    //Configure the moving time line.
    m_moveToCurrentLine->setUpdateInterval(16);
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
    //Initial the painter.
    QPainter painter(this);
    //Set the painter's hints.
    painter.setRenderHints(QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Check the test list is empty or not.
    if(m_backend==nullptr || m_backend->isEmpty())
    {
        //Draw the place holder text.
        painter.drawText(rect(),
                         Qt::AlignCenter,
                         m_placeHolderText);
        //Finished painting.
        return;
    }
    //Initial parameters.
    int currentLineLeft=(width()>>1)-m_centerOffset, paintingLine=m_currentLine,
        lineWidth=MinimumWidth;
    //Check if the current line is vaild.
    if(paintingLine>-1 && paintingLine<=m_backend->lastLine())
    {
        //Get the painting line.
        QString &&currentText=m_backend->lyricsText(paintingLine).simplified();
        //Draw the current line text.
        lineWidth=
                qMax(MinimumWidth,
                     fontMetrics().width(currentText));
        //Draw the text.
        painter.setPen(palette().color(QPalette::Highlight));
        painter.drawText(currentLineLeft,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         currentText);
    }
    //Draw the normal text.
    painter.setPen(palette().color(QPalette::Text));
    //Draw the lines after the current lines.
    int otherLineLeft=currentLineLeft+lineWidth+m_spacing;
    paintingLine++; //Move to the next line.
    while(otherLineLeft<width() && paintingLine<=m_backend->lastLine())
    {
        //Get the painting line.
        QString &&currentText=m_backend->lyricsText(paintingLine).simplified();
        //Draw the current line.
        lineWidth=qMax(MinimumWidth, fontMetrics().width(currentText));
        painter.drawText(otherLineLeft,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         currentText);
        //Move to the next line.
        otherLineLeft+=lineWidth+m_spacing;
        paintingLine++;
    }
    //Draw the lines before the current lines.
    paintingLine=m_currentLine-1;
    int otherLineRight=currentLineLeft;
    while(otherLineRight>0 && paintingLine>-1)
    {
        //Get the painting line.
        QString &&currentText=m_backend->lyricsText(paintingLine).simplified();
        //Draw the current line.
        lineWidth=qMax(MinimumWidth, fontMetrics().width(currentText));
        //MAGIC: the 'next' previous line's right is the current line's left,
        // moving the calculation here.
        otherLineRight-=lineWidth+m_spacing;
        painter.drawText(otherLineRight,
                         0,
                         lineWidth,
                         height(),
                         Qt::AlignVCenter,
                         currentText);
        //Move to previous line.
        paintingLine--;
    }
}

void KNMusicHScrollLyrics::onActionLyricsMoved(int frame)
{
    //Update the current line offset.
    m_centerOffset=frame;
    //Redraw the widget.
    update();
}

QString KNMusicHScrollLyrics::placeHolderText() const
{
    return m_placeHolderText;
}

void KNMusicHScrollLyrics::setPlaceHolderText(const QString &placeHolderText)
{
    //Save the place holder text.
    m_placeHolderText = placeHolderText;
    //Update the widget.
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
        disconnect(m_backend, SIGNAL(updateWidget()),
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
        connect(m_backend, SIGNAL(updateWidget()),
                this, SLOT(update()));
        connect(m_backend, &KNMusicLyricsBackend::lyricsChanged,
                this, &KNMusicHScrollLyrics::reset);
    }
}

void KNMusicHScrollLyrics::moveToLine(int lineIndex, const qint64 &position)
{
    //If the line index is the same as the current, we won't need to move.
    if(lineIndex==m_currentLine || (m_backend==nullptr))
    {
        return;
    }
    //Stop the time line first.
    m_moveToCurrentLine->stop();
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
    //Calculate the duration.
    qint64 targetDuration=qMax(m_backend->lyricsDuration(m_currentLine),
                               MinimumDuration),
           currentPosition=position-m_backend->lyricsPosition(m_currentLine);
    //Check out the target duration, if it's the last line, then make the
    //duration to be the whole left.
    if(m_currentLine==m_backend->lastLine() && knMusicGlobal->backend())
    {
        //Calculate the new duration.
        targetDuration=
                knMusicGlobal->backend()->duration() -
                m_backend->lyricsPosition(m_currentLine);
    }
    //Set the duration, make the minimum duration to be 2 miliseconds.
    m_moveToCurrentLine->setDuration(qMax(targetDuration-currentPosition,
                                          (qint64)2));
    //Calculate the total width.
    int totalWidth=qMax(MinimumWidth,
                        fontMetrics().width(m_backend->lyricsText(lineIndex))) +
                   m_spacing;
    //Set the start position.
    m_moveToCurrentLine->setStartFrame(
                (qreal)totalWidth*(qreal)currentPosition/(qreal)targetDuration);
    //Set the end position.
    m_moveToCurrentLine->setEndFrame(totalWidth);
    //Start the animation.
    m_moveToCurrentLine->start();
}

void KNMusicHScrollLyrics::setPause(bool pause)
{
    //Check the running state.
    if(m_moveToCurrentLine->state()!=QTimeLine::NotRunning)
    {
        //Change the pausing state.
        m_moveToCurrentLine->setPaused(pause);
    }
}
