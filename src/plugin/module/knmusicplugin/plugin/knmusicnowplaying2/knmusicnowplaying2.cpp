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
#include <QApplication>

#include <algorithm>
#include <random>

#include "knglobal.h"
#include "knconfigure.h"

#include "knmusicsingleplaylistmodel.h"
#include "knmusicmodelassist.h"
#include "knmusicproxymodel.h"
#include "knmusictab.h"

#include "knmusicnowplaying2.h"

#include <QDebug>

KNMusicNowPlaying2::KNMusicNowPlaying2(QObject *parent) :
    KNMusicNowPlayingBase(parent)
{
    //Initial the icons.
    m_playingIcon=QPixmap(":/plugin/music/common/playingicon.png");
    m_cantPlayIcon=QPixmap(":/plugin/music/common/cantplay.png");
    //Get the configures.
    m_cacheConfigure=KNGlobal::instance()->cacheConfigure();
    m_musicConfigure=KNMusicGlobal::instance()->musicConfigure();
    //Initial the models.
    initialTemporaryModel();
    initialShadowModel();

    //Link play row request.
    connect(this, &KNMusicNowPlaying2::requirePlayRow,
            this, &KNMusicNowPlaying2::playRow);

    //Link the apply preference request signal and apply preference.
    connect(KNGlobal::instance(), &KNGlobal::requireApplyPreference,
            this, &KNMusicNowPlaying2::applyPreference);
    applyPreference();
    //Link the retranslate request signal and do retranslate.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicNowPlaying2::retranslate);
}

KNMusicNowPlaying2::~KNMusicNowPlaying2()
{

}

void KNMusicNowPlaying2::setBackend(KNMusicBackend *backend)
{
    //Check if we have set a backend before, disconnect the backend finished
    //signal.
    if(m_backend!=nullptr)
    {
        disconnect(m_backend, &KNMusicBackend::finished,
                   this, &KNMusicNowPlaying2::onActionPlayingFinished);
    }
    //Save the backend.
    m_backend=backend;
    //Connect the finished signal.
    connect(m_backend, &KNMusicBackend::finished,
            this, &KNMusicNowPlaying2::onActionPlayingFinished);
    connect(m_backend, &KNMusicBackend::loaded,
            this, &KNMusicNowPlaying2::onActionLoaded);
    connect(m_backend, &KNMusicBackend::cannotLoad,
            this, &KNMusicNowPlaying2::onActionCantLoad);
}

KNMusicProxyModel *KNMusicNowPlaying2::playingModel()
{
    return m_playingModel;
}

KNMusicModel *KNMusicNowPlaying2::playingMusicModel()
{
    return m_playingMusicModel;
}

QPersistentModelIndex KNMusicNowPlaying2::currentPlayingIndex() const
{
    return m_currentPlayingIndex;
}

KNMusicAnalysisItem KNMusicNowPlaying2::currentAnalaysisItem() const
{
    return m_currentPlayingAnalysisItem;
}

int KNMusicNowPlaying2::loopState()
{
    return m_loopMode;
}

void KNMusicNowPlaying2::backupCurrentPlaying()
{
    //Reset the backup position.
    m_backupPosition=-1;
    //First we need to check that we need to backup.
    //!TODO: We need to save the playing state as well.
    if(m_currentPlayingIndex.isValid())
    {
        //Save the position.
        m_backupPosition=m_backend->position();
    }
}

void KNMusicNowPlaying2::restoreCurrentPlaying()
{
    //Check if the backup position is available.
    //If it's not -1, means we should play the current row again, set the
    //position to the backup position.
    if(m_backupPosition==-1)
    {
        //Replay the current row.
        playRow(m_playingModel->mapFromSource(m_currentPlayingIndex).row());
        //Restore the playing position.
        m_backend->setPosition(m_backupPosition);
        //!TODO: do pause if the status is paused.
        //Reset the backup position.
        m_backupPosition=-1;
    }
}

