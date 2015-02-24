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
#include <QGridLayout>

#include "knglobal.h"

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

    //Initial the widgets.
    initialAlbumArt();
    initialInformationPanel();
    initialLyricsPanel();
    initialPlaylistPanel();
    initialControlPanel();

    //Initial main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QBoxLayout *panelLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           mainLayout->widget());
    panelLayout->setContentsMargins(0,0,0,0);
    panelLayout->setSpacing(0);
    mainLayout->addLayout(panelLayout);

    panelLayout->addWidget(m_albumArt);
    panelLayout->addLayout(m_informationPanelLayout);


    //Link retranslate request.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicMainPlayer::retranslate);
    retranslate();
}

KNMusicMainPlayer::~KNMusicMainPlayer()
{
    ;
}

void KNMusicMainPlayer::retranslate()
{
    m_informationElementCaptions[ElementTitle]->setText(tr("Title"));
    m_informationElementCaptions[ElementArtist]->setText(tr("Artist"));
    m_informationElementCaptions[ElementAlbum]->setText(tr("Album"));
    m_informationElementCaptions[ElementGenre]->setText(tr("Genre"));
    m_informationElementCaptions[ElementYear]->setText(tr("Year"));
    m_informationElementCaptions[ElementQuality]->setText(tr("Quality"));
    m_informationElementCaptions[ElementLocation]->setText(tr("Location"));
}

void KNMusicMainPlayer::resizeEvent(QResizeEvent *event)
{
    //Do resize.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Calculate the information layout spacing.
    m_informationPanelLayout->setVerticalSpacing(height()/40);
    int fontSize=height()/40;
    if(fontSize<15)
    {
        fontSize=15;
    }
    QFont captionFont=m_informationElementCaptions[0]->font();
    captionFont.setPixelSize(fontSize);
    for(int i=0; i<InformationElementsCount; i++)
    {
        m_informationElementCaptions[i]->setFont(captionFont);
    }
}

void KNMusicMainPlayer::initialAlbumArt()
{
    m_albumArt=new QLabel(this);
}

void KNMusicMainPlayer::initialInformationPanel()
{
    //We will set the parent later.
    m_informationPanelLayout=new QGridLayout;

    m_informationPanelLayout->setRowStretch(0, 1);
    //Initial the element labels.
    for(int i=0; i<InformationElementsCount; i++)
    {
        //Initial the element caption and text label.
        m_informationElementCaptions[i]=new QLabel(this);
        m_informationElements[i]=new QLabel(this);
        //Add to layout.
        m_informationPanelLayout->addWidget(m_informationElementCaptions[i], i+1, 1, Qt::AlignLeft);
        m_informationPanelLayout->addWidget(m_informationElements[i], i+1, 2, Qt::AlignLeft);
    }
    m_informationPanelLayout->setRowStretch(InformationElementsCount+1, 1);
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
