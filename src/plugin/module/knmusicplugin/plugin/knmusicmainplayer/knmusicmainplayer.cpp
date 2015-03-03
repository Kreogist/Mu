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

#include "knmusicheaderplayerbase.h"
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

    //Initial the widgets.
    initialAlbumArt();
    initialInformationPanel();
    initialLyricsPanel();
    initialPlaylistPanel();
    initialControlPanel();

    //Initial main layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_mainLayout->widget());
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    m_mainLayout->addLayout(panelLayout);

    panelLayout->addWidget(m_albumArtLabel);

    QWidget *informationPanel=new QWidget(this);
    informationPanel->setLayout(m_infoPanelLayout);
    panelLayout->addWidget(informationPanel);

    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicMainPlayer::retranslate);
    retranslate();
}

KNMusicMainPlayer::~KNMusicMainPlayer()
{
    ;
}

void KNMusicMainPlayer::setHeaderPlayer(KNMusicHeaderPlayerBase *headerPlayer)
{
    m_headerPlayer=headerPlayer;
    connect(m_headerPlayer, &KNMusicHeaderPlayerBase::analysisItemUpdated,
            this, &KNMusicMainPlayer::onActionAnalysisItemUpdate);
}

void KNMusicMainPlayer::retranslate()
{
    m_informationElementCaptions[ElementTitle]->setText(tr("Title"));
    m_informationElementCaptions[ElementArtist]->setText(tr("Artist"));
    m_informationElementCaptions[ElementAlbum]->setText(tr("Album"));
    m_informationElementCaptions[ElementGenre]->setText(tr("Genre"));
    m_informationElementCaptions[ElementYear]->setText(tr("Year"));
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
    m_mainLayout->setSpacing(fontSize);
    m_infoPanelLayout->setContentsMargins(fontSize, 0, fontSize, 0);
    m_mainLayout->setContentsMargins(fontSize, 0, fontSize, 0);
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
    m_infoPanelLayout->setFormAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QString elementIconPath[InformationElementsCount];
    elementIconPath[ElementTitle]=":/plugin/music/category/01_musics.png";
    elementIconPath[ElementArtist]=":/plugin/music/category/02_artists.png";
    elementIconPath[ElementAlbum]=":/plugin/music/category/03_ablums.png";
    elementIconPath[ElementGenre]=":/plugin/music/category/04_genres.png";
    elementIconPath[ElementYear]=":/plugin/music/category/06_year.png";

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
    setEliedLabelText(m_informationElements[ElementYear], detailInfo.textLists[Year], m_maxElementWidth);
}

void KNMusicMainPlayer::initialLyricsPanel()
{
    ;
}

void KNMusicMainPlayer::initialPlaylistPanel()
{
    ;
}

void KNMusicMainPlayer::initialControlPanel()
{
    ;
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
