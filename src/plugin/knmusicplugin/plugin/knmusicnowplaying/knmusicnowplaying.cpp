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
#include "knmusicproxymodel.h"
#include "knmusicmodel.h"
#include "knmusicbackend.h"
#include "knmusicglobal.h"
#include "knmusicparser.h"
#include "knmusictemporaryplaylistmodel.h"

#include "knmusicnowplaying.h"

#include <QDebug>

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicNowPlayingBase(parent),
    m_backend(nullptr),
    m_playingProxyModel(nullptr),
    m_shadowPlayingModel(new KNMusicProxyModel(this)),
    m_temporaryProxyPlaylist(new KNMusicProxyModel(this)),
    m_temporaryPlaylist(new KNMusicTemporaryPlaylistModel(this)),
    m_playingTab(nullptr),
    m_loopState(NoRepeat),
    m_playingIndex(QPersistentModelIndex()),
    m_playingAnalysisItem(KNMusicAnalysisItem()),
    m_manualPlayed(false)
{
    //Set the temporary playlist to the proxy model.
    m_temporaryProxyPlaylist->setSourceModel(m_temporaryPlaylist);
    //Initial the random device and random generator.
    QVector<int> seedData;
    //Generate the random device.
    std::random_device randomDevice;
    //Get the seed array size.
    const int seedCount=(int)std::mt19937::state_size;
    //Resize the seed data.
    seedData.resize(seedCount);
    //Set the data to the seed data.
    for(int i=0; i<seedCount; ++i)
    {
        //Replace the seed data.
        seedData.replace(i, randomDevice());
    }
    //Transform list to seed sequence.
    std::seed_seq seedSequence(std::begin(seedData), std::end(seedData));
    //Initial the Mersenne twister seeds.
    m_mersenneSeed=std::mt19937(seedSequence);
}

void KNMusicNowPlaying::setBackend(KNMusicBackend *backend)
{
    //Disconnect all the previous backend connections.
    m_backendConnections.disconnectAll();
    //Save the new backend pointer.
    m_backend=backend;
    //Check out the backend. If the backend is not nullptr, then link the
    //backend with the now playing.
    if(m_backend!=nullptr)
    {
        //Link the finished signal and slot.
        m_backendConnections.append(
                    connect(m_backend, &KNMusicBackend::finished,
                            this, &KNMusicNowPlaying::onActionBackendFinished));
        //Link the load success and failed to load signal.
        m_backendConnections.append(
                    connect(m_backend, &KNMusicBackend::loadFailed,
                            this, &KNMusicNowPlaying::onActionLoadFailed));
        m_backendConnections.append(
                    connect(m_backend, &KNMusicBackend::loadSuccess,
                            this, &KNMusicNowPlaying::onActionLoadSuccess));
    }
}

KNMusicProxyModel *KNMusicNowPlaying::playingModel()
{
    return m_playingProxyModel;
}

KNMusicModel *KNMusicNowPlaying::playingMusicModel()
{
    //This is only a hacking way to get the music model.
    return m_playingProxyModel==nullptr?
                nullptr:
                m_playingProxyModel->musicModel();
}

int KNMusicNowPlaying::loopState()
{
    return m_loopState;
}

QPersistentModelIndex KNMusicNowPlaying::playingIndex()
{
    return m_playingIndex;
}

KNMusicAnalysisItem KNMusicNowPlaying::playingItem()
{
    return m_playingAnalysisItem;
}

KNMusicTab *KNMusicNowPlaying::playingTab()
{
    return m_playingTab;
}

void KNMusicNowPlaying::reset()
{
    //Reset current playing.
    resetCurrentPlaying();
    //Reset the shadow playing model.
    resetShadowModel();
    //Reset the playing models.
    m_playingProxyModel=nullptr;
    //Emit the proxy model changed signal.
    emit nowPlayingModelChanged(m_playingProxyModel);
}

void KNMusicNowPlaying::loadConfigure()
{
    //Get the loop state from the cache data.
}

void KNMusicNowPlaying::saveConfigure()
{
    ;
}

