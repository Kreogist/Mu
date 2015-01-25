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

#include "knmusicsingleplaylistmodel.h"
#include "knmusicmodelassist.h"
#include "knmusicproxymodel.h"
#include "knmusictab.h"

#include "knlocalemanager.h"

#include "knmusicnowplaying.h"

#include <QDebug>

KNMusicNowPlaying::KNMusicNowPlaying(QObject *parent) :
    KNMusicNowPlayingBase(parent)
{
    //Initial the icon.
    m_playingIcon=QPixmap(":/plugin/music/common/playingicon.png");
    m_cantPlayIcon=QPixmap(":/plugin/music/common/cantplay.png");
    //Initial music global.
    m_musicGlobal=KNMusicGlobal::instance();
    //Initial temporary model.
    m_temporaryModel=new KNMusicSinglePlaylistModel(this);
    //Initial shadow proxy model.
    m_shadowPlayingModel=new KNMusicProxyModel(this);
    //Generate temporary proxy model.
    m_temporaryProxyModel=new KNMusicProxyModel(this);
    m_temporaryProxyModel->setSourceModel(m_temporaryModel);

    connect(this, &KNMusicNowPlaying::requirePlayPrevAvailable,
            this, &KNMusicNowPlaying::playPrevAvailable);
    connect(this, &KNMusicNowPlaying::requirePlayNextAvailable,
            this, &KNMusicNowPlaying::playNextAvailable);


    //Connect apply preference signal.
    connect(KNPreferenceItemGlobal::instance(), &KNPreferenceItemGlobal::requireApplyPreference,
            this, &KNMusicNowPlaying::applyPreference);
    //Load preference and retranslate.
    applyPreference();
    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicNowPlaying::retranslate);
    retranslate();
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
    setLoopState(m_musicGlobal->configureData("LoopState",
                                              NoRepeat).toInt());
}

void KNMusicNowPlaying::playNext()
{
    //Get the next index with loop mode.
    int preferNextRow=nextSongIndex(m_playingModel->mapFromSource(m_currentPlayingIndex).row());
    //Check the index available.
    if(preferNextRow==-1)
    {
        resetPlayingItem();
        return;
    }
    //Or else play the row.
    if(playMusicRow(preferNextRow))
    {
        return;
    }
    //Process the events.
    qApp->processEvents();
    //Play the next available song if user wants.
    if(m_playNextAvailable)
    {
        emit requirePlayNextAvailable(preferNextRow);
    }
}

void KNMusicNowPlaying::playPrevious()
{
    //Get the previous index with loop mode.
    int preferPrevRow=prevSongIndex(m_playingModel->mapFromSource(m_currentPlayingIndex).row());
    //Check the index available.
    if(preferPrevRow==-1)
    {
        resetPlayingItem();
        return;
    }
    //Or else play the row.
    if(playMusicRow(preferPrevRow))
    {
        return;
    }
    //Process the events.
    qApp->processEvents();
    //Play the previous available song if the user wants.
    if(m_playNextAvailable)
    {
        emit requirePlayPrevAvailable(preferPrevRow);
    }
}