void KNMusicNowPlaying2::resetCurrentPlaying()
{
    //Ask player to clear the information first.
    emit requireResetInformation();
    //Clear the backend.
    m_backend->resetMainPlayer();
    //Clear previous the now playing icon.
    clearNowPlayingIcon();
    //Clear the current index and analysis item.
    m_currentPlayingIndex=QPersistentModelIndex();
    m_currentPlayingAnalysisItem=KNMusicAnalysisItem();
}

void KNMusicNowPlaying2::loadConfigure()
{
    //Restore the configure from cache data.
    //1. Recover the loop state.
    setLoopState(m_cacheConfigure->getData("LoopState",
                                           NoRepeat).toInt());
}

void KNMusicNowPlaying2::saveConfigure()
{
    m_cacheConfigure->setData("LoopState", m_loopMode);
}

void KNMusicNowPlaying2::showCurrentIndexInOriginalTab()
{
    //Abandon the action when the current tab is null.
    if(m_currentTab==nullptr)
    {
        return;
    }
    //Ask the tab to locate the index.
    m_currentTab->showIndexInModel(m_playingMusicModel,
                                   m_currentPlayingIndex);
}

void KNMusicNowPlaying2::shadowPlayingModel()
{
    //Check the playing model is null or not.
    if(m_playingModel==nullptr)
    {
        return;
    }
    //Do deep copy for playing model.
    //Clear the source model, because change the options may change the data.
    m_shadowPlayingModel->setSourceModel(nullptr);
    //Copy the options.
    m_shadowPlayingModel->setFilterRegExp(m_playingModel->filterRegExp());
    m_shadowPlayingModel->setFilterRole(m_playingModel->filterRole());
    m_shadowPlayingModel->setFilterCaseSensitivity(m_playingModel->filterCaseSensitivity());
    m_shadowPlayingModel->setFilterKeyColumn(m_playingModel->filterKeyColumn());
    //Copy the source model.
    m_shadowPlayingModel->setSourceModel(m_playingModel->sourceModel());
    //Check if there's any available sort options, copy the sort options.
    if(m_playingModel->sortColumn()!=-1)
    {
        //Set the sort option.
        m_shadowPlayingModel->setSortCaseSensitivity(m_playingModel->sortCaseSensitivity());
        m_shadowPlayingModel->setSortRole(m_playingModel->sortRole());
        //Emulate the sort.
        m_shadowPlayingModel->sort(m_playingModel->sortColumn(),
                                   m_playingModel->sortOrder());
    }
    //Set the playing model to shadow model.
    m_playingModel=m_shadowPlayingModel;
}

void KNMusicNowPlaying2::playMusicRow(KNMusicProxyModel *model,
                                      int row,
                                      KNMusicTab *tab)
{
    //Clear the now playing icon before changing the model.
    clearNowPlayingIcon();
    //Save the music tab first.
    m_currentTab=tab;
    //Set the playing model if the model is not the same.
    if(model!=m_playingModel)
    {
        //Save the proxy model.
        m_playingModel=model;
        //Update the music model.
        m_playingMusicModel=m_playingModel->musicModel();
    }
    //Reset the current playing index, this can trick the clearNowPlayingIcon()
    //in playRow() function to ignore the reset request.
    m_currentPlayingIndex=QPersistentModelIndex();
    //Set the mannual played switch.
    m_manualPlayed=true;
    //Clear the shuffle list.
    m_shuffleList.clear();
    //Check the loop state, if the state is random state, the shuffle the model.
    if(m_loopMode==Shuffle)
    {
        generateShuffleList();
    }
    //Play the row.
    playRow(row);
}