void KNMusicNowPlaying::shadowPlayingModel()
{
    //Check if the playing model is nullptr, ignore the invalid request.
    if(m_playingProxyModel==nullptr)
    {
        return;
    }
    //Clear the source model of the shadow model. Because that may caused
    //useless searching or filitering.
    m_shadowPlayingModel->setSourceModel(nullptr);
    //Do deep copy for the proxy playing model.
    m_shadowPlayingModel->setSearchBlocks(m_playingProxyModel->searchBlocks());
    //Copy the category settings.
    m_shadowPlayingModel->setCategoryColumn(
                m_playingProxyModel->categoryColumn());
    m_shadowPlayingModel->setCategoryContent(
                m_playingProxyModel->categoryContent());
    //Copy the source model.
    m_shadowPlayingModel->setSourceModel(m_playingProxyModel->sourceModel());
    //Check if there's any available sort options, copy the sort options.
    if(m_playingProxyModel->sortColumn()!=-1)
    {
        //Set the sort column.
        m_shadowPlayingModel->setSortCaseSensitivity(
                    m_playingProxyModel->sortCaseSensitivity());
        m_shadowPlayingModel->setSortRole(m_playingProxyModel->sortRole());
        //Emulate the sort of the original proxy model.
        m_shadowPlayingModel->sort(m_playingProxyModel->sortColumn(),
                                   m_playingProxyModel->sortOrder());
    }
    //Set the current playing proxy model to shadow model.
    m_playingProxyModel=m_shadowPlayingModel;
    //Emit the proxy model changed signal.
    emit nowPlayingModelChanged(m_playingProxyModel);
}

void KNMusicNowPlaying::playMusicRow(KNMusicProxyModel *model,
                                     int row,
                                     KNMusicTab *tab)
{
    //Check the model and chech the music model of the model.
    //Clear whether the previous model is the current model.
    if(m_playingProxyModel!=model)
    {
        //Check the previous model null.
        if(playingMusicModel()!=nullptr)
        {
            //Disconnect all the monitor signals.
            m_musicModelConnections.disconnectAll();
            //Reset the previous proxy odel.
            playingMusicModel()->setPlayingIndex(QModelIndex());
        }
        //Save the new proxy model.
        m_playingProxyModel=model;
        //Emit the proxy model changed signal.
        emit nowPlayingModelChanged(m_playingProxyModel);
        //Save the music tab.
        m_playingTab=tab;
        //Check the music model first, we can only connect when its not null.
        if(playingMusicModel())
        {
            //Connect the monitor signals.
            m_musicModelConnections.append(
                    connect(playingMusicModel(),
                            &KNMusicModel::playingItemRemoved,
                            this,
                            &KNMusicNowPlaying::onActionPlayingItemRemoved));
            m_musicModelConnections.append(
                    connect(playingMusicModel(),
                            &KNMusicModel::dataChanged,
                            this,
                            &KNMusicNowPlaying::onActionModelDataChanged));
        }
    }
    //Reset the current playing index to be empty.
    m_playingIndex=QPersistentModelIndex();
    //Marked last played action is done manually, this parameter will be used in
    //playNext function for checking.
    m_manualPlayed=true;
    //Play the row.
    playRow(row);
}

void KNMusicNowPlaying::playUrl(const QUrl &url)
{
    ;
}

void KNMusicNowPlaying::playNext()
{
    //Play the next music with the repeat mode.
    playNextRow(false);
}

