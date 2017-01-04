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

#define ListItemHeight          37
#define ListTitleSpacing        7
#define ListVerticalSpacing     6
#define ListTextLeft            20
#define ListTextSpacing         2

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
    //Check the model.
    if(listModel()==nullptr)
    {
        //Ignore the rendering hints.
        return;
    }
    //Initial the painter.
    QPainter painter(viewport());
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Get the model.
    KNMusicStoreHomeListModel *itemModel=listModel();
    //Calcualte the render boundary.
    int maxRenderCount=qMin(m_maxRenderingCount, itemModel->rowCount()),
            currentY=0, hoverRow=hoverIndex().row();
    //Get the fonts.
    QFont titleFont=font();
    titleFont.setBold(true);
    QFont titleUnderLine=titleFont;
    titleUnderLine.setUnderline(true);
    QFont normalUnderLineFont=font();
    normalUnderLineFont.setUnderline(true);
    //Render each item in the list.
    for(int i=0; i<maxRenderCount; ++i)
    {
        //Reset the font.
        painter.setFont(i==hoverRow?normalUnderLineFont:font());
        //Draw item index.
        painter.drawText(0, currentY+ListTitleSpacing, width(),
                         fontMetrics().height(),
                         Qt::AlignTop | Qt::AlignLeft,
                         QString::number(i+1)+".");
        //Get the item data.
        const KNMusicStoreHomeItem &item=itemModel->homeItem(i);
        //Calculate the text top.
        int textTop=currentY+ListVerticalSpacing,
                textWidth=width()-ListTextLeft;
        //Change the painter opacity.
        painter.setOpacity(0.5);
        //Draw the subtitle text.
        painter.drawText(ListTextLeft,
                         textTop+ListTextSpacing+fontMetrics().height(),
                         textWidth, fontMetrics().height(),
                         Qt::AlignTop | Qt::AlignLeft,
                         fontMetrics().elidedText(item.subheading,
                                                  Qt::ElideRight,
                                                  textWidth));
        //Reset the opacity.
        painter.setOpacity(1.0);
        //Set the title font.
        painter.setFont(i==hoverRow?titleUnderLine:titleFont);
        //Draw the title.
        painter.drawText(ListTextLeft, textTop,
                         textWidth, fontMetrics().height(),
                         Qt::AlignTop | Qt::AlignLeft,
                         fontMetrics().elidedText(item.title,
                                                  Qt::ElideRight,
                                                  textWidth));
        //Increase current Y.
        currentY+=ListItemHeight;
    }
}
