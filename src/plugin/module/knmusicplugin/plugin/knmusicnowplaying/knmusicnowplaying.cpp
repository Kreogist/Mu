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
#include "knmusicsingleplaylistmodel.h"
#include "knmusicmodelassist.h"
#include "knmusicproxymodel.h"
#include "knmusicglobal.h"

#include "knmusicnowplaying.h"

#include <QDebug>

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicNowPlayingBase(parent)
{
    //Initial the icon.
    m_playingIcon=QPixmap(":/plugin/music/common/playingicon.png");
    m_cantPlayIcon=QPixmap(":/plugin/music/common/cantplay.png");
    //Initial temporary model.
    m_temporaryModel=new KNMusicSinglePlaylistModel(this);
    //Initial shadow proxy model.
    m_shadowPlayingModel=new KNMusicProxyModel(this);
    //Generate temporary proxy model.
    m_temporaryProxyModel=new KNMusicProxyModel(this);
    m_temporaryProxyModel->setSourceModel(m_temporaryModel);
}

KNMusicNowPlaying::~KNMusicNowPlaying()
{
    saveConfigure();
}

void KNMusicNowPlaying::setBackend(KNMusicBackend *backend)
{
    if(m_backend==nullptr)
    {
        //Save the backend.
        m_backend=backend;
        //Connect request.
        connect(m_backend, &KNMusicBackend::finished,
                this, &KNMusicNowPlaying::onActionPlayingFinished);
        connect(m_backend, &KNMusicBackend::cannotLoadFile,
                this, &KNMusicNowPlaying::onActionCannotPlay);
    }
}

KNMusicProxyModel *KNMusicNowPlaying::playingModel()
{
    return m_playingModel;
}

KNMusicModel *KNMusicNowPlaying::playingMusicModel()
{
    return m_playingMusicModel;
}

int KNMusicNowPlaying::loopState()
{
    return m_loopMode;
}

void KNMusicNowPlaying::restoreConfigure()
{
    setLoopState(KNMusicGlobal::instance()->configureData("LoopState",
                                                          NoRepeat).toInt());
}

void KNMusicNowPlaying::playNext()
{
    playNextSong();
}

void KNMusicNowPlaying::playPrevious()
{
    //If there's no model or the source model is not the music model,
    //reset all the model.
    if(m_playingModel==nullptr)
    {
        resetPlayingItem();
        resetPlayingModels();
        return;
    }
    //If current playing index is invaild, then try to play the first item.
    if(!m_currentPlayingIndex.isValid())
    {
        if(m_playingModel->rowCount()>0)
        {
            playMusic(m_playingModel->rowCount()-1);
        }
        return;
    }
    QModelIndex currentIndex=m_playingModel->mapFromSource(m_currentPlayingIndex);
    //If it's the first one
    if(currentIndex.row()==0)
    {
        switch(m_loopMode)
        {
        case NoRepeat:
        case RepeatTrack:
            //Finished playing.
            resetPlayingItem();
            return;
        case RepeatAll:
            //Play the last one.
            playMusic(m_playingModel->rowCount()-1);
            return;
        }
    }
    //Play the previous song.
    playMusic(currentIndex.row()-1);
}

void KNMusicNowPlaying::onActionPlayingFinished()
{
    //Add play times.
    if(m_playingModel!=nullptr && m_currentPlayingIndex.isValid())
    {
        m_playingModel->addPlayTimes(m_currentPlayingIndex);
    }
    //If current mode is track repeat, just play it again. :)
    if(m_loopMode==RepeatTrack)
    {
        m_backend->play();
        return;
    }
    //Or else, we need to play the next.
    playNext();
}

void KNMusicNowPlaying::changeLoopState()
{
    //Switch to the next loop mode.
    setLoopState(m_loopMode+1);
}

void KNMusicNowPlaying::setLoopState(const int &state)
{
    //Set to the state.
    m_loopMode=state%LoopCount;
    //Emit the loop changed signal.
    emit loopStateChanged(m_loopMode);
}

