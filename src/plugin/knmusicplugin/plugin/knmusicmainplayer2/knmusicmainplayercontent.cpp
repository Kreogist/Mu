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
#include "knmusiclyricsmanager.h"
#include "knmusicscrolllyrics.h"
#include "knmusicnowplayinglistview.h"

#include "knmusicmainplayercontent.h"

#include <QDebug>

KNMusicMainPlayerContent::KNMusicMainPlayerContent(QWidget *parent) :
    QWidget(parent),
    m_albumArt(new KNHighLightLabel(this)),
    m_titleLabel(new KNLoopScrollLabel(this)),
    m_artistLabel(new KNLoopScrollLabel(this)),
    m_albumLabel(new KNLoopScrollLabel(this)),
    m_lyricsPanel(new KNMusicScrollLyrics(this)),
    m_playlistPanel(new KNMusicNowPlayingListView(this)),
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
        //Add to layout.
        m_gotoLayout->addWidget(m_gotoIcons[i]);
    }
    m_gotoLayout->addStretch();
    //Configure the lyrics.
    m_lyricsPanel->setObjectName("MainPlayerLyrics");
    m_lyricsPanel->setBackend(knMusicGlobal->lyricsManager()->backend());
    knTheme->registerWidget(m_lyricsPanel);
    //Configure the playlist panel.
    m_playlistPanel->setSizePolicy(QSizePolicy::Minimum,
                                   QSizePolicy::Minimum);
    m_playlistPanel->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_playlistPanel->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //Configure the panel.
    connect(m_animationGroup, &QParallelAnimationGroup::finished,
            [=]
            {
                //Take all the animation, but not delete them.
                while(m_animationGroup->animationCount())
                {
                    //Take the animation.
                    m_animationGroup->takeAnimation(0);
                }
                //Emit the current panel state.
                emit stateChanged(m_state);
            });

    //Configure the information container.
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                               m_infoContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
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
    //Check horizontal state.
    if(m_isHorizontal)
    {
        //When the first item of item.
        if(m_gotoLayout->itemAt(0)->widget()==nullptr)
        {
            //It should be a stretch item, remove it from the layout.
            delete m_gotoLayout->takeAt(0);
        }
    }
    else
    {
        //When the first item of item.
        if(m_gotoLayout->itemAt(0)->widget())
        {
            //Missing a stretch item, add it to the layout.
            m_gotoLayout->insertStretch(0);
        }
    }
    //Update the widget sizes.
    applySize();
    //Update the widget position.
    applyPosition();
}

void KNMusicMainPlayerContent::setPanelState(int panelState)
{
    //Check the panel state.
    bool fromLeft=panelState > m_state;
    //Update the panel state.
    m_state=panelState;
    //Update the widget position.
    applyPosition(false, fromLeft);
    //Execute the animation.
    m_animationGroup->start();
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

void KNMusicMainPlayerContent::setPlayingModel(KNMusicProxyModel *proxyModel)
{
    //Set the model to the list view.
    m_playlistPanel->setModel(proxyModel);
}

void KNMusicMainPlayerContent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Update the widget sizes.
    applySize();
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
    //Construct the animation.
    WidgetAnimeState animeState;
    //Create the property animation for the widget.
    animeState.animation=new QPropertyAnimation(widget, "pos", this);
    animeState.animation->setEasingCurve(QEasingCurve::OutCubic);
    //Update the visibility to shown.
    animeState.isShown=true;
    //Insert to the animation map.
    m_animeMap.insert(widget, animeState);
}

inline void KNMusicMainPlayerContent::showWidget(QWidget *widget, QPoint pos,
                                                 bool force, bool fromLeft)
{
    //Get the anime state.
    WidgetAnimeState animeState=m_animeMap.value(widget);
    //Check the force switch.
    if(force && m_animationGroup->state()!=QAbstractAnimation::Running)
    {
        //Apply the size and position.
        widget->move(pos);
        //Update animation state.
        animeState.isShown=true;
        m_animeMap.insert(widget, animeState);
        return;
    }
    //Else set the animation.
    bool shownButNotCorrect=widget->pos()!=pos;
    if(!animeState.isShown || shownButNotCorrect)
    {
        //Else set the animation.
        QPropertyAnimation *animation=animeState.animation;
        animation->setEndValue(pos);
        QPoint startPos(fromLeft?width():(-widget->width()), pos.y());
        animation->setStartValue(shownButNotCorrect?widget->pos():startPos);
        widget->move(pos);
        //Update the shown state.
        animeState.isShown=true;
        m_animeMap.insert(widget, animeState);
        //Add the animation to parallel group.
        m_animationGroup->addAnimation(animation);
    }
}

