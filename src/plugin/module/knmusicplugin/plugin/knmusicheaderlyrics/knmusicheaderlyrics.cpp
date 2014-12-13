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
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>

#include "knmusicheaderplayerbase.h"
#include "knmusiclyricsmanager.h"

#include "knlocalemanager.h"

#include "knmusicheaderlyrics.h"

#include <QDebug>

KNMusicHeaderLyrics::KNMusicHeaderLyrics(QWidget *parent) :
    KNMusicHeaderLyricsBase(parent)
{
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicLyricsManager::instance();

    //Initial the lyrics moving time line.
    m_moveToCurrent=new QTimeLine(m_animationDuration, this);
    m_moveToCurrent->setUpdateInterval(2);
    m_moveToCurrent->setEasingCurve(QEasingCurve::OutCubic);
    m_moveToCurrent->setEndFrame(0);
    connect(m_moveToCurrent, &QTimeLine::frameChanged,
            this, &KNMusicHeaderLyrics::onActionLyricsMoved);

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicHeaderLyrics::retranslate);
    //Retranslate.
    retranslate();
}

KNMusicHeaderLyrics::~KNMusicHeaderLyrics()
{
    delete m_lyricsManager;
}

void KNMusicHeaderLyrics::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    connect(player, &KNMusicHeaderPlayerBase::playerReset,
            this, &KNMusicHeaderLyrics::resetStatus);
    connect(player, &KNMusicHeaderPlayerBase::requireLoadLyrics,
            this, &KNMusicHeaderLyrics::loadLyricsForMusic);
    connect(player, &KNMusicHeaderPlayerBase::positionChanged,
            this, &KNMusicHeaderLyrics::onActionPositionChange);
}

void KNMusicHeaderLyrics::retranslate()
{
    //Get the latest title and item info.
    KNPreferenceTitleInfo lyricsTitle;
    QList<KNPreferenceItemInfo> itemList;
    generateTitleAndItemInfo(lyricsTitle, itemList);
    //Ask to insert the info list.
    KNMusicGlobal::instance()->insertItemInfoList(lyricsTitle, itemList);
}

void KNMusicHeaderLyrics::resetStatus()
{
    //Clear lines.
    m_lyricsLines=0;
    //Reset lines.
    m_currentLyricsLine=-1;
    //Clear lyrics manager.
    m_lyricsManager->clear();
    //Update the viewport.
    update();
}

void KNMusicHeaderLyrics::loadLyricsForMusic(const QString &filePath)
{
    //Reset the lyrics viewer.
    resetStatus();
    //Load the lyrics.
    if(m_lyricsManager->loadLyricsForFile(filePath))
    {
        //Update parameters.
        //Get the lyrics lines.
        m_lyricsLines=m_lyricsManager->lines();
        //Check is there lyrics.
        if(!m_lyricsManager->isEmpty())
        {
            //Initial the current line to the first line.
            m_currentLyricsLine=0;
            //Move the first line to center.
            onActionLyricsMoved(0);
        }
        //Update the viewer.
        update();
    }
}

void KNMusicHeaderLyrics::onActionPositionChange(const qint64 &position)
{
    //If no lyrics, do nothing.
    if(m_lyricsManager->isEmpty())
    {
        return;
    }
    //If there's lines but current line is -1, or we reach the very end,
    //Find from very beginning.
    if(m_currentLyricsLine<0 || m_currentLyricsLine>=m_lyricsLines)
    {
        //Find current line from the beginning.
        m_currentLyricsLine=0;
        while(m_currentLyricsLine<m_lyricsLines &&
              position<m_lyricsManager->positionAt(m_currentLyricsLine))
        {
            m_currentLyricsLine++;
        }
        update();
        return;
    }
    int yOffset=0;
    //Check the position is previous than the current yet.
    if(position<m_lyricsManager->positionAt(m_currentLyricsLine) &&
            m_currentLyricsLine!=0)
    {
        //Find the matching lyrics.
        while(m_currentLyricsLine>-1 &&
              m_lyricsManager->positionAt(m_currentLyricsLine)>position)
        {
            yOffset-=lyricsSize(m_lyricsManager->lyricsAt(m_currentLyricsLine)).height();
            m_currentLyricsLine--;
        }
        update();
        //Start animation.
        startMovingAnime((lyricsLineDuration(m_currentLyricsLine)>>2),
                         yOffset);
        return;
    }
    //Find the next specific position.
    while(m_currentLyricsLine<m_lyricsLines-2 &&
          m_lyricsManager->positionAt(m_currentLyricsLine+1)<position)
    {
        yOffset+=lyricsSize(m_lyricsManager->lyricsAt(m_currentLyricsLine)).height();
        m_currentLyricsLine++;
    }
    if(yOffset>0)
    {
        startMovingAnime((lyricsLineDuration(m_currentLyricsLine)>>2),
                         yOffset);
    }
}

void KNMusicHeaderLyrics::paintEvent(QPaintEvent *event)
{
    //Paint other things.
    QWidget::paintEvent(event);
    //Check is current line available, if not means no lyrics.
    if(m_lyricsManager->isEmpty() ||
            m_currentLyricsLine<0 ||
            m_currentLyricsLine>=m_lyricsLines)
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
    int spacingHeight=currentSize.height()+m_lineSpacing,
        lineTop=centerY+spacingHeight+m_lineSpacing,
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
        lineTop+=spacingHeight;
    }
    //Draw up lines.
    int lineBottom=centerY-m_lineSpacing;
    paintLine=m_currentLyricsLine-1;
    while(lineBottom>0 && paintLine>-1)
    {
        //Draw the line.
        currentText=m_lyricsManager->lyricsAt(paintLine);
        currentSize=lyricsSize(currentText);
        //MAGIC: the line bottom is current line's top, so calculate here.
        lineBottom-=spacingHeight;
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

void KNMusicHeaderLyrics::generateTitleAndItemInfo(KNPreferenceTitleInfo &listTitle,
                                                   QList<KNPreferenceItemInfo> &list)
{
    //Set the title.
    listTitle.advanced=false;
    listTitle.title=tr("Lyrics");
    listTitle.titleIdentifier="Lyrics";

    //Clear the list.
    list.clear();
    //Add the current info.
    list.append(KNPreferenceItemGlobal::generateInfo(PathEdit,
                                                     tr("Lyrics Folder"),
                                                     "LyricsFolder",
                                                     m_lyricsManager->lyricsFolderPath()));
}

int KNMusicHeaderLyrics::lyricsLineDuration(const int &index)
{
    if(index==-1)
    {
        return 0;
    }
    if(index<m_lyricsLines-1)
    {
        return m_lyricsManager->positionAt(index+1)-
                m_lyricsManager->positionAt(index);
    }
    return m_animationDuration<<2;
}

void KNMusicHeaderLyrics::startMovingAnime(const int &durationOffset,
                                           const int &yOffset)
{
    //Stop the time line.
    m_moveToCurrent->stop();
    //Reset the duration and the start frame.
    m_moveToCurrent->setDuration(qMin(qMax(durationOffset, 1),
                                      m_animationDuration));
    m_moveToCurrent->setStartFrame(yOffset);
    //Before we start, set the widget to the start frame state.
    onActionLyricsMoved(yOffset);
    //Start the animation.
    m_moveToCurrent->start();
}
