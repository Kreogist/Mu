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
#include <QBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "knhighlightlabel.h"
#include "kndpimanager.h"
#include "knloopscrolllabel.h"
#include "knopacityanimebutton.h"
#include "knthememanager.h"

#include "knmusicglobal.h"
#include "knmusicscrolllyrics.h"

#include "knmusicmainplayercontent.h"

#include <QDebug>

KNMusicMainPlayerContent::KNMusicMainPlayerContent(QWidget *parent) :
    QWidget(parent),
    m_albumArt(new KNHighLightLabel(this)),
    m_titleLabel(new KNLoopScrollLabel(this)),
    m_artistLabel(new KNLoopScrollLabel(this)),
    m_albumLabel(new KNLoopScrollLabel(this)),
    m_lyricsPanel(new KNMusicScrollLyrics(this)),
    m_playlistPanel(new QWidget(this)),
    m_infoContainer(new QWidget(this)),
    m_gotoLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_animationGroup(new QParallelAnimationGroup(this)),
    m_state(StateSongInfo),
    m_isHorizontal(false)
{
    setObjectName("MainPlayerInfoPanel");
    //Set the icon paths.
    QString gotoIconPath[GotoButtonCount];
    gotoIconPath[GotoSong]=":/plugin/music/category/song.png";
    gotoIconPath[GotoArtist]=":/plugin/music/category/artist.png";
    gotoIconPath[GotoAlbum]=":/plugin/music/category/ablum.png";
    gotoIconPath[GotoGenre]=":/plugin/music/category/genre.png";
    //Initial goto buttons.
    m_gotoLayout->addStretch();
    for(int i=0; i<GotoButtonCount; ++i)
    {
        //Initial detail info icon and text labels.
        m_gotoIcons[i]=new KNOpacityAnimeButton(this);
        //Configure the detail icon label.
        m_gotoIcons[i]->setFixedSize(knDpi->size(16, 16));
        //Set icon data to the icon label.
        m_gotoIcons[i]->setIcon(QPixmap(gotoIconPath[i]));
        //Add animation for the goto icon.
        addAnimation(m_gotoIcons[i]);
        //Add to layout.
        m_gotoLayout->addWidget(m_gotoIcons[i]);
    }
    m_gotoLayout->addStretch();
    //Configure the infor container.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                               m_infoContainer);
    containerLayout->setContentsMargins(0, knDpi->height(38), 0, 0);
    containerLayout->setSpacing(0);
    m_infoContainer->setLayout(containerLayout);
    //Add widget.
    containerLayout->addWidget(m_titleLabel);
    containerLayout->addWidget(m_artistLabel);
    containerLayout->addWidget(m_albumLabel);
    containerLayout->addLayout(m_gotoLayout);
    //Add animations for widgets.
    addAnimation(m_albumArt);
    addAnimation(m_infoContainer);
    addAnimation(m_lyricsPanel);
    addAnimation(m_playlistPanel);
    //Link the buttons.
    connect(m_gotoIcons[GotoSong], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContent::requireShowInSongs);
    connect(m_gotoIcons[GotoArtist], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContent::requireShowInArtists);
    connect(m_gotoIcons[GotoAlbum], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContent::requireShowInAlbums);
    connect(m_gotoIcons[GotoGenre], &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContent::requireShowInGenres);

    //Link to theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicMainPlayerContent::onActionThemeChanged);
    onActionThemeChanged();
}

void KNMusicMainPlayerContent::setHorizontal(bool isHorizontal)
{
    //Check the value.
    if(isHorizontal==m_isHorizontal)
    {
        //No need to update the data.
        return;
    }
    //Save the mode.
    m_isHorizontal=isHorizontal;
    //Update the widget position.
    applyPosition();
}

void KNMusicMainPlayerContent::setAnalysisItem(const KNMusicAnalysisItem &item)
{
    //Get the detail info.
    const KNMusicDetailInfo &detailInfo=item.detailInfo;
    //Set the analysis data to the labels.
    m_titleLabel->setText(detailInfo.textLists[Name].toString());
    m_artistLabel->setText(detailInfo.textLists[Artist].toString());
    m_albumLabel->setText(detailInfo.textLists[Album].toString());
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

void KNMusicMainPlayerContent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Update the widget position.
    applyPosition();
}

void KNMusicMainPlayerContent::onActionThemeChanged()
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

inline void KNMusicMainPlayerContent::addAnimation(QWidget *widget)
{
    //Create the property animation for the widget.
    m_animeMap.insert(widget, new QPropertyAnimation(widget, "pos", this));
}

inline void KNMusicMainPlayerContent::showWidget(QWidget *widget, QPoint pos,
                                                 bool force, bool fromLeft)
{
    //Check the force switch.
    if(force)
    {
        //Apply the size and position.
        widget->move(pos);
        return;
    }
    //Else set the animation.
    QPropertyAnimation *animation=m_animeMap.value(widget);
    animation->setEndValue(pos);
    QPoint startPos(fromLeft?(-widget->width()):width(), pos.y());
    animation->setStartValue(startPos);
    widget->move(pos);
    //Add to animation group.
    m_animationGroup->addAnimation(animation);
}

inline void KNMusicMainPlayerContent::applyPosition(bool force, bool fromLeft)
{
    //Check the horizontal.
    if(m_isHorizontal)
    {
        return;
    }
    //Vertial layout.
    switch(m_state)
    {
    case StateSongInfo:
    {
        //Calculate the font size.
        int fontSize=qMax(width()/30, knDpi->height(15));
        //Resize the font.
        QFont textFont=font();
        textFont.setPixelSize(fontSize);
        m_titleLabel->setFont(textFont);
        //Update the icon size.
        int iconSize=fontSize+5;
        for(int i=0; i<GotoButtonCount; ++i)
        {
            //Resize the icon size.
            m_gotoIcons[i]->setFixedSize(iconSize, iconSize);
        }
        m_gotoLayout->setSpacing(iconSize/3);
        m_gotoLayout->setContentsMargins(0, iconSize>>1, 0, 0);
        //Update the sub label font.
        fontSize=(fontSize>>2)*3;
        textFont.setPixelSize(fontSize);
        m_artistLabel->setFont(textFont);
        m_albumLabel->setFont(textFont);
        //Configure the label.
        m_titleLabel->setAlignment(Qt::AlignHCenter);
        m_artistLabel->setAlignment(Qt::AlignHCenter);
        m_albumLabel->setAlignment(Qt::AlignHCenter);
        //Update the goto icon spacing.

        //Calculate and apply the size to widget.
        int detailPanelHeight=m_infoContainer->sizeHint().height(),
                albumArtSize=(qreal)qMin(height()-detailPanelHeight,
                                         width()) * 0.9,
                albumArtTop=(height()-albumArtSize-detailPanelHeight)>>1;
        //Apply the info container size.
        m_infoContainer->resize(width(), detailPanelHeight);
        //Apply the album art position.
        int albumArtX=(width()-albumArtSize)>>1;
        m_albumArt->resize(albumArtSize, albumArtSize);
        showWidget(m_albumArt, QPoint(albumArtX, albumArtTop),
                   force, fromLeft);
        //Apply the label positions.
        showWidget(m_infoContainer, QPoint(0, albumArtTop+albumArtSize),
                   force, fromLeft);
        break;
    }
    }
}