void KNMusicNowPlaying2::playTemporaryFiles(const QStringList &filePaths)
{
    //Process events.
    qApp->processEvents();
    //Clear the origianl temporary model files.
    m_temporaryMusicModel->clearMusicRow();
    //Set the files to the model.
    m_temporaryMusicModel->setCurrentFiles(filePaths);
    //Process events.
    qApp->processEvents();
    //Check if there's any file we can play, according to the rowCount().
    if(m_temporaryMusicModel->rowCount()>0)
    {
        //Play the temporary model.
        playMusicRow(m_temporaryModel, 0);
    }
}

void KNMusicNowPlaying2::playNext()
{
    //Check the current playing index is available or not.
    if(m_playingModel==nullptr || m_playingModel->rowCount()==0)
    {
        return;
    }
    if(!m_currentPlayingIndex.isValid() ||
            m_currentPlayingIndex.model()!=m_playingModel->sourceModel())
    {
        emit requirePlayRow(0);
        return;
    }
    //Check the loop state.
    if(Shuffle==m_loopMode)
    {
        int nextProxyRow=nextShuffleProxyRow();
        //Check is the row available.
        if(nextProxyRow==-1)
        {
            //Clear the current playing.
            resetCurrentPlaying();
        }
        //Play the shuffle row.
        emit requirePlayRow(nextProxyRow);
        return;
    }
    //Get the next row.
    int nextProxyRow=nextRow(m_playingModel->mapFromSource(m_currentPlayingIndex).row(),
                             false);
    //Check if the row is available.
    if(nextProxyRow==-1)
    {
        //Clear the current playing.
        resetCurrentPlaying();
        return;
    }
    //Play this row.
    emit requirePlayRow(nextProxyRow);
}

void KNMusicNowPlaying2::playPrevious()
{
    //Check the current playing index is available or not.
    if(m_playingModel==nullptr || m_playingModel->rowCount()==0)
    {
        return;
    }
    if(!m_currentPlayingIndex.isValid() ||
            m_currentPlayingIndex.model()!=m_playingModel->sourceModel())
    {
        emit requirePlayRow(m_playingModel->rowCount()-1);
        return;
    }
    //Check the loop state.
    if(Shuffle==m_loopMode)
    {
        int prevProxyRow=prevShuffleProxyRow();
        //Check is the row available.
        if(prevProxyRow==-1)
        {
            //Clear the current playing.
            resetCurrentPlaying();
        }
        //Play the shuffle row.
        emit requirePlayRow(prevProxyRow);
        return;
    }
    //Get the previous row.
    int prevProxyRow=prevRow(m_playingModel->mapFromSource(m_currentPlayingIndex).row(),
                             false);
    //Check if the row is available.
    if(prevProxyRow==-1)
    {
        //Clear the current playing.
        resetCurrentPlaying();
        return;
    }
    //Play this row.
    emit requirePlayRow(prevProxyRow);
}

void KNMusicNowPlaying2::changeLoopState()
{
    //Switch to the next loop mode.
    setLoopState(m_loopMode+1);
}

void KNMusicNowPlaying2::setLoopState(const int &state)
{
    //Save the new state.
    m_loopMode=state % LoopCount;
    //Emit the loop mode changed signal.
    emit loopStateChanged(m_loopMode);
    //Check if the loop state is shuffle, generate the shuffle list.
    if(Shuffle==m_loopMode)
    {
        //Generate the shuffle list for the current model.
        generateShuffleList();
    }
}

void KNMusicNowPlaying2::setCurrentSongRating(const int &rating)
{
    //Set the rating number to the row text.
    m_playingMusicModel->setItemText(m_currentPlayingIndex.row(),
                                     Rating,
                                     QString::number(rating));
}

void KNMusicNowPlaying2::checkRemovedModel(KNMusicModel *model)
{
    //When a music model is going to be removed, check the model is being played
    //or not.
    if(model==m_playingMusicModel)
    {
        //We need to reset the current playing information and current playing
        //models.
        //Reset current playing first.
        resetCurrentPlaying();
        //Reset the playing models.
        m_playingModel=nullptr;
        m_playingMusicModel=nullptr;
        clearShadowModel();
        //Reset the music tab pointer.
        m_currentTab=nullptr;
        //Clear the shuffle list.
        m_shuffleList.clear();
    }
}

