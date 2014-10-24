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

#include "knmousesensewidget.h"
#include "knconnectionhandler.h"
#include "knmusicplaylistmodel.h"
#include "knmusicplaylistlistitem.h"
#include "knmusicplaylisttreeview.h"

#include "knmusicplaylistdisplay.h"

KNMusicPlaylistDisplay::KNMusicPlaylistDisplay(QWidget *parent) :
    KNDropProxyContainer(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Add layout.
    QBoxLayout *displayLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    displayLayout->setContentsMargins(0,0,0,0);
    displayLayout->setSpacing(0);
    setLayout(displayLayout);

    //Initial information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->setContentsMargins(20,12,0,8);

    QBoxLayout *infoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          infoContainer);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->setSpacing(0);
    infoContainer->setLayout(infoLayout);

    displayLayout->addWidget(infoContainer);

    //Initial the information label.
    m_playlistTitle=new QLabel(this);
    QPalette captionPal=m_playlistTitle->palette();
    captionPal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_playlistTitle->setPalette(captionPal);
    QFont titleFont=m_playlistTitle->font();
    titleFont.setPixelSize(17);
    titleFont.setBold(true);
    m_playlistTitle->setFont(titleFont);
    infoLayout->addWidget(m_playlistTitle);
    m_playlistInfo=new QLabel(this);
    m_playlistInfo->setPalette(captionPal);
    infoLayout->addWidget(m_playlistInfo);

    //Initial the tree view.
    m_playlistTreeView=new KNMusicPlaylistTreeView(this);
//    connect(m_playlistTreeView, &KNMusicPlaylistTreeView::requireRemoveItem,
//            this, &KNMusicPlaylistDisplay::requireRemoveItem);

    displayLayout->addWidget(m_playlistTreeView, 1);

    //Initial playlist connection handler.
    m_modelSignalHandler=new KNConnectionHandler(this);

    //Do retranslate.
    retranslate();
}

KNMusicPlaylistListItem *KNMusicPlaylistDisplay::currentItem()
{
    return m_currentItem;
}

void KNMusicPlaylistDisplay::displayPlaylistItem(KNMusicPlaylistListItem *item)
{
    //Reset the connection.
    m_modelSignalHandler->disConnectAll();
    //Backup the item.
    m_currentItem=item;
    if(m_currentItem==nullptr)
    {
        //Reset treeview.
        m_playlistTreeView->setMusicModel(nullptr);
        //Clear the detail info.
        updatePlaylistInfo();
        return;
    }
    //Set the model.
    KNMusicPlaylistModel *musicModel=m_currentItem->playlistModel();
    m_playlistTreeView->setMusicModel(musicModel);
    //When user add or remove file to playlist, should update the detail info.
    m_modelSignalHandler->addConnectionHandle(
                connect(musicModel, &KNMusicPlaylistModel::rowCountChanged,
                        this, &KNMusicPlaylistDisplay::updateDetailInfo));
    //Analysis file signal.
    m_modelSignalHandler->addConnectionHandle(
                connect(this, &KNMusicPlaylistDisplay::requireAnalysisFiles,
                        musicModel, &KNMusicPlaylistModel::requireAnalysisFiles));
    //Update the informations.
    updatePlaylistInfo();
}

void KNMusicPlaylistDisplay::retranslate()
{
    m_songCount[0]=tr("No song, ");
    m_songCount[1]=tr("1 song, ");
    m_songCount[2]=tr("%1 songs, ");

    m_minuateCount[0]=tr("0 minuate.");
    m_minuateCount[1]=tr("1 minuate.");
    m_minuateCount[2]=tr("%1 minuates.");

    updateDetailInfo();
}

void KNMusicPlaylistDisplay::updatePlaylistInfo()
{
    //Clear the original data first.
    m_playlistTitle->clear();
    m_playlistInfo->clear();
    //If the current item is null, then ignore it.
    if(m_currentItem==nullptr)
    {
        return;
    }
    //Set the title.
    m_playlistTitle->setText(m_currentItem->text());
    //Update the detail.
    updateDetailInfo();
}

void KNMusicPlaylistDisplay::updateDetailInfo()
{
    //Reset the detail info label text.
    m_playlistInfo->clear();
    //Check is the item usable.
    if(m_currentItem==nullptr)
    {
        return;
    }
    //Get the playlist model.
    KNMusicPlaylistModel *model=m_currentItem->playlistModel();
    if(model==nullptr)
    {
        return;
    }
    QString playlistInfoText;
    //First check the model row count, this will be used as song count.
    switch(model->rowCount())
    {
    case 0:
        playlistInfoText=m_songCount[0];
        break;
    case 1:
        playlistInfoText=m_songCount[1];
        break;
    default:
        playlistInfoText=m_songCount[2].arg(QString::number(model->rowCount()));
        break;
    }
    //Then calculate the minuates.
    int minuateCalulate=model->totalDuration()/60000;
    switch(minuateCalulate)
    {
    case 0:
        playlistInfoText+=m_minuateCount[0];
        break;
    case 1:
        playlistInfoText+=m_minuateCount[1];
        break;
    default:
        playlistInfoText+=m_minuateCount[2].arg(QString::number(minuateCalulate));
        break;
    }
    //Set detail info.
    m_playlistInfo->setText(playlistInfoText);
}