void KNMusicNowPlaying::playPrevious()
{
    //Check the current playing index is avaliable or not.
    if(m_playingProxyModel==nullptr ||
            m_playingProxyModel->rowCount()==0)
    {
        return;
    }
    //Like the playNext(), here we are only going to play the last one.
    if(!m_playingIndex.isValid() ||
            m_playingIndex.model()!=m_playingProxyModel->sourceModel())
    {
        //Play the first row.
        playRow(m_playingProxyModel->rowCount() - 1);
        //Finished.
        return;
    }
    //Check the loop state, if it's shuffle mode, use a specfic way to get the
    //index.
    if(Shuffle==m_loopState)
    {
        //Play the shuffle data.
        //Generate the shuffle distribution.
        std::uniform_int_distribution<> gapDistribution(
                    0,
                    m_playingProxyModel->rowCount()-1);
        //Get the row index.
        int preferRowGap=gapDistribution(m_mersenneSeed);
        //Check out the prefer gap.
        if(preferRowGap==0)
        {
            //Update the prefer gap.
            preferRowGap=gapDistribution(m_mersenneSeed);
        }
        //Calculate the prefer row, reuse the prefer row gap.
        preferRowGap=m_playingProxyModel->mapFromSource(m_playingIndex).row()-
                preferRowGap;
        //Make sure the number is valid.
        if(preferRowGap<0)
        {
            //To fix it, it's simple, add row count.
            preferRowGap+=(m_playingProxyModel->rowCount()-1);
        }
        //Play the row.
        playRow(preferRowGap);
        //Check out the prefer row is empty or not.
        return;
    }
    //Calculate the previous row.
    int previousProxyRow=
            m_playingProxyModel->mapFromSource(m_playingIndex).row();
    //If the row is the first row in the model,
    if(previousProxyRow==0)
    {
        //According to the loop mode, give out the row.
        //Only Repeat All can go back from the last line of the proxy model.
        previousProxyRow=
                (m_loopState==RepeatAll)?m_playingProxyModel->rowCount()-1:-1;
    }
    else
    {
        //Normal case: the previous row of the current row.
        --previousProxyRow;
    }

    //Check if the row is available.
    if(previousProxyRow==-1)
    {
        //Clear the current playing.
        resetCurrentPlaying();
        //Everything is done.
        return;
    }
    //Play the new row.
    playRow(previousProxyRow);
}

void KNMusicNowPlaying::playTemporaryFiles(const QStringList &filePaths)
{
    //Save the previous row count of the files.
    int previousFileCount=m_temporaryPlaylist->rowCount();
    //Add files to playing model.
    m_temporaryPlaylist->appendTemporaryFiles(filePaths);
    //Check whether there's new file loaded in to the playlist model.
    if(m_temporaryPlaylist->rowCount() > previousFileCount)
    {
        //New files have been loaded to the tempoary play list.
        //So now, the new file should be placed start at previousFileCount.
        playMusicRow(m_temporaryProxyPlaylist,
                     previousFileCount,
                     nullptr);
    }
}

void KNMusicNowPlaying::changeLoopState()
{
    //Simply switch to the next mode.
    setLoopState(m_loopState+1);
}

void KNMusicNowPlaying::setLoopState(const int &state)
{
    //Save the new state.
    m_loopState=state % LoopCount;
    //Emit the loop state changed signal.
    emit loopStateChanged(m_loopState);
}

void KNMusicNowPlaying::setPlayingRating(int rating)
{
    //Check the playing model.
    if(!m_playingIndex.isValid())
    {
        //Ignore the playing index.
        return;
    }
    //Update the detail info.
    if(playingMusicModel())
    {
        playingMusicModel()->setTextData(m_playingIndex.row(),
                                         Rating,
                                         QString::number(rating));
    }
}

void KNMusicNowPlaying::onActionModelRemoved(KNMusicModel *model)
{
    //When a music model is going to be removed, check the model is being played
    //or not.
    if(model==playingMusicModel())
    {
        //We need to reset the current playing information and current playing
        //models.
        //Reset current playing first.
        reset();
        //Reset the playing models.
        m_playingProxyModel=nullptr;
        //Emit the proxy model changed signal.
        emit nowPlayingModelChanged(m_playingProxyModel);
        //Reset the shadow proxy model.
        resetShadowModel();
        //Reset the music tab pointer.
        m_playingTab=nullptr;
    }
}

void KNMusicNowPlaying::onActionBackendFinished()
{
    //Add play times on current row.
    if(playingMusicModel() && m_playingIndex.isValid())
    {
        //Add play times on the playing index.
        playingMusicModel()->addPlayingTimes(m_playingIndex);
    }
    //Set the manual playing flag to become false.
    m_manualPlayed=false;
    //If current mode is repeat the current track, then just play it again.
    if(RepeatTrack==m_loopState)
    {
        //Check the backend first.
        if(m_backend!=nullptr &&
                m_playingAnalysisItem.detailInfo.duration>0)
        {
            m_backend->play();
        }
        //Action finished.
        return;
    }
    //Or else, play next.
    playNextRow(false);
}