void KNMusicMainPlayerContent::hideWidget(QWidget *widget,
                                          bool force, bool fromLeft)
{
    //Get the anime state.
    WidgetAnimeState animeState=m_animeMap.value(widget);
    //Check the force switch
    if(force && m_animationGroup->state()!=QAbstractAnimation::Running)
    {
        //Apply the position to the widget.
        widget->move(QPoint((fromLeft?(-widget->width()):width()),
                            widget->y()));
        //Update animation state.
        animeState.isShown=false;
        m_animeMap.insert(widget, animeState);
        return;
    }
    //Else set the animation.
    if(animeState.isShown)
    {
        QPropertyAnimation *animation=animeState.animation;
        animation->setStartValue(widget->pos());
        animation->setEndValue(QPoint((fromLeft?(-widget->width()):width()),
                                      widget->y()));
        //Update the state.
        animeState.isShown=false;
        m_animeMap.insert(widget, animeState);
        //Add the animation to parallel group.
        m_animationGroup->addAnimation(animation);
    }
}

inline void KNMusicMainPlayerContent::applyPosition(bool force, bool fromLeft)
{
    //Check the horizontal.
    if(m_isHorizontal)
    {
        if(m_state==StateSongInfo)
        {
            //Calculate the size and position.
            int albumArtAreaWidth=(qreal)qMin(width()*0.382, height()*0.9),
                    albumArtSize=(qreal)width()*0.23,
                    albumArtX=albumArtAreaWidth-albumArtSize,
                    albumArtTop=(qreal)(height()-albumArtSize)*0.618,
                    contentSpacing=(qreal)albumArtX*0.148;
            albumArtX-=contentSpacing;
            //Update album art widget.
            m_albumArt->resize(albumArtSize, albumArtSize);
            showWidget(m_albumArt, QPoint(albumArtX, albumArtTop),
                       force, fromLeft);
            //Update the font size.
            int fontSize=qMax(height()/30, knDpi->height(15));
            //Update the icon size.
            int iconSize=fontSize+5;
            for(int i=0; i<GotoButtonCount; ++i)
            {
                //Resize the icon size.
                m_gotoIcons[i]->setFixedSize(iconSize, iconSize);
            }
            m_gotoLayout->setSpacing(iconSize/3);
            m_gotoLayout->setContentsMargins(0, iconSize>>1, 0, 0);
            //Resize the font.
            QFont textFont=font();
            //Update the sub label font.
            textFont.setPixelSize(fontSize);
            m_artistLabel->setFont(textFont);
            m_albumLabel->setFont(textFont);
            //Update the title font.
            textFont.setPixelSize(fontSize+(fontSize>>2)*3);
            textFont.setBold(true);
            m_titleLabel->setFont(textFont);
            //Configure the info container.
            m_infoContainer->setContentsMargins(0, knDpi->height(38), 0, 0);
            //Update info container layout.
            m_infoContainer->resize(width()-albumArtAreaWidth
                                    -(contentSpacing<<1)-albumArtX,
                                    m_infoContainer->sizeHint().height());
            showWidget(m_infoContainer,
                       QPoint(albumArtX+albumArtSize+contentSpacing,
                              albumArtTop+albumArtSize-
                              m_infoContainer->height()),
                       force, fromLeft);
            //Hide the lyrics and playlist.
            hideWidget(m_lyricsPanel, force, fromLeft);
            hideWidget(m_playlistPanel, force, fromLeft);
            return;
        }
        //For state lyrics and playlist, they shares the same left layout.
        int albumArtSize=qMin((qreal)width()*0.23, (qreal)knDpi->height(220));
        //Resize the album art.
        m_albumArt->resize(albumArtSize, albumArtSize);
        int albumArtX=knDpi->width(120),
                albumArtY=(qreal)knDpi->height(93);
        showWidget(m_albumArt, QPoint(albumArtX, albumArtY),
                   force, fromLeft);
        //Resize the font.
        QFont textFont=font();
        //Update the sub label font.
        int fontHeight=knDpi->height(18);
        textFont.setPixelSize(fontHeight);
        m_artistLabel->setFont(textFont);
        m_albumLabel->setFont(textFont);
        //Update the icon size.
        int iconSize=textFont.pixelSize()+5;
        for(int i=0; i<GotoButtonCount; ++i)
        {
            //Resize the icon size.
            m_gotoIcons[i]->setFixedSize(iconSize, iconSize);
        }
        m_gotoLayout->setSpacing(iconSize/3);
        m_gotoLayout->setContentsMargins(0, 0, 0, 0);
        //Configure the info container.
        m_infoContainer->setContentsMargins(0, 0, 0, 0);
        int rightPartX=albumArtX+albumArtSize+knDpi->width(25),
            rightPartWidth=width()-rightPartX-albumArtX;
        //Check for the state.
        if(m_state==StateLyrics)
        {
            int titleHeight=fontHeight+(fontHeight>>1);
            textFont.setPixelSize(titleHeight);
            m_titleLabel->setFont(textFont);
            //Update the panel fonts.
            textFont.setPixelSize(titleHeight-(titleHeight>>2));
            m_lyricsPanel->setFont(textFont);
            //Update info container size.
            m_infoContainer->resize(rightPartWidth,
                                    m_infoContainer->sizeHint().height());
            showWidget(m_infoContainer, QPoint(rightPartX, albumArtY),
                       force, fromLeft);
            //Update the lyrics panel and playlist.
            int rightPartY=albumArtY+m_infoContainer->height()+
                            knDpi->height(10);
            m_lyricsPanel->resize(rightPartWidth,
                                  height()-rightPartY-albumArtY);
            //Lyrics.
            showWidget(m_lyricsPanel, QPoint(rightPartX-knDpi->width(15),
                                             rightPartY),
                       force, fromLeft);
            hideWidget(m_playlistPanel, force, fromLeft);
        }
        else
        {
            //Update title label size.
            m_titleLabel->setFont(textFont);
            //Update the info container size.
            m_infoContainer->resize(albumArtSize,
                                    m_infoContainer->sizeHint().height());
            showWidget(m_infoContainer,
                       QPoint(albumArtX,
                              albumArtY+albumArtSize+knDpi->height(5)),
                       force, fromLeft);
            //Update playlist panel font size.
            m_playlistPanel->setFont(textFont);
            //Update playlist panel.
            m_playlistPanel->resize(rightPartWidth, height()-(albumArtY<<1));
            //Playlist.
            showWidget(m_playlistPanel, QPoint(rightPartX,
                                               albumArtY),
                       force, fromLeft);
            hideWidget(m_lyricsPanel, force, fromLeft);
        }
        return;
    }
    //Vertial layout.
    switch(m_state)
    {
    case StateSongInfo:
    {
        //Calculate and apply the size to widget.
        int detailPanelHeight=m_infoContainer->sizeHint().height(),
            albumArtSize=(qreal)qMin(height()-detailPanelHeight-
                                     knDpi->height(128),
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
        showWidget(m_infoContainer,
                   QPoint(0, albumArtTop+albumArtSize),
                   force, fromLeft);
        //Hide the lyrics and playlist.
        hideWidget(m_lyricsPanel, force, fromLeft);
        hideWidget(m_playlistPanel, force, fromLeft);
        break;
    }
    case StateLyrics:
    {
        //Show the lyrics panel only.
        showWidget(m_lyricsPanel, QPoint(0, 0),
                   force, fromLeft);
        //Hide all the other widgets.
        hideWidget(m_albumArt, force, fromLeft);
        hideWidget(m_infoContainer, force, fromLeft);
        hideWidget(m_playlistPanel, force, fromLeft);
        break;
    }
    case StatePlayList:
    {
        //Show the playlist panel only.
        showWidget(m_playlistPanel, QPoint(0, 64),
                   force, fromLeft);
        //Hide all the other widgets.
        hideWidget(m_albumArt, force, fromLeft);
        hideWidget(m_infoContainer, force, fromLeft);
        hideWidget(m_lyricsPanel, force, fromLeft);
        break;
    }
    }
}

inline void KNMusicMainPlayerContent::applySize()
{
    int fontSize;
    //Check the horizontal or vertical layout.
    if(m_isHorizontal)
    {
        //Update sizes for horizontal layout.
        //Configure the label.
        m_titleLabel->setAlignment(Qt::AlignLeft);
        m_artistLabel->setAlignment(Qt::AlignLeft);
        m_albumLabel->setAlignment(Qt::AlignLeft);
        //Configure the lyrics panel.
        m_lyricsPanel->setAlignment(Qt::AlignLeft);
        //Calculate the font size.
        fontSize=qMax(height()/30, knDpi->height(15));
    }
    else
    {
        //Update sizes for vertical layout.
        //Configure the info container.
        m_infoContainer->setContentsMargins(0, knDpi->height(38), 0, 0);
        //Configure the label.
        m_titleLabel->setAlignment(Qt::AlignHCenter);
        m_artistLabel->setAlignment(Qt::AlignHCenter);
        m_albumLabel->setAlignment(Qt::AlignHCenter);
        //Configure the lyrics panel.
        m_lyricsPanel->setAlignment(Qt::AlignHCenter);
        //Calculate the font size.
        fontSize=qMax(width()/30, knDpi->height(15));
        //Update the icon size.
        int iconSize=fontSize+5;
        for(int i=0; i<GotoButtonCount; ++i)
        {
            //Resize the icon size.
            m_gotoIcons[i]->setFixedSize(iconSize, iconSize);
        }
        m_gotoLayout->setSpacing(iconSize/3);
        m_gotoLayout->setContentsMargins(0, iconSize>>1, 0, 0);
        //Update lyrics and playlist size.
        m_lyricsPanel->resize(size());
        m_playlistPanel->resize(width(), height()-knDpi->height(157));
    }
    //Resize the font.
    QFont textFont=font();
    textFont.setPixelSize(fontSize);
    //Update the title font.
    m_titleLabel->setFont(textFont);
    //Update the panel fonts.
    m_lyricsPanel->setFont(textFont);
    //Update the sub label font.
    fontSize=(fontSize>>2)*3;
    textFont.setPixelSize(fontSize);
    m_playlistPanel->setFont(textFont);
    m_artistLabel->setFont(textFont);
    m_albumLabel->setFont(textFont);
}