void KNMusicNowPlaying2::onActionPlayingFinished()
{
    //Add current row play times.
    if(m_playingModel!=nullptr && m_currentPlayingIndex.isValid())
    {
        m_playingModel->addPlayTimes(m_currentPlayingIndex);
    }
    //Set the manual played flag to false.
    m_manualPlayed=false;
    //If current mode is repeat the current track, just play it again :)
    if(m_loopMode==RepeatTrack)
    {
        m_backend->play();
        return;
    }
    //Or else, play the next row, with the repeat flag.
    playNext();
}

void KNMusicNowPlaying2::onActionCantLoad()
{
    int row=m_currentPlayingIndex.row();
    //Set the cannot play flag and icon to the row.
    m_playingMusicModel->setRowProperty(row,
                                        CantPlayFlagRole,
                                        true);
    m_playingMusicModel->setRoleData(row,
                                     BlankData,
                                     Qt::DecorationRole,
                                     m_cantPlayIcon);
    //Check if the user play flag is off, then automatically play the next song,
    //Or else we stops here, let user to do the next thing.
    if(m_manualPlayed)
    {
        return;
    }
    //Ask to play the next row.
    //!FIXME: Let these codes works together with the code in play next.
    //Check the current playing index is available or not.
    if(m_playingModel==nullptr || m_playingModel->rowCount()==0)
    {
        return;
    }
    if(!m_currentPlayingIndex.isValid() ||
            m_currentPlayingIndex.model()!=m_playingModel->sourceModel())
    {
        emit requirePlayRow(0);
        return;
    }
    //Get the next row.
    int nextProxyRow=nextRow(m_playingModel->mapFromSource(m_currentPlayingIndex).row(),
                             true);
    //Check if the row is available.
    if(nextProxyRow==-1)
    {
        //Clear the current playing.
        resetCurrentPlaying();
        return;
    }
    //Play this row.
    emit requirePlayRow(nextProxyRow);
}

void KNMusicNowPlaying2::onActionLoaded()
{
    //Force clear the cannot play flag and set the playing icon.
    m_playingMusicModel->setRowProperty(m_currentPlayingIndex.row(),
                                        CantPlayFlagRole,
                                        false);
    //Give out the update signal.
    emit nowPlayingChanged(m_currentPlayingAnalysisItem);
}

void KNMusicNowPlaying2::retranslate()
{
    ;
}

void KNMusicNowPlaying2::applyPreference()
{
    ;
}

inline void KNMusicNowPlaying2::initialTemporaryModel()
{
    //Initial the temporary music model.
    m_temporaryMusicModel=new KNMusicSinglePlaylistModel(this);
    //Initial the proxy model for temporary music model.
    m_temporaryModel=new KNMusicProxyModel(this);
    m_temporaryModel->setSourceModel(m_temporaryMusicModel);
}

void KNMusicNowPlaying2::initialShadowModel()
{
    //Initial the shadow playing model.
    m_shadowPlayingModel=new KNMusicProxyModel(this);
}

void KNMusicNowPlaying2::clearNowPlayingIcon()
{
    //First we need to check the previous index is available or not.
    if(m_currentPlayingIndex.isValid())
    {
        //Check if the row cannot be played, then the decorate role should
        //contains icon.
        if(!m_playingMusicModel->rowProperty(m_currentPlayingIndex.row(),
                                             CantPlayFlagRole).toBool())
        {
            //Clear the decorate role.
            m_playingMusicModel->setRoleData(m_currentPlayingIndex.row(),
                                             BlankData,
                                             Qt::DecorationRole,
                                             QPixmap());
        }
    }
}