void KNMusicNowPlaying::playTemporaryFiles(const QStringList &filePaths)
{
    //Remove all the old rows.
    m_temporaryModel->clearMusicRow();
    //Set the tempoary model to these files.
    m_temporaryModel->setCurrentFiles(filePaths);
    //Check is there available music in the temporary model.
    //If there's any file we can play, play it.
    if(m_temporaryModel->rowCount()>0)
    {
        //Set the playing model to temporary model.
        setPlayingModel(m_temporaryProxyModel);
        //Play the first file.
        playMusic(0);
    }
}

void KNMusicNowPlaying::setPlayingModel(KNMusicProxyModel *model)
{
    //Reset current item and music model.
    resetPlayingItem();
    resetPlayingModels();
    //Save the proxy model and music model.
    m_playingModel=model;
    if(m_playingModel!=nullptr)
    {
        m_playingMusicModel=m_playingModel->musicModel();
    }
}

void KNMusicNowPlaying::playMusic(const int &row)
{
    Q_ASSERT(m_playingModel!=nullptr &&
            row>-1 &&
            row<m_playingModel->rowCount());
    //Clear the current item.
    resetPlayingItem();
    //Get the model index in the proxy model.
    QModelIndex sourceIndex=
            m_playingModel->mapToSource(
                m_playingModel->index(row,
                                      m_playingModel->playingItemColumn()));
    //Get the source standard item. Storage the item.
    //Because the index of the current item might be changing, store the item
    //pointer can solve this problem.
    //Oct 18th, 2014: Actually there's a better way to solve this problem, and
    //                we don't need any pointer, that's QPersistentModelIndex,
    //                Check it ASAP.
    m_currentPlayingIndex=QPersistentModelIndex(sourceIndex);
    //Set the current playing icon.
    m_playingMusicModel->setRoleData(m_currentPlayingIndex.row(),
                                     BlankData,
                                     Qt::DecorationRole,
                                     m_playingIcon);
    KNMusicDetailInfo currentInfo;
    //Parse the current index, if we cannot parse it, play next.
    if(KNMusicModelAssist::reanalysisRow(m_playingMusicModel,
                                         m_currentPlayingIndex,
                                         currentInfo))
    {
        //Update the player's data.
        emit requireUpdatePlayerInfo(currentInfo);
        //Update the data in proxy model.
        m_playingMusicModel->updateMusicRow(m_currentPlayingIndex.row(),
                                            currentInfo);
        //Ask backend to play the file.
        if(currentInfo.startPosition==-1)
        {
            m_backend->playFile(currentInfo.filePath);
        }
        else
        {
            m_backend->playSection(currentInfo.filePath,
                                   currentInfo.startPosition,
                                   currentInfo.duration);
        }
    }
}

void KNMusicNowPlaying::playMusic(const QModelIndex &index)
{
    playMusic(index.row());
}

void KNMusicNowPlaying::checkRemovedModel(KNMusicModel *model)
{
    //When a music model is removing, we need to check whether the music model
    //is the current playing one.
    if(model==m_playingMusicModel)
    {
        //If sure, reset everything first.
        resetPlayingItem();
        resetPlayingModels();
    }
}

void KNMusicNowPlaying::saveConfigure()
{
    KNMusicGlobal::instance()->setConfigureData("LoopState", m_loopMode);
}

void KNMusicNowPlaying::onActionCannotPlay()
{
    int cannotPlayRow=m_currentPlayingIndex.row();
    //Play the next song with cannot play mode.
    playNextSong(true);
    //Set cannot play icon.
    m_playingMusicModel->setRoleData(cannotPlayRow,
                                     BlankData,
                                     Qt::DecorationRole,
                                     m_cantPlayIcon);
}

