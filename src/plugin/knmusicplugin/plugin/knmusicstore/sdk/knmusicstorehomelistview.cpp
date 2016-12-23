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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>

#include "knmusicstorehomelistmodel.h"

#include "knmusicstorehomelistview.h"

#define ListItemHeight  37

KNMusicStoreHomeListView::KNMusicStoreHomeListView(QWidget *parent) :
    KNMusicStoreHomeItemView(parent),
    m_maxRenderingCount(0)
{
}

QModelIndex KNMusicStoreHomeListView::indexAt(const QPoint &point) const
{
    //Check the height is 0 or not.
    if(height()==0 || nullptr==listModel())
    {
        //Ignore the null value.
        return QModelIndex();
    }
    //Calculate the point index.
    return listModel()->index(point.y()/ListItemHeight);
}

QRect KNMusicStoreHomeListView::visualRect(const QModelIndex &index) const
{
    //Check the index row, and it is pretty easy to calculate.
    return QRect(0, index.row()*ListItemHeight, width(), ListItemHeight);
}

void KNMusicStoreHomeListView::tweakHeight(int maxRenderingCount)
{
    //Save the item maximum number.
    m_maxRenderingCount=maxRenderingCount;
    //Resize the list to the size.
    setFixedHeight(m_maxRenderingCount*ListItemHeight);
}

void KNMusicStoreHomeListView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Render each item in the list.
    for(int i=0; i<m_maxRenderingCount; ++i)
    {
        //Start rendering items.
    }
}
