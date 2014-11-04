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
#include <QLabel>

#include "knmusicglobal.h"
#include "knmusicparser.h"

#include "knmusicdetailoverview.h"

KNMusicDetailOverview::KNMusicDetailOverview(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                         this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the album art layout
    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              mainLayout->widget());
    albumArtLayout->setContentsMargins(0,0,0,0);
    albumArtLayout->setSpacing(0);
    mainLayout->addLayout(albumArtLayout);

    //Initial album art.
    m_albumArt=new QLabel(this);
    m_albumArt->setFixedSize(100, 100);
    albumArtLayout->addWidget(m_albumArt);

    //Initial the basic info layout.
    QBoxLayout *basicInfoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                               mainLayout->widget());
    basicInfoLayout->setContentsMargins(0,0,0,0);
    basicInfoLayout->setSpacing(0);
    albumArtLayout->addLayout(basicInfoLayout);

    //Initial the labels.
    m_title=new QLabel(this);
    basicInfoLayout->addWidget(m_title);
    m_duration=new QLabel(this);
    basicInfoLayout->addWidget(m_duration);
    m_artist=new QLabel(this);
    basicInfoLayout->addWidget(m_artist);
    m_album=new QLabel(this);
    basicInfoLayout->addWidget(m_album);
}

void KNMusicDetailOverview::setFilePath(const QString &filePath)
{
    KNMusicDetailInfo currentInfo;
    //Analysis the file.
    KNMusicGlobal::parser()->parseFile(filePath, currentInfo);
    KNMusicGlobal::parser()->parseAlbumArt(currentInfo);
    //Set the data.
    QPixmap albumArtPixmap=QPixmap::fromImage(currentInfo.coverImage);
    m_albumArt->setPixmap(albumArtPixmap.scaled(QSize(100, 100),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
    m_title->setText(currentInfo.textLists[Name]);
    m_artist->setText(currentInfo.textLists[Artist]);
    m_album->setText(currentInfo.textLists[Album]);
}