void KNMusicNowPlaying::onActionPlayingItemRemoved()
{
    //Reset current playing.
    resetCurrentPlaying();
}

void KNMusicNowPlaying::onActionLoadSuccess()
{
    //Check the model first.
    if(playingMusicModel())
    {
        //Block model signal first.
        playingMusicModel()->blockSignals(true);
        //Clear out the cannot playing flag.
        playingMusicModel()->setData(m_playingIndex,
                                     false,
                                     CannotPlayFlagRole);
        //Block model signal first.
        playingMusicModel()->blockSignals(false);
        //Give out the current.
        emit nowPlayingChanged(m_playingAnalysisItem);
    }
    //Play the backend.
    m_backend->play();
}

void KNMusicNowPlaying::onActionLoadFailed()
{
    //Set the cannot playing flag.
    playingMusicModel()->setData(m_playingIndex,
                                 true,
                                 CannotPlayFlagRole);
    //Check if the user play flag is off, then automatically play the next song,
    //Or else we stops here, let user to do the next thing.
    if(m_manualPlayed)
    {
        //Reset the playing model data.
        resetCurrentPlayingModelData();
        //We don't need to tried continued.
        return;
    }
    //Play the next row without loop mode.
    playNextRow(true);
}

void KNMusicNowPlaying::onActionModelDataChanged(const QModelIndex &topLeft,
                                                 const QModelIndex &bottomRight)
{
    //Get the current row.
    int currentRow=m_playingIndex.row();
    //Check whether the current index is in the range.
    if(currentRow >= topLeft.row() && currentRow <= bottomRight.row())
    {
        //Get the new detail info.
        m_playingAnalysisItem.detailInfo=
                playingMusicModel()->rowDetailInfo(currentRow);
        //Give out the current.
        emit nowPlayingChanged(m_playingAnalysisItem);
    }
}

void KNMusicNowPlaying::playRow(int proxyRow)
{
    //Assert the pre-check.
    Q_ASSERT(m_playingProxyModel!=nullptr &&
             proxyRow>-1 &&
             proxyRow<m_playingProxyModel->rowCount());
    //Get the music model.
    KNMusicModel *musicModel=playingMusicModel();
    //Get the new playing index.
    m_playingIndex=
            QPersistentModelIndex(m_playingProxyModel->mapToSource(
                                      m_playingProxyModel->index(proxyRow, 0)));
    //Set the playing index.
    musicModel->setPlayingIndex(m_playingIndex);
    //First we need to reanalysis that row, if we cannot analysis that row,
    //means we cannot play that row.
    KNMusicAnalysisItem reanalysisItem;
    //Get a copy from the music model.
    reanalysisItem.detailInfo=musicModel->rowDetailInfo(m_playingIndex.row());
    //Check the parser first, if you cannot play
    if(knMusicGlobal->parser()->reanalysisItem(reanalysisItem))
    {
        //Save the current reanlaysis item.
        m_playingAnalysisItem=reanalysisItem;
        //Update the music model row.
        musicModel->updateRow(m_playingIndex.row(), reanalysisItem);
        //Check the backend before playing, if there's no parser, do nothing.
        if(m_backend==nullptr)
        {
            //Treat this kind of features as load failed.
            onActionLoadFailed();
            //Finished.
            return;
        }
        //Get the detail info of the current playing anlaysis item.
        const KNMusicDetailInfo &detailInfo=
                m_playingAnalysisItem.detailInfo;
        //Play the music, according to the detail information. Check the track
        //file path is empty.
        if(detailInfo.trackFilePath.isEmpty())
        {
            //Load the whole file.
            m_backend->loadMusic(detailInfo.filePath);
        }
        else
        {
            //Load the whole file, and set the start position and the end
            //position via giving the duration.
            m_backend->loadMusic(detailInfo.filePath,
                                 detailInfo.startPosition,
                                 detailInfo.duration);
        }
        //Mission complete.
        return;
    }
    //Or else, this song should be failed to play, we cannot even analysis it,
    //how could we play it?.
    onActionLoadFailed();
}

