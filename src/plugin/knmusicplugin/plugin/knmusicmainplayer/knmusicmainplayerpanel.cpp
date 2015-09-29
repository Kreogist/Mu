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
#include "knopacityanimebutton.h"
#include "knthememanager.h"

#include "knmusicglobal.h"

#include "knmusicmainplayerpanel.h"

#include <QDebug>

KNMusicMainPlayerPanel::KNMusicMainPlayerPanel(QWidget *parent) :
    QWidget(parent),
    m_titleLabel(new KNLoopScrollLabel(this)),
    m_artistAlbumLabel(new KNLoopScrollLabel(this)),
    m_detailPanel(new QWidget(this)),
    m_albumArt(new KNHighLightLabel(this))
{
    setObjectName("MainPlayerInfoPanel");
    //Configure the label.
    m_titleLabel->setAlignment(Qt::AlignHCenter);
    m_artistAlbumLabel->setAlignment(Qt::AlignHCenter);
    //Configure the panel widget.
    m_detailPanel->setContentsMargins(5,10,5,0);
    //Initial the panel layout.
    QBoxLayout *detailLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            m_detailPanel);
    detailLayout->setContentsMargins(0,0,0,0);
    detailLayout->setSpacing(0);
    m_detailPanel->setLayout(detailLayout);
    //Set the icon paths.
    QString gotoIconPath[GotoButtonCount];
    gotoIconPath[GotoSong]=":/plugin/music/category/song.png";
    gotoIconPath[GotoArtist]=":/plugin/music/category/artist.png";
    gotoIconPath[GotoAlbum]=":/plugin/music/category/ablum.png";
    gotoIconPath[GotoGenre]=":/plugin/music/category/genre.png";
    //Add the title and artist-album labels to layout.
    detailLayout->addWidget(m_titleLabel);
    detailLayout->addWidget(m_artistAlbumLabel);
    //Initial the goto button layout.
    m_buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                  detailLayout->widget());
    m_buttonLayout->setContentsMargins(0,0,0,0);
    m_buttonLayout->addStretch();
    //Initial goto buttons.
    for(int i=0; i<GotoButtonCount; ++i)
    {
        //Initial detail info icon and text labels.
        m_gotoIcons[i]=new KNOpacityAnimeButton(m_detailPanel);
        //Configure the detail icon label.
        m_gotoIcons[i]->setFixedSize(16, 16);
        //Set icon data to the icon label.
        m_gotoIcons[i]->setIcon(QPixmap(gotoIconPath[i]));
        //Add row to detail panel.
        m_buttonLayout->addWidget(m_gotoIcons[i]);
    }
    m_buttonLayout->addStretch();
    //Link the buttons.
    connect(m_gotoIcons[GotoSong], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerPanel::requireShowInSongs);
    connect(m_gotoIcons[GotoArtist], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerPanel::requireShowInArtists);
    connect(m_gotoIcons[GotoAlbum], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerPanel::requireShowInAlbums);
    connect(m_gotoIcons[GotoGenre], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerPanel::requireShowInGenres);
    //Add the button layout to detailed layout.
    detailLayout->addLayout(m_buttonLayout);

    //Link to theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicMainPlayerPanel::onActionThemeChanged);
    onActionThemeChanged();
}

void KNMusicMainPlayerPanel::setAnalysisItem(const KNMusicAnalysisItem &item)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=item.detailInfo;
    //Set the analysis data to the labels.
    m_titleLabel->setText(detailInfo.textLists[Name].toString());
    setAristAndAlbum(detailInfo.textLists[Artist].toString(),
                     detailInfo.textLists[Album].toString());
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

void KNMusicMainPlayerPanel::updatePanelFont(const QFont &labelFont)
{
    //Set the font.
    setFont(labelFont);
    //Update the icon size.
    int iconSize=labelFont.pixelSize()+5;
    for(int i=0; i<GotoButtonCount; ++i)
    {
        //Resize the icon size.
        m_gotoIcons[i]->setFixedSize(iconSize, iconSize);
    }
    //Resize the artist-album label font size.
    QFont artistAlbumFont=labelFont;
    artistAlbumFont.setPixelSize((labelFont.pixelSize()>>2)*3);
    m_artistAlbumLabel->setFont(artistAlbumFont);
    //Resize the spacing.
    m_detailPanel->setContentsMargins(5, iconSize, 5, 0);
    m_buttonLayout->setSpacing(iconSize/3);
    m_buttonLayout->setContentsMargins(0, iconSize>>1, 0, 0);
}

void KNMusicMainPlayerPanel::resizeEvent(QResizeEvent *event)
{
    //Resize the widge first.
    QWidget::resizeEvent(event);

    //Check the width and height, and place the position of the detail panel.
    //When width is 1.5 larger than height, we will treat it as a horizontal
    //rectangle.
    if(height()+(height()>>1) < width())
    {
        //Horizontal mode.
        //Calculate the album art size.
        int albumArtWidth=(qreal)width()*0.618,
            albumArtSize=qMin(albumArtWidth, height()),
            detailPanelHeight=m_detailPanel->sizeHint().height();
        //Make the album art a little bit smaller.
        albumArtSize=(qreal)albumArtSize * 0.85;
        m_albumArt->setGeometry(albumArtWidth-albumArtSize,
                                (height()-albumArtSize)>>1,
                                albumArtSize,
                                albumArtSize);
        //Get the panel size.
        m_detailPanel->setGeometry(albumArtWidth,
                                   (height()-detailPanelHeight)>>1,
                                   width()-albumArtWidth,
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

void KNMusicMainPlayerPanel::onActionThemeChanged()
{
    //Get the theme.
    QPalette widgetPalette=knTheme->getPalette(objectName());
    //Set the palette.
    setPalette(widgetPalette);

    //Generate the title label.
    QPalette titlePalette=widgetPalette;
    titlePalette.setColor(QPalette::WindowText,
                          titlePalette.color(QPalette::Highlight));
    m_titleLabel->setPalette(titlePalette);
}

void KNMusicMainPlayerPanel::setAristAndAlbum(const QString &artist,
                                              const QString &album)
{
    //Clear the label first.
    m_artistAlbumLabel->setText("");
    //Set the text.
    if(artist.isEmpty())
    {
        //Set the label to display the 'Album'
        m_artistAlbumLabel->setText(album);
    }
    else
    {
        //Check if the album is empty.
        if(album.isEmpty())
        {
            //Set the lable to display 'Artist'
            m_artistAlbumLabel->setText(artist);
        }
        else
        {
            //Set the lable to display 'Artist - Album'
            m_artistAlbumLabel->setText(artist + " - " + album);
        }
    }
}

