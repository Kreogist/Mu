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

#include "knlocalemanager.h"
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
    mainLayout->setContentsMargins(17,12,17,12);
    mainLayout->setSpacing(9);
    setLayout(mainLayout);

    //Initial the album art layout
    QBoxLayout *albumArtLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                              mainLayout->widget());
    albumArtLayout->setContentsMargins(0,0,0,0);
    albumArtLayout->setSpacing(16);
    mainLayout->addLayout(albumArtLayout);

    //Initial album art.
    m_albumArt=new QLabel(this);
    m_albumArt->setFixedSize(100, 100);
    albumArtLayout->addWidget(m_albumArt);

    //Initial the basic info layout.
    QBoxLayout *basicInfoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                               mainLayout->widget());
    basicInfoLayout->setContentsMargins(0,0,0,0);
    basicInfoLayout->setSpacing(6);
    albumArtLayout->addLayout(basicInfoLayout);

    //Initial the labels.
    initialBasicInfoLabel();
    basicInfoLayout->addWidget(m_title);
    basicInfoLayout->addWidget(m_duration);
    basicInfoLayout->addWidget(m_artist);
    basicInfoLayout->addWidget(m_album);
    basicInfoLayout->addStretch();

    //Initial the overview form layout.
    QFormLayout *infoFormLayout=new QFormLayout(mainLayout->widget());
    infoFormLayout->setVerticalSpacing(9);
    infoFormLayout->setLabelAlignment(Qt::AlignRight);
    infoFormLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    infoFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    mainLayout->addLayout(infoFormLayout, 1);

    //Initial the detail infos.
    initialDetailInfoLabel();
    for(int i=0; i<DetailInformationCount; i++)
    {
        infoFormLayout->addRow(m_detailInfoCaption[i],
                               m_detailInfo[i]);
    }

    //Link require.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicDetailOverview::retranslate);
    //Retranslate.
    retranslate();
}

void KNMusicDetailOverview::setFilePath(const QString &filePath)
{
    KNMusicDetailInfo currentInfo;
    //Analysis the file.
    KNMusicGlobal::parser()->parseFile(filePath, currentInfo);
    KNMusicGlobal::parser()->parseAlbumArt(currentInfo);
    //Set the data.
    QPixmap albumArtPixmap=QPixmap::fromImage(currentInfo.coverImage);
    if(albumArtPixmap.isNull())
    {
        albumArtPixmap=KNMusicGlobal::instance()->noAlbumArt();
    }
    m_albumArt->setPixmap(albumArtPixmap.scaled(QSize(100, 100),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
    m_title->setText(currentInfo.textLists[Name]);
    m_duration->setText(KNMusicGlobal::msecondToString(currentInfo.duration));
    m_artist->setText(currentInfo.textLists[Artist]);
    m_album->setText(currentInfo.textLists[Album]);
}

void KNMusicDetailOverview::retranslate()
{
    m_detailInfoCaption[Kind]->setText(tr("Kind"));
    m_detailInfoCaption[Duration]->setText(tr("Duration"));
    m_detailInfoCaption[Size]->setText(tr("Size"));
    m_detailInfoCaption[BitRate]->setText(tr("Bit Rate"));
    m_detailInfoCaption[SampleRate]->setText(tr("Sample Rate"));
    m_detailInfoCaption[DateModified]->setText(tr("Date Modified"));
}

void KNMusicDetailOverview::initialBasicInfoLabel()
{
    //Initial title label.
    m_title=new QLabel(this);
    QPalette infoPalette=m_title->palette();
    infoPalette.setColor(QPalette::WindowText, QColor(0x51, 0x51, 0x51));
    m_title->setPalette(infoPalette);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(20);
    m_title->setFont(titleFont);

    //Initial duration label.
    m_duration=new QLabel(this);
    m_duration->setPalette(infoPalette);
    QFont detailInfoFont=m_duration->font();
    detailInfoFont.setPixelSize(12);
    m_duration->setFont(detailInfoFont);

    //Initial artist label.
    m_artist=new QLabel(this);
    m_artist->setPalette(infoPalette);
    m_artist->setFont(detailInfoFont);

    //Initial album label.
    m_album=new QLabel(this);
    m_album->setPalette(infoPalette);
    m_album->setFont(detailInfoFont);
}

void KNMusicDetailOverview::initialDetailInfoLabel()
{
    //Initial the first caption.
    m_detailInfoCaption[0]=new QLabel(this);
    QFont captionFont=m_detailInfoCaption[0]->font();
    captionFont.setPixelSize(14);
    m_detailInfoCaption[0]->setFont(captionFont);
    QPalette captionPalette=m_detailInfoCaption[0]->palette();
    captionPalette.setColor(QPalette::WindowText, QColor(0xa0, 0xa0, 0xa0));
    m_detailInfoCaption[0]->setPalette(captionPalette);
    //Initial the first data field.
    m_detailInfo[0]=new QLabel(this);
    QPalette infoPalette=m_detailInfo[0]->palette();
    infoPalette.setColor(QPalette::WindowText, QColor(0x51, 0x51, 0x51));
    m_detailInfo[0]->setPalette(infoPalette);

    //Initial other caption.
    for(int i=1; i<DetailInformationCount; i++)
    {
        //Initial the caption.
        m_detailInfoCaption[i]=new QLabel(this);
        m_detailInfoCaption[i]->setPalette(captionPalette);
        m_detailInfoCaption[i]->setFont(captionFont);
        //Initial the data field.
        m_detailInfo[i]=new QLabel(this);
        m_detailInfo[i]->setPalette(infoPalette);
    }
}
