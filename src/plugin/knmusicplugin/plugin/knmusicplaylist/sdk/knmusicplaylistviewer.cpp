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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>

#include "knconnectionhandler.h"
#include "knmousesensewidget.h"
#include "knsideshadowwidget.h"
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knscrolllabel.h"

#include "knmusicsearchbase.h"
#include "knmusicplaylisttreeview.h"
#include "knmusicplaylistmodel.h"
#include "knmusicproxymodel.h"
#include "knmusicglobal.h"

#include "knmusicplaylistviewer.h"

#include <QDebug>

#define ShadowWidth 15

KNMusicPlaylistViewer::KNMusicPlaylistViewer(QWidget *parent, KNMusicTab *tab) :
    QWidget(parent),
    m_treeView(new KNMusicPlaylistTreeView(this, tab)),
    m_title(new KNScrollLabel(this)),
    m_detail(new QLabel(this)),
    m_leftShadow(new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                        this)),
    m_modelLinkHandler(new KNConnectionHandler())
{
    //Configure the tree view.
    setFocusProxy(m_treeView);
    //Configure title label.
    m_title->setObjectName("PlaylistViewerLabel");
    m_title->setContentsMargins(0,0,0,0);
    QFont labelFont=m_title->font();
    labelFont.setBold(true);
    labelFont.setPixelSize(17);
    m_title->setFont(labelFont);
    //Link the theme manager with the label.
    knTheme->registerWidget(m_title);
    //Configure detail label.
    m_detail->setObjectName("PlaylistDetailLabel");
    knTheme->registerWidget(m_detail);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->updateObjectName("PlaylistInformationContainer");
    infoContainer->setContentsMargins(20, 12, 20, 8);
    //Add widget to layout.
    mainLayout->addWidget(infoContainer);
    mainLayout->addWidget(m_treeView, 1);
    //Initial the information container layout.
    QBoxLayout *informationLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                 infoContainer);
    informationLayout->setContentsMargins(0,0,0,0);
    informationLayout->setSpacing(0);
    infoContainer->setLayout(informationLayout);
    //Add labels to information layout.
    informationLayout->addWidget(m_title);
    informationLayout->addWidget(m_detail);

    //Link the search.
    connect(knMusicGlobal->search(), &KNMusicSearchBase::requireSearch,
            this, &KNMusicPlaylistViewer::onActionSearch);

    //Move the shadow to the top of the widget.
    m_leftShadow->setDarkness(100);
    m_leftShadow->raise();

    //Link with locale manager.
    knI18n->link(this, &KNMusicPlaylistViewer::retranslate);
    retranslate();
}

KNMusicPlaylistViewer::~KNMusicPlaylistViewer()
{
    //Disconnect all the connections in the model link handler.
    m_modelLinkHandler->disconnectAll();
    //Delete the handler.
    delete m_modelLinkHandler;
}

KNMusicPlaylistModel *KNMusicPlaylistViewer::playlist()
{
    //Check the music model of the playlist treeview.
    if(m_treeView->musicModel())
    {
        //Cast the music model as playlist model.
        return static_cast<KNMusicPlaylistModel *>(m_treeView->musicModel());
    }
    //Or else just return a nullptr model.
    return nullptr;
}

void KNMusicPlaylistViewer::setPlaylist(KNMusicPlaylistModel *model)
{
    //Disconnect with the previous playlist.
    m_modelLinkHandler->disconnectAll();
    //Check whether the model is null.
    if(model)
    {
        //Check whether the model has been built from the stored data before.
        if(!model->isBuilt())
        {
            //Build the model.
            model->buildModel();
        }
        //Set the model to playlist tree view.
        m_treeView->setMusicModel(model);
        //Link the model with the details.
        m_modelLinkHandler->append(
                 connect(model, &KNMusicPlaylistModel::rowCountChanged,
                         this,
                         &KNMusicPlaylistViewer::onActionModelRowCountChanged));
        m_modelLinkHandler->append(
                    connect(model, &KNMusicPlaylistModel::titleChanged,
                            this, &KNMusicPlaylistViewer::updateTitle));
    }
    else
    {
        //For nullptr model, we have to clear all the information.
        m_treeView->setMusicModel(nullptr);
    }
    //Update the playlist information.
    updateTitle();
    //Update the detail information.
    updateDetailInfo();
}

void KNMusicPlaylistViewer::selectSong(const QModelIndex &sourceIndex)
{
    //Select the index.
    m_treeView->selectSourceSong(sourceIndex.row());
}

void KNMusicPlaylistViewer::resizeEvent(QResizeEvent *event)
{
    //Resize the viewer.
    QWidget::resizeEvent(event);
    //Resize the shadow,
    m_leftShadow->resize(ShadowWidth, height());
}

