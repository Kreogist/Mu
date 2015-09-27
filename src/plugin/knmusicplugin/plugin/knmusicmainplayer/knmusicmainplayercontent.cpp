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
#include "knhwidgetswitcher.h"

#include "knmusicmainplayercontent.h"

#include <QDebug>

KNMusicMainPlayerContent::KNMusicMainPlayerContent(QWidget *parent) :
    QWidget(parent),
    m_widgetSwicher(new KNHWidgetSwitcher(this)),
    m_columnCount(1)
{
    //Clear up the widget pointer array.
    m_columnWidgets[0]=nullptr;
    m_columnWidgets[1]=nullptr;
    m_columnWidgets[2]=nullptr;
}

void KNMusicMainPlayerContent::setColumnCount(const int &columnCount)
{
    //Checkout the column count.
    if(columnCount>3)
    {
        //Greater than 3 columns, take it as 3 column.
        m_columnCount=3;
        return;
    }
    //If the column is less than 1, there will be at least 1 column.
    if(columnCount<1)
    {
        //Save the column count.
        m_columnCount=1;
        return;
    }
    //Save the column count.
    m_columnCount=columnCount;
    //Update the widget configure.
    m_widgetSwicher->clear();
    //Show up all the widgets.
    for(int i=0; i<3; ++i)
    {
        //Check the pointer first.
        if(m_columnWidgets[i]!=nullptr)
        {
            //Change the relationship.
            m_columnWidgets[i]->setParent(this, Qt::Widget);
            //Show the widget.
            m_columnWidgets[i]->setVisible(true);
        }
    }
    //Check out the column count.
    switch(m_columnCount)
    {
    case 1:
        //Add all widgets in order to the widget switcher.
        for(int i=0; i<3; ++i)
        {
            //Check the widget first.
            if(m_columnWidgets[i]!=nullptr)
            {
                //Add the widget to switcher.
                m_widgetSwicher->addWidget(m_columnWidgets[i]);
            }
        }
        break;
    case 2:
        //Add the No.1 and No.2 widgets to switcher.
        for(int i=1; i<3; ++i)
        {
            //Check the widget first.
            if(m_columnWidgets[i]!=nullptr)
            {
                //Add the widget to switcher.
                m_widgetSwicher->addWidget(m_columnWidgets[i]);
            }
        }
        break;
    case 3:
        //Add playlist widget to the switcher.
        //Check the widget first.
        if(m_columnWidgets[2]!=nullptr)
        {
            //Add the widget to switcher.
            m_widgetSwicher->addWidget(m_columnWidgets[2]);
        }
        break;
    }
    //Update the geometry of the widgets.
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
    if(columnCount==m_columnCount)
    {
        //Regeometry the widgets.
        regeometryWidgets();
    }
    else
    {
        //Set the column count and regeometry the widgets.
        setColumnCount(columnCount);
    }
}

inline void KNMusicMainPlayerContent::regeometryWidgets()
{
    //Accroding to the column count, we will do different things.
    switch(m_columnCount)
    {
    case 1:
        //When there's only one column, resize the switcher.
        m_widgetSwicher->setGeometry(rect());
        break;
    case 2:
    {
        //For two widgets, place the detail info panel at the left part.
        //Follow up the golden ratio.
        int detailInfoWidth=(qreal)width()*0.382;
        //Check out the widget pointer.
        if(m_columnWidgets[0]!=nullptr)
        {
            //Ensure the visible.
            m_columnWidgets[0]->setVisible(true);
            //Resize the detail info panel widget.
            m_columnWidgets[0]->setGeometry(0,
                                            0,
                                            detailInfoWidth,
                                            height());
        }
        //Resize the switcher panel.
        m_widgetSwicher->setGeometry(detailInfoWidth,
                                     0,
                                     width()-detailInfoWidth,
                                     height());
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
            playlistWidth=qMin(detailInfoWidth,
                               m_columnWidgets[2]->maximumWidth());
            //Resize the widget switcher.
            m_widgetSwicher->setGeometry(width()-playlistWidth,
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
    }
}

