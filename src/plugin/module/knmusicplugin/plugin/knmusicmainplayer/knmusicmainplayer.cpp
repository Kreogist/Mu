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
#include <QLabel>
#include <QBoxLayout>
#include <QFormLayout>

#include "knglobal.h"
#include "knlabelbutton.h"
#include "knopacityanimebutton.h"
#include "knprogressslider.h"

#include "knmusicheaderplayerbase.h"
#include "knmusicmainlyrics.h"
#include "knmusicglobal.h"

#include "knmusicmainplayer.h"

#include <QDebug>

KNMusicMainPlayer::KNMusicMainPlayer(QWidget *parent) :
    KNMusicMainPlayerBase(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,220));
    setPalette(pal);

    //Initial the default album pixmap to no album.
    m_albumArt=KNMusicGlobal::instance()->noAlbumArt();

    //Initial the hide main player button.
    m_hideMainPlayer=new KNOpacityAnimeButton(this);
    m_hideMainPlayer->move(16, 16);
    m_hideMainPlayer->setFixedSize(32, 32);
    m_hideMainPlayer->setIcon(QPixmap(":/plugin/music/player/hide_mainplayer.png"));
    connect(m_hideMainPlayer, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayer::onActionHideMainPlayer);

    //Initial the widgets.
    initialAlbumArt();
    initialInformationPanel();
    initialLyricsPanel();
    initialPlaylistPanel();
    initialControlPanel();
    initialBanner();

    //Initial main layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_mainLayout->widget());
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    m_mainLayout->addLayout(panelLayout, 1);

    QBoxLayout *songLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_mainLayout->widget());
    songLayout->setContentsMargins(0,0,0,0);
    songLayout->setSpacing(0);
    panelLayout->addLayout(songLayout, 1);

    songLayout->addWidget(m_albumArtLabel, 0, Qt::AlignBottom | Qt::AlignHCenter);

    QWidget *informationPanel=new QWidget(this);
    informationPanel->setLayout(m_infoPanelLayout);
    songLayout->addWidget(informationPanel, 0, Qt::AlignTop | Qt::AlignLeft);

    panelLayout->addWidget(m_mainLyrics, 2);

    m_mainLayout->addWidget(m_controlWidget);

    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicMainPlayer::retranslate);
    retranslate();
}

KNMusicMainPlayer::~KNMusicMainPlayer()
{
}

QWidget *KNMusicMainPlayer::banner()
{
    return m_banner;
}

void KNMusicMainPlayer::setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)
{
    //Save the header player.
    m_headerPlayer=headerPlayer;
    //Set the header player to main lyrics.
    m_mainLyrics->setHeaderPlayer(m_headerPlayer);
    //Link the header player.
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::analysisItemUpdated,
            this, &KNMusicMainPlayer::onActionAnalysisItemUpdate);
}

void KNMusicMainPlayer::retranslate()
{
    m_informationElementCaptions[ElementTitle]->setText(tr("Title"));
    m_informationElementCaptions[ElementArtist]->setText(tr("Artist"));
    m_informationElementCaptions[ElementAlbum]->setText(tr("Album"));
    m_informationElementCaptions[ElementGenre]->setText(tr("Genre"));
}

void KNMusicMainPlayer::onActionHideMainPlayer()
{
    //Hide the banner.
    m_banner->hide();
    //Emit hide main player signal.
    emit requireHideMainPlayer();
}

void KNMusicMainPlayer::onActionAnalysisItemUpdate()
{
    //Set detail information.
    const KNMusicAnalysisItem &analysisItem=
            m_headerPlayer->currentAnalysisItem();
    if(analysisItem.coverImage.isNull())
    {
        m_albumArt=KNMusicGlobal::instance()->noAlbumArt();
    }
    else
    {
        //Save the album art.
        m_albumArt=QPixmap::fromImage(analysisItem.coverImage);
    }
    //Update the panel.
    updateInformationPanel();
}

void KNMusicMainPlayer::showEvent(QShowEvent *event)
{
    //Show the banner.
    m_banner->show();
    //Start draw the lyrics.
    m_mainLyrics->setDrawLyrics(true);
    //Do show event.
    KNMusicMainPlayerBase::showEvent(event);
}

void KNMusicMainPlayer::hideEvent(QHideEvent *event)
{
    //Stop to draw lyrics.
    m_mainLyrics->setDrawLyrics(false);
    //Do hide event.
    KNMusicMainPlayerBase::hideEvent(event);
}

void KNMusicMainPlayer::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Calculate the information layout spacing.
    int parameterSize=qMin(width(), height());
    //Update pixmap size.
    m_pixmapSize=parameterSize/3;
    //Update the font size.
    int fontSize=parameterSize/40;
    m_infoPanelLayout->setVerticalSpacing(fontSize);
    if(fontSize<15)
    {
        fontSize=15;
    }
    m_mainLayout->setContentsMargins(fontSize, 0, fontSize, 0);
    m_mainLayout->setSpacing(fontSize);
    m_infoPanelLayout->setContentsMargins(0, fontSize, 0, fontSize);
    m_infoPanelLayout->setHorizontalSpacing(fontSize);

    //Change the font size.
    QFont captionFont=m_informationElementCaptions[0]->font();
    captionFont.setPixelSize(fontSize);
    int maxLabelWidth=0;
    for(int i=0; i<InformationElementsCount; i++)
    {
        maxLabelWidth=qMax(maxLabelWidth,
                           m_infoPanelLayout->itemAt(i, QFormLayout::LabelRole)->widget()->width());
        m_informationElementIcons[i]->setFixedSize(fontSize+5,
                                                   fontSize+5);
        m_informationElementCaptions[i]->setFont(captionFont);
        m_informationElements[i]->setFont(captionFont);
    }
    //Set lyrics font.
    QFont lyricsFont=m_mainLyrics->font();
    lyricsFont.setPixelSize(fontSize+(fontSize>>1));
    m_mainLyrics->setFont(lyricsFont);
    m_mainLyrics->setSpacing(fontSize>>1);
    m_mainLyrics->setLeftSpacing(fontSize<<1);
    //Calculate the elements label maximum width.
    m_maxElementWidth=width()/3-(fontSize+5+maxLabelWidth);
    //Update information.
    updateInformationPanel();
}