void KNMusicNowPlaying2::clearShadowModel()
{
    //Clear the shadow playing model data.
    m_shadowPlayingModel->setSourceModel(nullptr);
    m_shadowPlayingModel->setSortRole(-1);
    m_shadowPlayingModel->setFilterFixedString("");
    m_shadowPlayingModel->setFilterRole(-1);
}

void KNMusicNowPlaying2::generateShuffleList()
{
    //First, check is the model available.
    //Second, check is the shuffle list empty, if there's data in the list,
    //ignore the request.
    if(m_playingMusicModel==nullptr ||
            !m_shuffleList.isEmpty())
    {
        return;
    }
    //Clear the shuffle list first.
    m_shuffleList.clear();
    //Get all the model index.
    for(int i=0, modelRows=m_playingMusicModel->rowCount(); i<modelRows; i++)
    {
        m_shuffleList.append(
                    QPersistentModelIndex(
                        m_playingMusicModel->index(i,
                                                   m_playingMusicModel->playingItemColumn())));
    }
    //Shuffle the list.
    std::random_device randomDevice;
    std::mt19937_64 randomGenerator(randomDevice());
    std::shuffle(m_shuffleList.begin(), m_shuffleList.end(), randomGenerator);
}

inline int KNMusicNowPlaying2::prevShuffleProxyRow()
{
    //Ensure the shuffle list is not empty.
    if(m_shuffleList.isEmpty())
    {
        return -1;
    }
    //Now the shuffle list cannot be a empty list.
    //Find the current playing index in the shuffle list from the last.
    int playingIndex=m_shuffleList.indexOf(m_currentPlayingIndex);
    if(playingIndex==-1)
    {
        //Find the first available index.
        while(!m_shuffleList.isEmpty())
        {
            //If the first one is available, return the first one.
            if(m_shuffleList.last().isValid())
            {
                return m_playingModel->mapFromSource(m_shuffleList.last()).row();
            }
            //Remove the invaild index.
            m_shuffleList.removeLast();
        }
        return -1;
    }
    //When we come here, it means:
    //1. The shuffle list is not empty.
    //2. We find the now playing index in the shuffle list.
    //We need to find a vaild index in the previous row.
    for(int i=playingIndex-1; i>-1; i--)
    {
        //If we find a vaild one, then return the index.
        if(m_shuffleList.at(i).isValid())
        {
            return m_playingModel->mapFromSource(m_shuffleList.at(i)).row();
        }
        //Or else, remove the invaild index.
        m_shuffleList.removeAt(i);
    }
    //When we comes here, means all the index before the current index is
    //invaild, find the index from the last one.
    //Find the first available index.
    while(!m_shuffleList.isEmpty())
    {
        //If the first one is available, return the first one.
        if(m_shuffleList.last().isValid())
        {
            return m_playingModel->mapFromSource(m_shuffleList.last()).row();
        }
        //Remove the invaild index.
        m_shuffleList.removeLast();
    }
    //It should never comes here, because there's at least one available index
    //in the list.
    return -1;
}

