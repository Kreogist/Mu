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
#include "knfilepathlabel.h"
#include "knmusicglobal.h"
#include "knmusicparser.h"

#include "knmusicdetailoverview.h"

#include <QDebug>

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

    //Initial the detail info form layout.
    QFormLayout *detailLayout=new QFormLayout(mainLayout->widget());
    detailLayout->setVerticalSpacing(9);
    detailLayout->setLabelAlignment(Qt::AlignRight);
    detailLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    detailLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    mainLayout->addLayout(detailLayout);

    //Initial the detail infos.
    initialDetailInfoLabel();
    for(int i=0; i<DetailInformationCount; i++)
    {
        detailLayout->addRow(m_detailInfoCaption[i],
                               m_detailInfo[i]);
    }

    //Initila the file path info.
    QBoxLayout *pathLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          mainLayout->widget());
    pathLayout->setContentsMargins(0,0,0,0);
    pathLayout->setSpacing(9);
    mainLayout->addLayout(pathLayout, 1);
    pathLayout->addWidget(m_filePathCaption, 0, Qt::AlignTop);
    pathLayout->addWidget(m_filePathDataField, 1, Qt::AlignTop);

    //Link require.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicDetailOverview::retranslate);
    //Retranslate.
    retranslate();
}

void KNMusicDetailOverview::setFilePath(const QString &filePath)
{
    KNMusicAnalysisItem analysisItem;
    //Analysis the file.
    KNMusicGlobal::parser()->parseFile(filePath, analysisItem);
    KNMusicGlobal::parser()->parseAlbumArt(analysisItem);
    //Get detail info.
    KNMusicDetailInfo &currentInfo=analysisItem.detailInfo;
    //Set the caption data.
    QPixmap albumArtPixmap=QPixmap::fromImage(analysisItem.coverImage);
    if(albumArtPixmap.isNull())
    {
        albumArtPixmap=KNMusicGlobal::instance()->noAlbumArt();
    }
    m_albumArt->setPixmap(albumArtPixmap.scaled(QSize(100, 100),
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
    setEliedText(m_title, currentInfo.textLists[Name], 310);
    setEliedText(m_artist, currentInfo.textLists[Artist], 310);
    setEliedText(m_album, currentInfo.textLists[Album], 310);
    m_duration->setText(KNMusicGlobal::msecondToString(currentInfo.duration));

    //Set the detail info data.
    m_detailInfo[DetailYear]->setText(currentInfo.textLists[Year]);
    m_detailInfo[DetailGenre]->setText(currentInfo.textLists[Genre]);
    m_detailInfo[DetailKind]->setText(currentInfo.textLists[Kind]);
    m_detailInfo[DetailSize]->setText(currentInfo.textLists[Size]);
    m_detailInfo[DetailBitRate]->setText(currentInfo.textLists[BitRate]);
    m_detailInfo[DetailSampleRate]->setText(currentInfo.textLists[SampleRate]);
    m_detailInfo[DetailDateModified]->setText(
                currentInfo.textLists[DateModified]);
    m_filePathDataField->setText(currentInfo.filePath);
    m_filePathDataField->setFilePath(currentInfo.filePath);
}

void KNMusicDetailOverview::retranslate()
{
    m_detailInfoCaption[DetailYear]->setText(tr("Year"));
    m_detailInfoCaption[DetailGenre]->setText(tr("Genre"));
    m_detailInfoCaption[DetailKind]->setText(tr("Kind"));
    m_detailInfoCaption[DetailSize]->setText(tr("Size"));
    m_detailInfoCaption[DetailBitRate]->setText(tr("Bit Rate"));
    m_detailInfoCaption[DetailSampleRate]->setText(tr("Sample Rate"));
    m_detailInfoCaption[DetailDateModified]->setText(tr("Date Modified"));
    m_filePathCaption->setText(tr("Location"));
}

inline void KNMusicDetailOverview::initialBasicInfoLabel()
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

inline void KNMusicDetailOverview::initialDetailInfoLabel()
{
    //Initial the first caption.
    m_detailInfoCaption[0]=new QLabel(this);
    QFont captionFont=m_detailInfoCaption[0]->font();
    captionFont.setPixelSize(14);
    m_detailInfoCaption[0]->setFont(captionFont);
    QPalette captionPalette=m_detailInfoCaption[0]->palette();
    captionPalette.setColor(QPalette::WindowText, QColor(0xa0, 0xa0, 0xa0));
    m_detailInfoCaption[0]->setPalette(captionPalette);
    m_detailInfoCaption[0]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_detailInfoCaption[0]->setMinimumWidth(100);
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
        m_detailInfoCaption[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_detailInfoCaption[i]->setPalette(captionPalette);
        m_detailInfoCaption[i]->setMinimumWidth(100);
        m_detailInfoCaption[i]->setFont(captionFont);
        //Initial the data field.
        m_detailInfo[i]=new QLabel(this);
        m_detailInfo[i]->setPalette(infoPalette);
    }

    //Initial file path caption.
    m_filePathCaption=new QLabel(this);
    m_filePathCaption->setMinimumWidth(100);
    m_filePathCaption->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_filePathCaption->setPalette(captionPalette);
    m_filePathCaption->setFont(captionFont);
    //Initial the data field.
    m_filePathDataField=new KNFilePathLabel(this);
    m_filePathDataField->setWordWrap(true);
    m_filePathDataField->setPalette(infoPalette);
}

void KNMusicDetailOverview::setEliedText(QLabel *label,
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
