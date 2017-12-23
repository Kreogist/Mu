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
#include <QPainter>

#include "kndpimanager.h"
#include "knthememanager.h"
#include "knopacityanimebutton.h"

#include "knmusicnowplayingbase.h"
#include "knmusicmainplayercontrol.h"
#include "knmusicmainplayercontent.h"

#include "knmusicmainplayer2.h"

KNMusicMainPlayer2::KNMusicMainPlayer2(QWidget *parent) :
    KNMusicMainPlayerBase(parent),
    m_background(QPixmap()),
    m_originalBackground(QPixmap()),
    m_controlPanel(new KNMusicMainPlayerControl(this)),
    m_contentContainer(new KNMusicMainPlayerContent(this)),
    m_isHorizontal(false)
{
    setObjectName("MainPlayer");
    //Link the hide main player request.
    KNOpacityAnimeButton *hideMainPlayer=m_controlPanel->hideMainPlayer();
    connect(hideMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayer2::requireHide);

    //Set the main player layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add widgets.
    QBoxLayout *topLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    topLayout->setContentsMargins(knDpi->margins(16, 16, 16, 16));
    topLayout->setSpacing(0);
    topLayout->addWidget(hideMainPlayer);
    topLayout->addStretch();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_contentContainer, 1);
    mainLayout->addWidget(m_controlPanel);
    //Configure the detail info panel.
    // Link the require show signals.
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInSongs,
            this, &KNMusicMainPlayer2::requireShowInSongs);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInArtists,
            this, &KNMusicMainPlayer2::requireShowInArtists);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInAlbums,
            this, &KNMusicMainPlayer2::requireShowInAlbums);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInGenres,
            this, &KNMusicMainPlayer2::requireShowInGenres);
    // When it require show, it will automatically hide the songs.
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInSongs,
            this, &KNMusicMainPlayer2::requireHide);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInArtists,
            this, &KNMusicMainPlayer2::requireHide);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInAlbums,
            this, &KNMusicMainPlayer2::requireHide);
    connect(m_contentContainer, &KNMusicMainPlayerContent::requireShowInGenres,
            this, &KNMusicMainPlayer2::requireHide);
    //Register the widget to the theme manager.
    knTheme->registerWidget(this);
}

void KNMusicMainPlayer2::setBackend(KNMusicBackend *backend)
{
    //Give the backend to the control panel.
    m_controlPanel->setBackend(backend);
}

void KNMusicMainPlayer2::setNowPlaying(KNMusicNowPlayingBase *nowPlaying)
{
    //When the playing item changed, main player need to update the data.
    connect(nowPlaying, &KNMusicNowPlayingBase::nowPlayingChanged,
            this, &KNMusicMainPlayer2::onNowPlayingChanged);
    //Set the now playing to control panel.
    m_controlPanel->setNowPlaying(nowPlaying);
}

void KNMusicMainPlayer2::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Check the layout type.
    m_isHorizontal=width() > height();
    //Rescale the pixmap.
    scaleBackground();
    //Set the horizontal setting to content.
    m_contentContainer->setHorizontal(m_isHorizontal);
}

void KNMusicMainPlayer2::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Paint the base.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);
    //Check if background is valid.
    if(!m_originalBackground.isNull())
    {
        //Draw the background.
        painter.drawPixmap((width()-m_background.width())>>1,
                           (height()-m_background.height())>>1,
                           m_background);
    }
}

void KNMusicMainPlayer2::onNowPlayingChanged(
        const KNMusicAnalysisItem &analysisItem)
{
    //Get the image picture, cast as pixmap.
    m_originalBackground=QPixmap::fromImage(analysisItem.coverImage);
    //Scale the background.
    scaleBackground();
    //Set the data to player content.
    m_contentContainer->setAnalysisItem(analysisItem);
}

inline void KNMusicMainPlayer2::scaleBackground()
{
    //Check the original background is valid or not.
    if(m_originalBackground.isNull())
    {
        //Ignore the request.
        return;
    }
    //Check the type of the main player container, scale the image.
    if(m_isHorizontal)
    {
        //Check the original image height and width.
        m_background=
                (m_originalBackground.width()>m_originalBackground.height())?
                    //Also a horizontal image.
                    m_originalBackground.scaledToHeight(
                        height(),
                        Qt::SmoothTransformation):
                    //A vertical image.
                    m_originalBackground.scaledToWidth(
                         width(),
                         Qt::SmoothTransformation);
    }
    else
    {
        //Check the original image height and width.
        m_background=
                (m_originalBackground.width()<m_originalBackground.height())?
                    //Also a vertical image.
                    m_originalBackground.scaledToWidth(
                         width(),
                        Qt::SmoothTransformation):
                    //A horizontal image.
                    m_originalBackground.scaledToHeight(
                        height(),
                        Qt::SmoothTransformation);
    }
    //Draw the black color on the background.
    QPainter painter(&m_background);
    painter.fillRect(QRect(QPoint(0, 0), m_background.size()),
                     QColor(0, 0, 0, 215));
    painter.end();
}