void KNMusicNowPlaying::onActionPlayingFinished()
{
    //Add play times, check the playing model and index.
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

void KNMusicNowPlaying::setPlayingModel(KNMusicProxyModel *model,
                                        KNMusicTab *tab)
{
    //Reset current item and music model.
    resetPlayingItem();
    resetPlayingModels();
    //Save the proxy model and music model.
    m_playingModel=model;
    m_currentTab=tab;
    if(m_playingModel!=nullptr)
    {
        m_playingMusicModel=m_playingModel->musicModel();
    }
}

void KNMusicNowPlaying::playMusic(int row)
{
    //If we can play this row.
    if(playMusicRow(row))
    {
        return;
    }
    //Process the events.
    qApp->processEvents();
    //Play the next available song if the user wants, give the initial row.
    if(m_playNextAvailable)
    {
        emit requirePlayNextAvailable(row);
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

void KNMusicNowPlaying::applyPreference()
{
    m_playNextAvailable=m_musicGlobal->configureData("PlayNextAvailable",
                                                     m_playNextAvailable).toBool();
}

void KNMusicNowPlaying::retranslate()
{
    //Get the latest title and item info.
    KNPreferenceTitleInfo lyricsTitle;
    QList<KNPreferenceItemInfo> itemList;
    generateTitleAndItemInfo(lyricsTitle, itemList);
    //Ask to insert the info list.
    KNMusicGlobal::instance()->insertItemInfoList(lyricsTitle, itemList);
}

void KNMusicNowPlaying::saveConfigure()
{
    m_musicGlobal->setConfigureData("LoopState", m_loopMode);
}

void KNMusicNowPlaying::generateTitleAndItemInfo(KNPreferenceTitleInfo &listTitle,
                                                 QList<KNPreferenceItemInfo> &list)
{
    //Set the title.
    listTitle.advanced=false;
    listTitle.title=tr("Playback");
    listTitle.titleIdentifier="Playback";

    //Clear the list.
    list.clear();
    //Add the current info.
    list.append(KNPreferenceItemGlobal::generateInfo(Switcher,
                                                     tr("Play next when error"),
                                                     "PlayNextAvailable",
                                                     m_playNextAvailable));
}

void KNMusicNowPlaying::playNextAvailable(const int &currentProxyRow)
{
    //Process the events.
    qApp->processEvents();
    //We need to search the next available index.
    int preferRow=nextSongIndex(currentProxyRow, true);
    if(preferRow==-1)
    {
        //If we come here, means there's no file we can play.
        resetPlayingItem();
        return;
    }
    //Get the next available row.
    if(playMusicRow(preferRow))
    {
        return;
    }
    //Process the events.
    qApp->processEvents();
    //Emit check next signal.
    emit requirePlayNextAvailable(preferRow);
}

void KNMusicNowPlaying::playPrevAvailable(const int &currentProxyRow)
{
    //Process the events.
    qApp->processEvents();
    //We need to search the next available index.
    int preferRow=prevSongIndex(currentProxyRow, true);
    if(preferRow==-1)
    {
        //If we come here, means there's no file we can play.
        resetPlayingItem();
        return;
    }
    //Get the next available row.
    if(playMusicRow(preferRow))
    {
        return;
    }
    //Process the events.
    qApp->processEvents();
    //Emit check next signal.
    emit requirePlayPrevAvailable(preferRow);
}

void KNMusicNowPlaying::setRating(const int &rating)
{
    //Get the current index.
    int ratingRow=m_currentPlayingIndex.row();
    //Set the rating to the row.
    m_playingMusicModel->setItemText(ratingRow,
                                     Rating,
                                     QString::number(rating));
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
            if(!m_playingMusicModel->rowProperty(m_currentPlayingIndex.row(),
                                                 CantPlayFlagRole).toBool())
            {
                m_playingMusicModel->setRoleData(m_currentPlayingIndex.row(),
                                                 BlankData,
                                                 Qt::DecorationRole,
                                                 QPixmap());
            }
        }
        m_currentPlayingIndex=QPersistentModelIndex();
        m_currentPlayingAnalysisItem=KNMusicAnalysisItem();
    }
}

void KNMusicNowPlaying::resetPlayingModels()
{
    //Clear the playing model.
    m_playingModel=nullptr;
    m_currentTab=nullptr;
    //Clear the shadow playing model data.
    m_shadowPlayingModel->setSourceModel(nullptr);
    m_shadowPlayingModel->setSortRole(-1);
    m_shadowPlayingModel->setFilterFixedString("");
    m_shadowPlayingModel->setFilterRole(-1);
    //Clear music model.
    m_playingMusicModel=nullptr;
}

int KNMusicNowPlaying::prevSongIndex(int currentProxyRow,
                                     bool ignoreLoopMode)
{
    //If there's no model or the source model is not the music model,
    //return an invaild row.
    if(m_playingModel==nullptr)
    {
        return -1;
    }
    //If current playing index is invaild, then try to play the first item.
    if(currentProxyRow<0)
    {
        return m_playingModel->rowCount()>0?0:-1;
    }
    //If it's the first one
    if(currentProxyRow==0)
    {
        if(ignoreLoopMode)
        {
            //Reach the start of the model.
            return -1;
        }
        switch(m_loopMode)
        {
        case NoRepeat:
        case RepeatTrack:
            //Reach the start of the model.
            return -1;
        case RepeatAll:
            //Play the last one.
            return m_playingModel->rowCount()-1;
        }
    }
    //Return the previous row.
    return currentProxyRow-1;
}

int KNMusicNowPlaying::nextSongIndex(int currentProxyRow,
                                     bool ignoreLoopMode)
{
    //If there's no model or the source model is not the music model,
    //return an invaild row.
    if(m_playingModel==nullptr)
    {
        return -1;
    }
    //Check the current row, if it is smaller than 0, means current row is
    //unavailable.
    if(currentProxyRow<0)
    {
        return m_playingModel->rowCount()>0?0:-1;
    }
    //If the row is the last one.
    if(currentProxyRow==m_playingModel->rowCount()-1)
    {
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
        case RepeatAll:
            //Back to the first one.
            return 0;
        }
    }
    //Return the next song's row.
    return currentProxyRow+1;
}

