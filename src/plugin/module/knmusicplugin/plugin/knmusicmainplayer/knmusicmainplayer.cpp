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
#include <QPropertyAnimation>
#include <QBoxLayout>
#include <QLabel>

#include "knhighlightlabel.h"

#include "knmusicmainplayer.h"

#include <QDebug>

KNMusicMainPlayer::KNMusicMainPlayer(QWidget *parent) :
    KNMusicMainPlayerBase(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,220));
    setPalette(pal);
    //Initial the widgets.
    initialWidgets();
    //Initial the layouts.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QBoxLayout *playerLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    playerLayout->setContentsMargins(0,0,0,0);
    playerLayout->setSpacing(0);
    mainLayout->addLayout(playerLayout);

    QBoxLayout *artworkLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    artworkLayout->setContentsMargins(0,0,0,0);
    artworkLayout->setSpacing(0);
    playerLayout->addLayout(artworkLayout);

    artworkLayout->addWidget(m_albumArt);

    QBoxLayout *detailLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    detailLayout->setContentsMargins(0,0,0,0);
    detailLayout->setSpacing(0);
    artworkLayout->addLayout(detailLayout);

    detailLayout->addWidget(m_detail);
    detailLayout->addWidget(m_title);

    //--Debug--.
    m_detail->setText("BiBi - Cutie Panther");
    m_title->setText("Cutie Panther");
}

KNMusicMainPlayer::~KNMusicMainPlayer()
{
    ;
}

void KNMusicMainPlayer::resizeEvent(QResizeEvent *event)
{
    //Apply resize.
    KNMusicMainPlayerBase::resizeEvent(event);
    //Resize widgets.
    int sizeParameter=qMin(width(), height());
    float artworkSize=(float)sizeParameter*0.32;
    m_albumArt->resize(artworkSize, artworkSize);
    //Resize font.
    int captionFontSize=qMax((int)(artworkSize*0.08), 15),
        titleFontSize=qMax((int)(artworkSize*0.13), 30);
    m_captionFont.setPixelSize(captionFontSize);
    m_titleFont.setPixelSize(titleFontSize);
    //Set the font to widget.
    m_detail->setFont(m_captionFont);
    m_title->setFont(m_titleFont);
}

inline void KNMusicMainPlayer::initialWidgets()
{
    //Initial the album art label.
    m_albumArt=new KNHighlightLabel(this);

    //Initial the captions.
    m_detail=new QLabel(this);
    QPalette pal=m_detail->palette();
    pal.setColor(QPalette::WindowText, QColor(0x99, 0x99, 0x99));
    m_detail->setPalette(pal);

    m_title=new QLabel(this);
    pal=m_title->palette();
    pal.setColor(QPalette::WindowText, QColor(0xff, 0xa5, 0x00));
    m_title->setPalette(pal);

    //Initial the font.
    m_captionFont=m_detail->font();
    m_titleFont=m_title->font();
}