void KNMusicPlaylistViewer::retranslate()
{
    m_songCount[0]=tr("No song, ");
    m_songCount[1]=tr("1 song, ");
    m_songCount[2]=tr("%1 songs, ");

    m_minuateCount[0]=tr("0 minuate.");
    m_minuateCount[1]=tr("1 minuate.");
    m_minuateCount[2]=tr("%1 minuates.");

    m_hourCount[0]=tr("1 hour and ");
    m_hourCount[1]=tr("%1 hours and ");

    m_hourCountWithoutMinuate[0]=tr("1 hour.");
    m_hourCountWithoutMinuate[1]=tr("%1 hours.");

    m_searchCount[0]=tr("No result.");
    m_searchCount[1]=tr("1 result.");
    m_searchCount[2]=tr("%1 results.");

    m_searchResultIn=tr("Search result in %1");
}

void KNMusicPlaylistViewer::onActionModelRowCountChanged()
{
    //Update the detail information.
    updateDetailInfo();
}

void KNMusicPlaylistViewer::onActionSearch()
{
    //Check whether the proxy model is nullptr.
    if(m_treeView->proxyModel()==nullptr)
    {
        //Ignore the search request.
        return;
    }
    //Set the search rules to the proxy model.
    m_treeView->proxyModel()->setSearchBlocks(knMusicGlobal->search()->rules());
    //Update the title and detail info.
    updateTitle();
    updateDetailInfo();
}

void KNMusicPlaylistViewer::updateTitle()
{
    //Clear the title.
    m_title->setText("");
    //Check whether the proxy model is nullptr.
    if(m_treeView->proxyModel()==nullptr)
    {
        //Ignore the title update request.
        return;
    }
    //Get the proxy model.
    KNMusicProxyModel *proxyModel=m_treeView->proxyModel();
    //Check the proxy model.
    if(!proxyModel)
    {
        //Ignore the null model.
        return;
    }
    //Get the playlist model of the tree view.
    KNMusicPlaylistModel *model=
            static_cast<KNMusicPlaylistModel *>(m_treeView->musicModel());
    //Check the music model.
    if(!model)
    {
        //Ignore the null model.
        return;
    }
    //Check whether the proxy model is in search mode.
    if(proxyModel->isSearchMode())
    {
        //Update the search request.
        m_title->setText(m_searchResultIn.arg(model->title()));
        return;
    }
    //Simply set the name of the playlist to be the title.
    m_title->setText(model->title());
}

inline void KNMusicPlaylistViewer::updateDetailInfo()
{
    //Clear the detail label.
    m_detail->clear();
    //Check the music model of the treeview first.
    if(m_treeView->musicModel()==nullptr)
    {
        //Get back.
        return;
    }
    //Get the playlist model of the tree view.
    KNMusicPlaylistModel *model=
            static_cast<KNMusicPlaylistModel *>(m_treeView->musicModel());
    //Check the music model.
    if(!model)
    {
        //Ignore the request.
        return;
    }
    //Check if is now searching.
    if(m_treeView->proxyModel()->isSearchMode())
    {
        //Get the search result count.
        int searchResult=m_treeView->proxyModel()->rowCount();
        //Set the detail text.
        m_detail->setText(searchResult<2?
                              m_searchCount[searchResult]:
                              m_searchCount[2].arg(
                                QString::number(searchResult)));
        //Get back right now.
        return;
    }
    //Generate a empty text string.
    QString playlistDetail;
    //First check the model row count, this will be used as song count.
    playlistDetail=model->rowCount()<2?
                m_songCount[model->rowCount()]:
                m_songCount[2].arg(QString::number(model->rowCount()));
    //Then calculate the minuates and the hours of the model.
    quint64 minuatePart=model->totalDuration()/60000,
            hourPart=minuatePart/60;
    minuatePart-=hourPart*60;
    //Check whether the hour part is bigger than 0.
    if(hourPart>0)
    {
        //Check the minuates part, if minuate is 0, use the hour without
        //minuate.
        if(minuatePart==0)
        {
            //Use the hour without minuate part.
            playlistDetail.append(hourPart==1?
                                     m_hourCountWithoutMinuate[0]:
                                     m_hourCountWithoutMinuate[1].arg(
                                         QString::number(hourPart)));
        }
        //Add the hour part to the detail.
        playlistDetail.append(hourPart==1?
                                  m_hourCount[0]:
                                  m_hourCount[1].arg(
                                    QString::number(hourPart)));
    }
    //Check the minuate part.
    if(minuatePart==0)
    {
        //If the hour part is 0 as well,
        if(hourPart==0)
        {
            //then append the 0 minuate to the detail.
            playlistDetail.append(m_minuateCount[0]);
        }
    }
    else
    {
        //Add the minuate part to the detail info text string.
        playlistDetail.append(minuatePart<2?
                                  m_minuateCount[minuatePart]:
                                  m_minuateCount[2].arg(
                                      QString::number(minuatePart)));
    }
    //Set the text to the detail label.
    m_detail->setText(playlistDetail);
}