void KNMusicMainPlayer::initialAlbumArt()
{
    m_albumArtLabel=new QLabel(this);
    m_albumArtLabel->setAlignment(Qt::AlignCenter);
}

void KNMusicMainPlayer::initialInformationPanel()
{
    //We will set the parent later.
    m_infoPanelLayout=new QFormLayout;
    m_infoPanelLayout->setFormAlignment(Qt::AlignHCenter);

    QString elementIconPath[InformationElementsCount];
    elementIconPath[ElementTitle]=":/plugin/music/category/01_musics.png";
    elementIconPath[ElementArtist]=":/plugin/music/category/02_artists.png";
    elementIconPath[ElementAlbum]=":/plugin/music/category/03_ablums.png";
    elementIconPath[ElementGenre]=":/plugin/music/category/04_genres.png";

    //Initial the element labels.
    for(int i=0; i<InformationElementsCount; i++)
    {
        //Initial the element caption.
        QWidget *caption=new QWidget(this);
        caption->setContentsMargins(0,0,0,0);
        QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                 caption);
        captionLayout->setContentsMargins(0,0,0,0);
        captionLayout->setSpacing(15);
        caption->setLayout(captionLayout);
        //Initial the element icon.
        m_informationElementIcons[i]=new KNLabelButton(this);
        m_informationElementIcons[i]->setScaledContents(true);
        m_informationElementIcons[i]->setPixmap(QPixmap(elementIconPath[i]));
        captionLayout->addWidget(m_informationElementIcons[i]);
        //Initial text label.
        m_informationElementCaptions[i]=new KNLabelButton(this);
        captionLayout->addWidget(m_informationElementCaptions[i]);
        m_informationElements[i]=new KNLabelButton(this);
        //Add to layout.
        m_infoPanelLayout->addRow(caption,
                                  m_informationElements[i]);
    }
    QPalette captionPal=m_informationElementCaptions[0]->palette(),
             elementsPal=m_informationElements[0]->palette();
    captionPal.setColor(QPalette::WindowText, QColor(0xcc, 0xcc, 0xcc));
    elementsPal.setColor(QPalette::WindowText, QColor(0xff, 0xff, 0xff));
    for(int i=0; i<InformationElementsCount; i++)
    {
        m_informationElementCaptions[i]->setPalette(captionPal);
        m_informationElements[i]->setPalette(elementsPal);
    }
}

void KNMusicMainPlayer::updateInformationPanel()
{
    //Update album art.
    m_albumArtLabel->setPixmap(m_albumArt.scaled(m_pixmapSize,
                                                 m_pixmapSize,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
    //Update detail captions.
    const KNMusicDetailInfo &detailInfo=m_headerPlayer->currentAnalysisItem().detailInfo;
    setEliedLabelText(m_informationElements[ElementTitle], detailInfo.textLists[Name], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementArtist], detailInfo.textLists[Artist], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementAlbum], detailInfo.textLists[Album], m_maxElementWidth);
    setEliedLabelText(m_informationElements[ElementGenre], detailInfo.textLists[Genre], m_maxElementWidth);
}

void KNMusicMainPlayer::initialLyricsPanel()
{
    m_mainLyrics=new KNMusicMainLyrics(this);
    //Hide the lyrics by default.
    m_mainLyrics->setDrawLyrics(false);
}

void KNMusicMainPlayer::initialBanner()
{
    //Initial the banner.
    m_banner=new QWidget(this);
    //Hide the banner at default.
    m_banner->hide();
}

void KNMusicMainPlayer::initialPlaylistPanel()
{
    ;
}

void KNMusicMainPlayer::initialControlPanel()
{
    //Initial the control widget.
    m_controlWidget=new QWidget(this);

    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             m_controlWidget);
    controlLayout->setContentsMargins(0,0,0,0);
    controlLayout->setSpacing(0);
    m_controlWidget->setLayout(controlLayout);

    m_progress=new KNProgressSlider(this);
    controlLayout->addWidget(m_progress);

    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            controlLayout->widget());
    buttonLayout->setContentsMargins(0,0,0,0);
    buttonLayout->setSpacing(0);
    controlLayout->addLayout(buttonLayout);
}

void KNMusicMainPlayer::setEliedLabelText(QLabel *label,
                                          const QString &text,
                                          const int &width)
{
    if(label->fontMetrics().width(text)>width)
    {
        label->setText(label->fontMetrics().elidedText(text,
                                                       Qt::ElideRight,
                                                       width));
        label->setToolTip(text);
    }
    else
    {
        label->setText(text);
        label->setToolTip("");
    }
}