inline void KNMusicNowPlaying::resetShadowModel()
{
    //Clear the shadow playing model.
    m_shadowPlayingModel->setSourceModel(nullptr);
    //Clear the search block.
    m_shadowPlayingModel->clearSearchBlock();
}

inline void KNMusicNowPlaying::resetCurrentPlaying()
{
    //Reset the playing model data.
    resetCurrentPlayingModelData();
    //Clear the current playing index and the analysis item.
    m_playingIndex=QPersistentModelIndex();
    m_playingAnalysisItem=KNMusicAnalysisItem();
}

void KNMusicNowPlaying::resetCurrentPlayingModelData()
{
    //To reset the now playing, clear the backend first.
    //Check whether the backend is valid.
    if(m_backend)
    {
        //Reset the backend.
        m_backend->reset();
    }
    //Clear previous the now playing icon.
    playingMusicModel()->setPlayingIndex(QModelIndex());
    //Emit now playing reset signal.
    emit nowPlayingReset();
}

inline void KNMusicNowPlaying::playNextRow(bool noLoopMode)
{
    //Check the current playing index is avaliable or not.
    if(m_playingProxyModel==nullptr ||
            m_playingProxyModel->rowCount()==0)
    {
        return;
    }
    //Check the current index is valid or not.
    //If the current index is not valid, then ask to play the first song in the
    //model.
    //If the playing index is not the same as the source model of the proxy
    //model, that should be a bug. We can fix it via playing the first song in
    //the model as well.
    if(!m_playingIndex.isValid() ||
            m_playingIndex.model()!=m_playingProxyModel->sourceModel())
    {
        //Play the first row.
        playRow(0);
        //Finished.
        return;
    }
    //Check the loop state, if it's shuffle mode, use a specfic way to get the
    //index.
    if(Shuffle==m_loopState)
    {
        //Play the shuffle data.
        //Generate the shuffle distribution.
        std::uniform_int_distribution<> gapDistribution(
                    0,
                    m_playingProxyModel->rowCount()-1);
        //Get the row index.
        int preferRowGap=gapDistribution(m_mersenneSeed);
        //Check out the prefer gap.
        if(preferRowGap==0)
        {
            //Update the prefer gap.
            preferRowGap=gapDistribution(m_mersenneSeed);
        }
        //Calculate the prefer row, reuse the prefer row gap.
        preferRowGap=m_playingProxyModel->mapFromSource(m_playingIndex).row()+
                preferRowGap;
        //Make sure the number is valid.
        if(preferRowGap>=m_playingProxyModel->rowCount())
        {
            //To fix it, it's simple, reduce row count.
            preferRowGap-=(m_playingProxyModel->rowCount()-1);
        }
        //Play the row.
        playRow(preferRowGap);
        //Check out the prefer row is empty or not.
        return;
    }
    //Get the next row.
    int proxyRow=m_playingProxyModel->mapFromSource(m_playingIndex).row(),
        nextProxyRow;
    //If the row is the last row in the model,
    if(proxyRow==m_playingProxyModel->rowCount()-1)
    {
        //If it doesn't in a loop mode,
        if(noLoopMode)
        {
            //then reach the end of the model.
            nextProxyRow=-1;
        }
        else
        {
            //Now, according to the loop mode, give out the row.
            //Only Repeat All can go back from the first line of the proxy model.
            nextProxyRow=((m_loopState==RepeatAll)?0:-1);
        }
    }
    else
    {
        //Normal case: return the next row of the current row.
        nextProxyRow=proxyRow+1;
    }
    //Check if the row is available.
    if(nextProxyRow==-1)
    {
        //Clear the current playing.
        resetCurrentPlaying();
        //Everything is done.
        return;
    }
    //Play the new row.
    playRow(nextProxyRow);
}
