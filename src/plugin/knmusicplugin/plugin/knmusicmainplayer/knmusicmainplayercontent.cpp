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
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "knmusicmainplayercontent.h"
#include "knmusicmainplayercontentswitcher.h"

#include <QDebug>

KNMusicMainPlayerContent::KNMusicMainPlayerContent(QWidget *parent) :
    QWidget(parent),
    m_singleIn(generateAnime()),
    m_singleOut(generateAnime()),
    m_doublePlaylist(generateAnime()),
    m_singleAnime(new QParallelAnimationGroup(this)),
    m_singleSwitcher(new KNMusicMainPlayerContentSwitcher(this)),
    m_columnCount(1),
    m_currentIndex(-1),
    m_movingPrevious(false)
{
    //Clear up the widget pointer array.
    m_columnWidgets[0]=nullptr;
    m_columnWidgets[1]=nullptr;
    m_columnWidgets[2]=nullptr;
    //Add animations to anime group.
    m_singleAnime->addAnimation(m_singleIn);
    m_singleAnime->addAnimation(m_singleOut);
    //Link the contect switcher to the content widget.
    connect(m_singleSwitcher, &KNMusicMainPlayerContentSwitcher::requireNext,
            this, &KNMusicMainPlayerContent::onActionSingleToNext);
    connect(m_singleSwitcher,
            &KNMusicMainPlayerContentSwitcher::requirePrevious,
            this, &KNMusicMainPlayerContent::onActionSingleToPrevious);
    connect(m_singleAnime, &QParallelAnimationGroup::finished,
            this, &KNMusicMainPlayerContent::onActionSingleFinished);
}

void KNMusicMainPlayerContent::setColumnCount(int columnCount)
{
    //Checkout the column count.
    if(columnCount>3)
    {
        //Greater than 3 columns, take it as 3 column.
        columnCount=3;
    }
    //If the column is less than 1, there will be at least 1 column.
    else if(columnCount<1)
    {
        //Save the column count.
        columnCount=1;
    }

    //Check out whether the column count is the same or not.
    if(columnCount!=m_columnCount)
    {
        //Save the valid column count.
        m_columnCount=columnCount;
        //Emit the changed signal.
        emit columnCountChanged(m_columnCount);
        //Check out the column count first.
        switch(m_columnCount)
        {
        case 1:
            //Reset the current index to 0.
            m_currentIndex=0;
            m_singleSwitcher->setCurrentIndex(0);
            //Hide the second and the third widget.
            setWidgetVisible(0, true);
            setWidgetVisible(1, false);
            setWidgetVisible(2, false);
            //Show up the switcher.
            m_singleSwitcher->show();
            break;
        case 2:
            //Reset the index to -1.
            m_currentIndex=-1;
            //Hide the single switcher.
            m_singleSwitcher->hide();
            //Show the first and second widget.
            setWidgetVisible(0, true);
            setWidgetVisible(1, true);
            setWidgetVisible(2, false);
            break;
        case 3:
            //Reset the index to -1.
            m_currentIndex=-1;
            //Show all the widgets.
            setWidgetVisible(0, true);
            setWidgetVisible(1, true);
            setWidgetVisible(2, true);
            //Hide the single switcher.
            m_singleSwitcher->hide();
            break;
        }
    }

    //Update the geometry.
    regeometryWidgets();
}

void KNMusicMainPlayerContent::setColumnWidget(const int &index,
                                               QWidget *widget)
{
    //Check out the parameter validation.
    if(widget==nullptr || index<0 || index>3)
    {
        //Ignore the invalid parameter.
        return;
    }
    //Change the relationship of the widget.
    widget->setParent(this);
    //Save the pointer.
    m_columnWidgets[index]=widget;
}

void KNMusicMainPlayerContent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget first.
    QWidget::resizeEvent(event);
    //Set the default mode to column 1.
    int columnCount=1;
    //Check the size of the main player.
    //If the width is 1.5 times greater than the height, then column two.
    if((height()+(height()>>2)) <= width())
    {
        columnCount=2;
    }
    //If the width is two times greater than the height, then column three.
    if(height()+(height()>>1) <= width())
    {
        columnCount=3;
    }
    //Set the column count and regeometry the widgets.
    setColumnCount(columnCount);
}

void KNMusicMainPlayerContent::onActionSingleToPrevious()
{
    //Check the following things.
    // 1. The column count is not 1.
    // 2. Current index is 0.
    // 3. The animation is running.
    // 4. The widgets are null.
    //Any one of above is true, ignore this moving preivous.
    if(m_columnCount!=1 || m_currentIndex==0 ||
            m_singleAnime->state()==QAbstractAnimation::Running ||
            m_columnWidgets[m_currentIndex]==nullptr ||
            m_columnWidgets[m_currentIndex-1]==nullptr)
    {
        //Ignore the to moving previous.
        return;
    }
    //Set the flag.
    m_movingPrevious=true;
    //Show the widgets.
    m_columnWidgets[m_currentIndex-1]->show();
    m_columnWidgets[m_currentIndex]->show();
    //Set the animation target.
    m_singleIn->setTargetObject(m_columnWidgets[m_currentIndex-1]);
    m_singleOut->setTargetObject(m_columnWidgets[m_currentIndex]);
    //Update single in parameter.
    updateSinglePrevParameter();
    //Start the animation group.
    m_singleAnime->start();
}

