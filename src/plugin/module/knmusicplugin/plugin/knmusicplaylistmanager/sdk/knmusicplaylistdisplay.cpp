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
#include <QFileDialog>
#include <QLabel>

#include "knglobal.h"

#include "knmousesensewidget.h"
#include "knconnectionhandler.h"
#include "knsideshadowwidget.h"
#include "knopacityanimebutton.h"

#include "knmusicsearchbase.h"
#include "knmusicplaylistmodel.h"
#include "knmusicplaylistlistitem.h"
#include "knmusicplaylisttreeview.h"

#include "knmusicplaylistdisplay.h"

#include <QDebug>

KNMusicPlaylistDisplay::KNMusicPlaylistDisplay(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Add layout.
    QBoxLayout *displayLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    displayLayout->setContentsMargins(0,0,0,0);
    displayLayout->setSpacing(0);
    setLayout(displayLayout);

    //Initial the labels.
    initialLabels();
    initialTreeView();
    initialControls();

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
    infoLayout->addWidget(m_playlistTitle);
    QBoxLayout *detailLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            infoLayout->widget());
    detailLayout->setContentsMargins(0,0,0,0);
    detailLayout->setSpacing(5);
    detailLayout->addWidget(m_playlistDetail);
    detailLayout->addWidget(m_playPlaylist);
    detailLayout->addWidget(m_addToPlaylist);
    detailLayout->addStretch();
    infoLayout->addLayout(detailLayout);

    displayLayout->addWidget(m_playlistTreeView, 1);

    //Initial playlist connection handler.
    m_modelSignalHandler=new KNConnectionHandler(this);

    //Initial the shadow.
    m_leftShadow=new KNSideShadowWidget(LeftShadow, this);

    //Connect search signal.
    connect(KNMusicGlobal::musicSearch(), SIGNAL(requireSearch(QString)),
            this, SLOT(updatePlaylistTitle()));

    //Connect retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicPlaylistDisplay::retranslate);
    //Do retranslate.
    retranslate();
}

KNMusicPlaylistListItem *KNMusicPlaylistDisplay::currentItem()
{
    return m_currentItem;
}

void KNMusicPlaylistDisplay::setMusicTab(KNMusicTab *tab)
{
    m_playlistTreeView->setMusicTab(tab);
}

void KNMusicPlaylistDisplay::searchText(const QString &text)
{
    m_playlistTreeView->searchText(text);
}

void KNMusicPlaylistDisplay::displayPlaylistItem(KNMusicPlaylistListItem *item)
{
    //Reset the connection.
    m_modelSignalHandler->disconnectAll();
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
                connect(musicModel, &KNMusicPlaylistModel::itemChanged,
                        this, &KNMusicPlaylistDisplay::onActionRowChanged));
    m_modelSignalHandler->addConnectionHandle(
                connect(musicModel, &KNMusicPlaylistModel::rowCountChanged,
                        this, &KNMusicPlaylistDisplay::onActionRowChanged));
    //Update the informations.
    updatePlaylistInfo();
}

void KNMusicPlaylistDisplay::scrollToSourceSongRow(const int &row)
{
    m_playlistTreeView->scrollToSourceSongRow(row);
}

void KNMusicPlaylistDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Resize the shadow.
    m_leftShadow->resize(15, height());
}

void KNMusicPlaylistDisplay::onActionRowChanged()
{
    //Update the detail info first.
    updateDetailInfo();
    //Set the changed flag to true.
    m_currentItem->setChanged(true);
}

void KNMusicPlaylistDisplay::onActionAddToPlaylist()
{
    QStringList preferAddFileLists=
            QFileDialog::getOpenFileNames(this, tr("Add to playlist %1").arg(m_currentItem->text()));
    if(!preferAddFileLists.isEmpty())
    {
        emit requireAddToPlaylist(m_currentItem->row(), preferAddFileLists);
    }
}

void KNMusicPlaylistDisplay::retranslate()
{
    m_songCount[0]=tr("No song, ");
    m_songCount[1]=tr("1 song, ");
    m_songCount[2]=tr("%1 songs, ");

    m_minuateCount[0]=tr("0 minuate.");
    m_minuateCount[1]=tr("1 minuate.");
    m_minuateCount[2]=tr("%1 minuates.");

    m_searchCount[0]=tr("No result.");
    m_searchCount[1]=tr("1 result.");
    m_searchCount[2]=tr("%1 results.");

    m_searchResultIn=tr("Search '%1' in '%2'");

    updatePlaylistTitle();
    updateDetailInfo();

    m_playPlaylist->setToolTip(tr("Play the current playlist."));
    m_addToPlaylist->setToolTip(tr("Add music to playlist."));
}

