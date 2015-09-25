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
#include <QBoxLayout>
#include <QFormLayout>
#include <QLabel>

#include "knloopscrolllabel.h"
#include "knhighlightlabel.h"

#include "knmusicglobal.h"

#include "knmusicmainplayerpanel.h"

#include <QDebug>

KNMusicMainPlayerPanel::KNMusicMainPlayerPanel(QWidget *parent) :
    QWidget(parent),
    m_detailPanel(new QWidget(this)),
    m_albumArt(new KNHighLightLabel(this))
{
    //Initial the panel layout.
    QBoxLayout *detailLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            m_detailPanel);
    m_detailPanel->setLayout(detailLayout);
    //Set the icon paths.
    QString detailIconPath[DetailLabelCount];
    detailIconPath[SongLabel]=":/plugin/music/category/song.png";
    detailIconPath[ArtistLabel]=":/plugin/music/category/artist.png";
    detailIconPath[AlbumLabel]=":/plugin/music/category/ablum.png";
    detailIconPath[GenreLabel]=":/plugin/music/category/genre.png";
    //Initial labels.
    for(int i=0; i<DetailLabelCount; ++i)
    {
        //Generate row layout.
        QBoxLayout *rowLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             detailLayout->widget());
        //Initial detail info icon and text labels.
        m_detailIcons[i]=new QLabel(m_detailPanel);
        m_detailText[i]=new KNLoopScrollLabel(m_detailPanel);
        //Configure the detail icon label.
        m_detailIcons[i]->setScaledContents(true);
        //Set icon data to the icon label.
        m_detailIcons[i]->setPixmap(QPixmap(detailIconPath[i]));
        //Add row to detail panel.
        rowLayout->addWidget(m_detailIcons[i]);
        rowLayout->addWidget(m_detailText[i], 1);
        detailLayout->addLayout(rowLayout);
    }
}

void KNMusicMainPlayerPanel::setAnalysisItem(const KNMusicAnalysisItem &item)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=item.detailInfo;
    //Set the analysis data to the labels.
    m_detailText[SongLabel]->setText(detailInfo.textLists[Name].toString());
    m_detailText[ArtistLabel]->setText(detailInfo.textLists[Artist].toString());
    m_detailText[AlbumLabel]->setText(detailInfo.textLists[Album].toString());
    m_detailText[GenreLabel]->setText(detailInfo.textLists[Genre].toString());
    //Set the album art.
    //Check the validation of the cover image.
    if(item.coverImage.isNull())
    {
        m_albumArt->setPixmap(knMusicGlobal->noAlbumArt());
    }
    else
    {
        m_albumArt->setPixmap(QPixmap::fromImage(item.coverImage));
    }
}

void KNMusicMainPlayerPanel::resizeEvent(QResizeEvent *event)
{
    //Resize the widge first.
    QWidget::resizeEvent(event);
    //Calculate the font size.
    int sizeParameter=qMin(width(), height()),
        fontSize=sizeParameter/17;
    //Generate the font.
    QFont matchedFont=font();
    matchedFont.setPixelSize(fontSize);
    //Set size to the icon and text labels.
    for(int i=0; i<DetailLabelCount; ++i)
    {
        //Resize the icon label.
        m_detailIcons[i]->setFixedSize(fontSize, fontSize);
        //Update the text label font size.
        m_detailText[i]->setFont(matchedFont);
    }

    //Check the width and height, and place the position of the detail panel.
    //When width is 1.5 larger than height, we will treat it as a horizontal
    //rectangle.
    if(height()+(height()>>1) < width())
    {
        //Horizontal mode.
        //Calculate the album art size.
        int halfWidth=width()>>1,
            albumArtSize=qMin(halfWidth, height()),
            detailPanelHeight=m_detailPanel->sizeHint().height();
        //Make the album art a little bit smaller.
        albumArtSize=(qreal)albumArtSize * 0.85;
        m_albumArt->setGeometry((halfWidth-albumArtSize)>>1,
                                (height()-albumArtSize)>>1,
                                albumArtSize,
                                albumArtSize);
        //Get the panel size.
        m_detailPanel->setGeometry(halfWidth,
                                   (height()-detailPanelHeight)>>1,
                                   halfWidth,
                                   detailPanelHeight);
    }
    else
    {
        //Vertical mode.
        //Get the panel size and calculate the top of the album art label.
        int detailPanelHeight=m_detailPanel->sizeHint().height(),
            albumArtSize=(qreal)(qMin(height()-detailPanelHeight, width()))
                            * 0.9,
            albumArtTop=(height()-albumArtSize-detailPanelHeight)>>1;
        //Resize the album art.
        m_albumArt->setGeometry((width()-albumArtSize)>>1,
                                albumArtTop,
                                albumArtSize,
                                albumArtSize);
        //Resize the detail panel, and put it to the right place.
        m_detailPanel->setGeometry(0,
                                   albumArtTop+albumArtSize,
                                   width(),
                                   detailPanelHeight);
    }
}