void KNMusicMainPlayerContent::onActionSingleToNext()
{
    //Check the following things.
    // 1. The column count is not 1.
    // 2. Current index is 2.
    // 3. The animation is running.
    // 4. The widgets are null.
    //Any one of above is true, ignore this moving preivous.
    if(m_columnCount!=1 || m_currentIndex==2 ||
            m_singleAnime->state()==QAbstractAnimation::Running ||
            m_columnWidgets[m_currentIndex]==nullptr ||
            m_columnWidgets[m_currentIndex-1]==nullptr)
    {
        //Ignore the to previous check.
        return;
    }
    //Set the flag.
    m_movingPrevious=false;
    //Show the widgets.
    m_columnWidgets[m_currentIndex+1]->show();
    m_columnWidgets[m_currentIndex]->show();
    //Set the animation target.
    m_singleIn->setTargetObject(m_columnWidgets[m_currentIndex+1]);
    m_singleOut->setTargetObject(m_columnWidgets[m_currentIndex]);
    //Update single in parameter.
    updateSingleNextParameter();
    //Start the animation group.
    m_singleAnime->start();
}

void KNMusicMainPlayerContent::onActionSingleFinished()
{
    //Check out the moving flag, update the index.
    m_currentIndex=m_movingPrevious?m_currentIndex-1:m_currentIndex+1;
    //Update the content switcher.
    m_singleSwitcher->setCurrentIndex(m_currentIndex);
}

inline void KNMusicMainPlayerContent::setWidgetVisible(int index, bool visible)
{
    //Check out the pointer first.
    if(m_columnWidgets[index]!=nullptr)
    {
        //Set the visible to the widget.
        m_columnWidgets[index]->setVisible(visible);
    }
}

inline void KNMusicMainPlayerContent::regeometryWidgets()
{
    switch(m_columnCount)
    {
    case 1:
        //Check whether the animation is running.
        if(m_singleAnime->state()==QAbstractAnimation::Running)
        {
            //Check out the in/out flag.
            if(m_movingPrevious)
            {
                //Update the animation to previous parameter.
                updateSinglePrevParameter();
                //Mission complete.
                return;
            }
            //Update the animation to next parameter.
            updateSingleNextParameter();
            //Mission complete.
            return;
        }
        //Check whether the current index is valid.
        if(m_currentIndex>-1 && m_currentIndex<3)
        {
            //Check out the pointer is null or not.
            if(m_columnWidgets[m_currentIndex]!=nullptr)
            {
                //Set the column switcher widget.
                m_singleSwitcher->setGeometry(0, height()-16, width(), 16);
                //Update the widget geometry.
                m_columnWidgets[m_currentIndex]->setGeometry(0, 0, width(),
                                                             height()-16);
            }
        }
        break;
    case 2:
    {
        //Follow up the golden ratio.
        int detailInfoWidth=(qreal)width()*0.382;
        //Check out the first widget pointer.
        if(m_columnWidgets[0]!=nullptr)
        {
            //Resize the detail info panel widget.
            m_columnWidgets[0]->setGeometry(0,
                                            0,
                                            detailInfoWidth,
                                            height());
        }
        //Resize the second widget.
        if(m_columnWidgets[1]!=nullptr)
        {
            //Update the second widget geometry.
            m_columnWidgets[1]->setGeometry(detailInfoWidth,
                                            0,
                                            width()-detailInfoWidth,
                                            height());
        }
        break;
    }
    case 3:
    {
        //Calculate detail info width.
        int detailInfoWidth=(qreal)width()*0.276;
        //Check out the widget pointer.
        if(m_columnWidgets[0]!=nullptr)
        {
            //Resize the detail info panel widget.
            m_columnWidgets[0]->setGeometry(0,
                                            0,
                                            detailInfoWidth,
                                            height());
        }
        //Initial the playlist widget width.
        int playlistWidth=detailInfoWidth;
        //Check out the switcher size.
        if(m_columnWidgets[2]!=nullptr)
        {
            //Calculate the switcher width.
            playlistWidth=qMax((detailInfoWidth>>2)+(detailInfoWidth>>1), 178);
            //Resize the widget switcher.
            m_columnWidgets[2]->setGeometry(width()-playlistWidth,
                                            0,
                                            playlistWidth,
                                            height());
        }
        //Calculate the lyrics widget width, and resize the lyrics widget.
        //Check out the column widget.
        if(m_columnWidgets[1]!=nullptr)
        {
            //Resize the lyrics widget.
            m_columnWidgets[1]->setGeometry(
                        detailInfoWidth,
                        0,
                        width()-playlistWidth-detailInfoWidth,
                        height());
        }
        break;
    }
    default:
        break;
    }
}

inline void KNMusicMainPlayerContent::updateSinglePrevParameter()
{
    //Calculate the current rect.
    QRect currentRect=QRect(0, 0, width(), height()-16);
    //Configure the animation start and end value for previous configure.
    m_singleIn->setStartValue(QRect(-width(), 0, width(), height()-16));
    m_singleIn->setEndValue(currentRect);
    m_singleOut->setStartValue(currentRect);
    m_singleOut->setEndValue(QRect(width(), 0, width(), height()-16));
}

inline void KNMusicMainPlayerContent::updateSingleNextParameter()
{
    //Calculate the current rect.
    QRect currentRect=QRect(0, 0, width(), height()-16);
    //Configure the animation start and end value for next configure.
    m_singleIn->setStartValue(QRect(width(), 0, width(), height()-16));
    m_singleIn->setEndValue(currentRect);
    m_singleOut->setStartValue(currentRect);
    m_singleOut->setEndValue(QRect(-width(), 0, width(), height()-16));
}

inline QPropertyAnimation *KNMusicMainPlayerContent::generateAnime()
{
    //Generate the anime.
    QPropertyAnimation *anime=new QPropertyAnimation(this);
    //Configure the animation.
    anime->setPropertyName("geometry");
    anime->setEasingCurve(QEasingCurve::OutCubic);
    //Give back the animation object.
    return anime;
}