void KNMusicPlaylistDisplay::updatePlaylistInfo()
{
    //Clear the original data first.
    m_playlistTitle->clear();
    m_playlistDetail->clear();
    //If the current item is null, then ignore it.
    if(m_currentItem==nullptr)
    {
        return;
    }
    //Set the title.
    updatePlaylistTitle();
    //Update the detail.
    updateDetailInfo();
}

void KNMusicPlaylistDisplay::onActionRemoveCurrent()
{
    //Backup header first.
    m_playlistTreeView->backupHeader();
    //Set to model to nullptr.
    displayPlaylistItem(nullptr);
}

void KNMusicPlaylistDisplay::updateDetailInfo()
{
    //Reset the detail info label text.
    m_playlistDetail->clear();
    //Check is the item usable.
    if(m_currentItem==nullptr)
    {
        return;
    }
    //Check is now searching.
    if(!KNMusicGlobal::musicSearch()->searchText().isEmpty())
    {
        //Get the result count, is the proxy model's count.
        int searchCount=m_playlistTreeView->model()->rowCount();
        switch(searchCount)
        {
        case 0:
            m_playlistDetail->setText(m_searchCount[0]);
            break;
        case 1:
            m_playlistDetail->setText(m_searchCount[1]);
            break;
        default:
            m_playlistDetail->setText(m_searchCount[2].arg(QString::number(searchCount)));
            break;
        }
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
    m_playlistDetail->setText(playlistInfoText);
}

void KNMusicPlaylistDisplay::updatePlaylistTitle()
{
    if(m_currentItem!=nullptr)
    {
        m_playlistTitle->setText(
                    KNMusicGlobal::musicSearch()->searchText().isEmpty()?
                        m_currentItem->text():
                        m_searchResultIn.arg(KNMusicGlobal::musicSearch()->searchText(),
                                       m_currentItem->text()));
    }
    else
    {
        m_playlistTitle->clear();
    }
}

inline void KNMusicPlaylistDisplay::initialLabels()
{
    //Initial the playlist title label.
    m_playlistTitle=new QLabel(this);
    QPalette captionPal=m_playlistTitle->palette();
    captionPal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_playlistTitle->setPalette(captionPal);
    QFont titleFont=m_playlistTitle->font();
    titleFont.setPixelSize(17);
    titleFont.setBold(true);
    m_playlistTitle->setFont(titleFont);

    //Initial the playlist information title.
    m_playlistDetail=new QLabel(this);
    m_playlistDetail->setPalette(captionPal);
}

inline void KNMusicPlaylistDisplay::initialTreeView()
{
    //Initial the tree view.
    m_playlistTreeView=new KNMusicPlaylistTreeView(this);
    //Give the focus to the treeview.
    setFocusProxy(m_playlistTreeView);
    connect(m_playlistTreeView, &KNMusicPlaylistTreeView::searchComplete,
            this, &KNMusicPlaylistDisplay::updateDetailInfo);
}

void KNMusicPlaylistDisplay::initialControls()
{
    //Initial the play playlist button.
    m_playPlaylist=new KNOpacityAnimeButton(this);
    m_playPlaylist->setFixedSize(15, 15);
    m_playPlaylist->setIcon(QPixmap(":/plugin/music/playlist/playlist_control_play.png"));
    connect(m_playPlaylist, &KNOpacityAnimeButton::clicked,
            m_playlistTreeView, &KNMusicPlaylistTreeView::playCurrentrPlaylist);

    //Initial the playlist menu button.
    m_addToPlaylist=new KNOpacityAnimeButton(this);
    m_addToPlaylist->setFixedSize(15, 15);
    m_addToPlaylist->setIcon(QPixmap(":/plugin/music/playlist/playlist_control_add.png"));
    connect(m_addToPlaylist, &KNOpacityAnimeButton::clicked,
            this, &KNMusicPlaylistDisplay::onActionAddToPlaylist);
}
