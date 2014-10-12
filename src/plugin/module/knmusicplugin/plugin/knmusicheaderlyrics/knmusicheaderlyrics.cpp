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
#include <QPaintEvent>
#include <QPainter>

#include "knmusicheaderplayerbase.h"
#include "knmusiclyricsmanager.h"

#include "knmusicheaderlyrics.h"

#include <QDebug>

KNMusicHeaderLyrics::KNMusicHeaderLyrics(QWidget *parent) :
    KNMusicHeaderLyricsBase(parent)
{
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicLyricsManager::instance();

    //Initial the lyrics moving time line.
    m_moveToCurrent=new QTimeLine(200, this);
    m_moveToCurrent->setUpdateInterval(5);
    m_moveToCurrent->setEasingCurve(QEasingCurve::OutCubic);
    m_moveToCurrent->setEndFrame(0);
    connect(m_moveToCurrent, &QTimeLine::frameChanged,
            this, &KNMusicHeaderLyrics::onActionLyricsMoved);
}

KNMusicHeaderLyrics::~KNMusicHeaderLyrics()
{
    delete m_lyricsManager;
}

void KNMusicHeaderLyrics::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    connect(player, &KNMusicHeaderPlayerBase::requireLoadLyrics,
            this, &KNMusicHeaderLyricsBase::loadLyricsForMusic);
    connect(player, &KNMusicHeaderPlayerBase::positionChanged,
            this, &KNMusicHeaderLyrics::onActionPositionChange);
}

void KNMusicHeaderLyrics::reset()
{
    //Get the lyrics lines.
    m_lyricsLines=m_lyricsManager->lines();
    //Initial the current line to the first line.
    m_currentLyricsLine=(m_lyricsLines>-1)?0:-1;
    //Move the first line to center.
    onActionLyricsMoved(0);
}

void KNMusicHeaderLyrics::loadLyricsForMusic(const QString &filePath)
{
    //Load the lyrics.
    m_lyricsManager->loadLyricsForFile(filePath);
    //Reset the lyrics viewer.
    reset();
}

void KNMusicHeaderLyrics::onActionPositionChange(const qint64 &position)
{
    //Check the position is previous than the current yet.
    if(position<m_lyricsManager->positionAt(m_currentLyricsLine))
    {
        //Find the matching lyrics.
        while(m_lyricsManager->positionAt(m_currentLyricsLine)>position)
        {
            m_currentLyricsLine--;
        }
        //Update and return.
        update();
        return;
    }
    //Find the next specific position.
    while(m_lyricsManager->positionAt(m_currentLyricsLine+1)<position)
    {
        m_currentLyricsLine++;
    }
    update();
}

void KNMusicHeaderLyrics::paintEvent(QPaintEvent *event)
{
    //Paint other things.
    QWidget::paintEvent(event);
    //Check is current line available, if not means no lyrics.
    if(m_currentLyricsLine==-1)
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    QSize currentSize;
    QString currentText;
    int centerY=(height()>>1)+m_currentLineOffsetY;
    //Draw the current line.
    painter.setPen(m_highlightColor);
    currentText=m_lyricsManager->lyricsAt(m_currentLyricsLine);
    currentSize=lyricsSize(currentText);
    centerY-=(currentSize.height()>>1);
    painter.drawText(m_leftSpacing,
                     centerY,
                     width(),
                     currentSize.height(),
                     Qt::AlignLeft,
                     currentText);
    //Draw other lyrics.
    painter.setPen(m_normalText);
    //Draw down lines.
    int lineTop=centerY+currentSize.height(),
        paintLine=m_currentLyricsLine+1;
    while(lineTop<height() && paintLine<m_lyricsLines)
    {
        //Draw the line.
        currentText=m_lyricsManager->lyricsAt(paintLine);
        currentSize=lyricsSize(currentText);
        painter.drawText(m_leftSpacing,
                         lineTop,
                         width(),
                         currentSize.height(),
                         Qt::AlignLeft,
                         currentText);
        //To the next line.
        paintLine++;
        lineTop+=currentSize.height();
    }
    //Draw up lines.
    int lineBottom=centerY;
    paintLine=m_currentLyricsLine-1;
    while(lineBottom>0 && paintLine>0)
    {
        //Draw the line.
        currentText=m_lyricsManager->lyricsAt(paintLine);
        currentSize=lyricsSize(currentText);
        //MAGIC: the line bottom is current line's top, so calculate here.
        lineBottom-=currentSize.height();
        painter.drawText(m_leftSpacing,
                         lineBottom,
                         width(),
                         currentSize.height(),
                         Qt::AlignLeft,
                         currentText);
        //To the previous line.
        paintLine--;
    }
}

void KNMusicHeaderLyrics::onActionLyricsMoved(const int &frame)
{
    //Update the offset.
    m_currentLineOffsetY=frame;
    //Redraw the display.
    update();
}