bool KNMusicNowPlaying::playMusicRow(const int &row)
{
    Q_ASSERT(m_playingModel!=nullptr &&
            row>-1 &&
            row<m_playingModel->rowCount());
    //Clear the current item.
    resetPlayingItem();
    //Get the source index. Storage the index as persistent index.
    QPersistentModelIndex sourceIndex=
            QPersistentModelIndex(m_playingModel->mapToSource(
                                      m_playingModel->index(row,
                                                            m_playingModel->playingItemColumn())));
    //Parse the current index.
    KNMusicAnalysisItem currentItem;
    if(KNMusicModelAssist::reanalysisRow(m_playingMusicModel,
                                         sourceIndex,
                                         currentItem))
    {
        KNMusicDetailInfo &currentInfo=currentItem.detailInfo;
        //Update the data in proxy model first.
        m_playingMusicModel->updateMusicRow(sourceIndex.row(),
                                            currentInfo);
        //Get the result of playing the file.
        bool playingResult=
                currentInfo.startPosition==-1?
                    m_backend->playFile(currentInfo.filePath):
                    m_backend->playSection(currentInfo.filePath,
                                           currentInfo.startPosition,
                                           currentInfo.duration);
        //If we play the music successful,
        if(playingResult)
        {
            //Save the current analysis item and current index.
            m_currentPlayingIndex=sourceIndex;
            m_currentPlayingAnalysisItem=currentItem;
            //Clear the cannot play flag.
            m_playingMusicModel->setRowProperty(m_currentPlayingIndex.row(),
                                                CantPlayFlagRole,
                                                false);
            //Set the playing icon.
            m_playingMusicModel->setRoleData(m_currentPlayingIndex.row(),
                                             BlankData,
                                             Qt::DecorationRole,
                                             m_playingIcon);
            //Emit current changed signal.
            emit nowPlayingChanged();
        }
        else
        {
            //Set the cannot play icon to current row.
            setCannotPlay(sourceIndex.row());
        }
        return playingResult;
    }
    //If we can't analysis the file, this file can't be played.
    return false;
}

void KNMusicNowPlaying::setCannotPlay(const int &row)
{
    //Set the cannot play flag.
    m_playingMusicModel->setRowProperty(row,
                                        CantPlayFlagRole,
                                        true);
    //Set cannot play icon to current row.
    m_playingMusicModel->setRoleData(row,
                                     BlankData,
                                     Qt::DecorationRole,
                                     m_cantPlayIcon);
}

QPersistentModelIndex KNMusicNowPlaying::currentPlayingIndex() const
{
    return m_currentPlayingIndex;
}

KNMusicAnalysisItem KNMusicNowPlaying::currentAnalaysisItem() const
{
    return m_currentPlayingAnalysisItem;
}

void KNMusicNowPlaying::backupCurrentPlaying()
{
    m_backupPosition=m_backend->position();
}

void KNMusicNowPlaying::restoreCurrentPlaying()
{
    //Check if the backup position is available.
    //If it's not -1, means we should play the current index again,
    //set the position the backup position.
    if(m_backupPosition!=-1)
    {
        //Replay the current playing index.
        playMusic(m_playingModel->mapFromSource(m_currentPlayingIndex));
        //Restore the playing position.
        m_backend->setPosition(m_backupPosition);
        //Reset the backup flag to invaild.
        m_backupPosition=-1;
    }
}

void KNMusicNowPlaying::showCurrentIndexInOriginalTab()
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