void KNMusicNowPlaying::playNextSong(bool cannotLoadFile)
{
    //If there's no model or the source model is not the music model,
    //reset all the model.
    if(m_playingModel==nullptr)
    {
        resetPlayingItem();
        resetPlayingModels();
        return;
    }
    //If current playing index is invaild, then try to play the first item.
    if(!m_currentPlayingIndex.isValid())
    {
        if(m_playingModel->rowCount()>0)
        {
            playMusic(0);
        }
        return;
    }
    QModelIndex currentIndex=m_playingModel->mapFromSource(m_currentPlayingIndex);
    //If it's the last one.
    if(currentIndex.row()==m_playingModel->rowCount()-1)
    {
        if(cannotLoadFile)
        {
            //Finished playing.
            resetPlayingItem();
            return;
        }
        switch(m_loopMode)
        {
        case NoRepeat:
        case RepeatTrack:
            //Finished playing.
            resetPlayingItem();
            return;
        case RepeatAll:
            //Play the first one.
            playMusic(0);
            return;
        }
    }
    //Play the next song.
    playMusic(currentIndex.row()+1);
}

void KNMusicNowPlaying::resetPlayingItem()
{
    //No matter what, reset header player first.
    emit requireResetPlayer();
    //Check is the current item null, if not, clear the playing icon.
    if(m_currentPlayingIndex.isValid())
    {
        //Though this should never happend, only for safe.
        if(m_playingMusicModel!=nullptr)
        {
            m_playingMusicModel->setData(m_currentPlayingIndex,
                                         QPixmap(),
                                         Qt::DecorationRole);
        }
        m_currentPlayingIndex=QPersistentModelIndex();
    }
}

void KNMusicNowPlaying::resetPlayingModels()
{
    //Clear the playing model.
    m_playingModel=nullptr;
    //Clear the shadow playing model data.
    m_shadowPlayingModel->setSourceModel(nullptr);
    m_shadowPlayingModel->setSortRole(-1);
    m_shadowPlayingModel->setFilterFixedString("");
    m_shadowPlayingModel->setFilterRole(-1);
    //Clear music model.
    m_playingMusicModel=nullptr;
}

QPersistentModelIndex KNMusicNowPlaying::currentPlayingIndex() const
{
    return m_currentPlayingIndex;
}

void KNMusicNowPlaying::shadowPlayingModel()
{
    //Do deep copy for play model.
    if(m_playingModel!=nullptr)
    {
        //--Copy the filter options.
        m_shadowPlayingModel->setFilterRegExp(m_playingModel->filterRegExp());
        m_shadowPlayingModel->setFilterRole(m_playingModel->filterRole());
        m_shadowPlayingModel->setFilterCaseSensitivity(m_playingModel->filterCaseSensitivity());
        m_shadowPlayingModel->setFilterKeyColumn(m_playingModel->filterKeyColumn());
        //--Copy the source model.
        m_shadowPlayingModel->setSourceModel(m_playingModel->sourceModel());
        //--Copy the sort options.
        if(m_playingModel->sortColumn()!=-1)
        {
            m_shadowPlayingModel->setSortCaseSensitivity(m_playingModel->sortCaseSensitivity());
            m_shadowPlayingModel->setSortRole(m_playingModel->sortRole());
            m_shadowPlayingModel->sort(m_playingModel->sortColumn(),
                                       m_playingModel->sortOrder());
        }
    }
    //Check if is the playing index available.
    if(m_currentPlayingIndex.isValid())
    {
        //Switch to shadow playing index.
        QModelIndex proxyIndex=m_playingModel->mapFromSource(m_currentPlayingIndex),
                shadowPorxyIndex=m_shadowPlayingModel->index(proxyIndex.row(),
                                                             proxyIndex.column());
        m_currentPlayingIndex=
                QPersistentModelIndex(m_shadowPlayingModel->mapToSource(shadowPorxyIndex));
    }
    //Set the playing model to shadow model.
    m_playingModel=m_shadowPlayingModel;
    if(m_playingModel!=nullptr)
    {
        m_playingMusicModel=m_playingModel->musicModel();
    }
}

void KNMusicNowPlaying::resetCurrentPlaying()
{
    resetPlayingItem();
}