int KNMusicNowPlaying2::nextShuffleProxyRow()
{
    //Ensure the shuffle list is not empty.
    if(m_shuffleList.isEmpty())
    {
        return -1;
    }
    //Now the shuffle list cannot be a empty list.
    //Find the current playing index from the first one in the shuffle list.
    int playingIndex=m_shuffleList.indexOf(m_currentPlayingIndex);
    if(playingIndex==-1)
    {
        //Find the first available index.
        while(!m_shuffleList.isEmpty())
        {
            //If the first one is available, return the first one.
            if(m_shuffleList.first().isValid())
            {
                return m_playingModel->mapFromSource(m_shuffleList.first()).row();
            }
            //Remove the invaild index.
            m_shuffleList.removeFirst();
        }
        return -1;
    }
    //When we come here, it means:
    //1. The shuffle list is not empty.
    //2. We find the now playing index in the shuffle list.
    //We need to find a vaild index in the next rows.
    int nextRow=playingIndex+1;
    while(nextRow<m_shuffleList.size())
    {
        //If we find a vaild one, then return the index.
        if(m_shuffleList.at(nextRow).isValid())
        {
            return m_playingModel->mapFromSource(m_shuffleList.at(nextRow)).row();
        }
        //Or else, remove the invaild index.
        m_shuffleList.removeAt(nextRow);
    }
    //When we comes here, means all the index after the current index is
    //invaild, find the index from the first one.
    //Find the first available index.
    while(!m_shuffleList.isEmpty())
    {
        //If the first one is available, return the first one.
        if(m_shuffleList.first().isValid())
        {
            return m_playingModel->mapFromSource(m_shuffleList.first()).row();
        }
        //Remove the invaild index.
        m_shuffleList.removeFirst();
    }
    //It should never comes here, because there's at least one available index
    //in the list.
    return -1;
}

int KNMusicNowPlaying2::nextRow(int currentProxyRow, bool ignoreLoopMode)
{
    //If the row is the last row in the model.
    if(currentProxyRow==m_playingModel->rowCount()-1)
    {
        //Check the ignore loop mode flag.
        if(ignoreLoopMode)
        {
            //Reach the end of the model.
            return -1;
        }
        switch(m_loopMode)
        {
        case NoRepeat:
        case RepeatTrack:
            //Reach the end of the model.
            return -1;
            break;
        case RepeatAll:
            //Back to the first row.
            return 0;
        }
    }
    //Normal case: return the next row.
    return currentProxyRow+1;
}

int KNMusicNowPlaying2::prevRow(int currentProxyRow, bool ignoreLoopMode)
{
    //If the row is the first row in the model.
    if(currentProxyRow==0)
    {
        //Check the ignore loop mode flag.
        if(ignoreLoopMode)
        {
            //Reach the begin of the model.
            return -1;
        }
        switch(m_loopMode)
        {
        case NoRepeat:
        case RepeatTrack:
            //Reach the begin of the model.
            return -1;
        case RepeatAll:
            //Play the last one.
            return m_playingModel->rowCount()-1;
        }
    }
    //Normal case: return the previous row.
    return currentProxyRow-1;
}

inline void KNMusicNowPlaying2::playRow(const int &proxyRow)
{
    Q_ASSERT(m_playingModel!=nullptr &&
            proxyRow>-1 &&
            proxyRow<m_playingModel->rowCount());
    //Remove the previous playing icon no matter what happend.
    clearNowPlayingIcon();
    //Get the source index, and save the source index as a persistent index.
    m_currentPlayingIndex=QPersistentModelIndex(m_playingModel->mapToSource(
                                                    m_playingModel->index(proxyRow,
                                                                          m_playingModel->playingItemColumn())));
    //Set the playing icon.
    m_playingMusicModel->setRoleData(m_currentPlayingIndex.row(),
                                     BlankData,
                                     Qt::DecorationRole,
                                     m_playingIcon);
    //First we need to reanalysis that row, if we cannot analysis that row,
    //means we cannot .
    KNMusicAnalysisItem currentAnalysisItem;
    if(KNMusicModelAssist::reanalysisRow(m_playingMusicModel,
                                         m_currentPlayingIndex,
                                         currentAnalysisItem))
    {
        //Process events.
        qApp->processEvents();
        //Save the current analsys item.
        m_currentPlayingAnalysisItem=currentAnalysisItem;
        //Get the detail info.
        KNMusicDetailInfo &currentInfo=m_currentPlayingAnalysisItem.detailInfo;
        //Update the music model row.
        m_playingMusicModel->updateMusicRow(m_currentPlayingIndex.row(),
                                            currentAnalysisItem);
        //Play the music, according to the detail information.
        //This is a much better judge than the original version.
        if(currentInfo.trackFilePath.isEmpty())
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
