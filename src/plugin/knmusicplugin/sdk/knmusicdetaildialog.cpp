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

#include "knmusicdetaildialogpanel.h"
#include "knhtabgroup.h"
#include "knmusicglobal.h"
#include "knmusicparser.h"
#include "knhwidgetswitcher.h"

#include "knmusicdetaildialog.h"

#include <QDebug>

#define albumArtSize 100

using namespace MusicUtil;

KNMusicDetailDialog::KNMusicDetailDialog(QWidget *parent) :
    KNMessageBox(parent),
    m_panelList(QLinkedList<KNMusicDetailDialogPanel *>()),
    m_panelSwitcher(new KNHTabGroup(this)),
    m_panelContainer(new KNHWidgetSwitcher(this))
{
    //Set properties.
    setTitleText("Information");
    //Configure the panel switcher.
    m_panelSwitcher->setFixedHeight(25);

    //Link the switcher and container.
    connect(m_panelSwitcher, &KNHTabGroup::currentIndexChange,
            m_panelContainer, &KNHWidgetSwitcher::setCurrentIndex);

    //Initial the basic information labels
    for(int i=0; i<BasicInformationCount; i++)
    {
        //Simply initial the label.
        m_basicInfoLabel[i]=new QLabel(this);
    }
    //Configure the basic information labels.
    m_basicInfoLabel[LabelAlbumArt]->setFixedSize(albumArtSize, albumArtSize);
    m_basicInfoLabel[LabelAlbumArt]->setAlignment(Qt::AlignCenter);

    //Initial the widget container.
    QWidget *widgetContainer=new QWidget(this);
    widgetContainer->setFixedSize(460, 400);
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          widgetContainer);
    mainLayout->setContentsMargins(17,12,17,12);
    mainLayout->setSpacing(9);
    widgetContainer->setLayout(mainLayout);
    //Initial the basic information layout.
    QBoxLayout *basicInfoLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               mainLayout->widget());
    basicInfoLayout->setContentsMargins(0,0,0,0);
    basicInfoLayout->setSpacing(6);
    mainLayout->addLayout(basicInfoLayout);
    //Add widget to basic information layout.
    basicInfoLayout->addWidget(m_basicInfoLabel[LabelAlbumArt]);
    //Initial the basic text information layout.
    QBoxLayout *basicTextInfoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                   basicInfoLayout->widget());
    basicTextInfoLayout->setContentsMargins(0,0,0,0);
    basicTextInfoLayout->setSpacing(6);
    basicInfoLayout->addLayout(basicTextInfoLayout);
    //Add widgets to basic text information layout.
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelName]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelDuration]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelArtist]);
    basicTextInfoLayout->addWidget(m_basicInfoLabel[LabelAlbum]);
    basicTextInfoLayout->addStretch();
    //Add widget to main layout.
    mainLayout->addWidget(m_panelSwitcher);
    mainLayout->addWidget(m_panelContainer, 1);
    //Set the detail widget container as the content.
    setContentWidget(widgetContainer);
}

void KNMusicDetailDialog::addPanel(KNMusicDetailDialogPanel *panel)
{
    //Add panel to the panel container.
    m_panelContainer->addWidget(panel);
    //Add the switcher button to switcher.
    m_panelSwitcher->addTab(panel->tabButton());
    //Save the panel to panel list.
    m_panelList.append(panel);
}

void KNMusicDetailDialog::showDialog(const QString &filePath,
                                     const QString &indexFilePath,
                                     const int &index)
{
    //Analysis the song first.
    KNMusicAnalysisItem analysisItem;
    //Get the parser.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Check the index file path is empty or not.
    if(indexFilePath.isEmpty())
    {
        //Parse it as a single music file.
        parser->parseFile(filePath, analysisItem);
    }
    else
    {
        //Generate the analysis list.
        QList<KNMusicAnalysisItem> analysisItemList;
        //Parse the track list file.
        parser->parseTrackList(indexFilePath,
                               analysisItemList);
        //Check the index of the analysis file.
        if(index>-1 && index<analysisItemList.size())
        {
            analysisItem=analysisItemList.at(index);
        }
        else
        {
            //Parse it as a single music file.
            parser->parseFile(filePath, analysisItem);
        }
    }
    //Parse the cover image.
    parser->parseAlbumArt(analysisItem);

    //Check whether the album art is null.
    if(analysisItem.coverImage.isNull())
    {
        ;
    }
    else
    {
        m_basicInfoLabel[LabelAlbumArt]->setPixmap(
                    QPixmap::fromImage(analysisItem.coverImage).scaled(
                        albumArtSize,
                        albumArtSize,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    }
    //Get the detail information.
    const KNMusicDetailInfo &detailInfo=analysisItem.detailInfo;
    //Set the text basic information.
    m_basicInfoLabel[LabelName]->setText(detailInfo.textLists[Name]);
    m_basicInfoLabel[LabelDuration]->setText(detailInfo.textLists[Time]);
    m_basicInfoLabel[LabelArtist]->setText(detailInfo.textLists[Artist]);
    m_basicInfoLabel[LabelAlbum]->setText(detailInfo.textLists[Album]);

    //Ask all the panel to update the information.
    for(QLinkedList<KNMusicDetailDialogPanel *>::iterator i=m_panelList.begin();
        i!=m_panelList.end();
        ++i)
    {
        //Update the panel information.
        (*i)->setAnalysisItem(analysisItem);
    }
    //Show the dialog.
    exec();
}

