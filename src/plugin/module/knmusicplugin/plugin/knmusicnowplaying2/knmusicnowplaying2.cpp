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
#include "knglobal.h"
#include "knconfigure.h"

#include "knmusicsingleplaylistmodel.h"
#include "knmusicmodelassist.h"
#include "knmusicproxymodel.h"
#include "knmusictab.h"

#include "knmusicnowplaying2.h"

KNMusicNowPlaying2::KNMusicNowPlaying2(QObject *parent) :
    KNMusicNowPlayingBase(parent)
{
    //Get the configures.
    m_cacheConfigure=KNGlobal::instance()->cacheConfigure();
    m_musicConfigure=KNMusicGlobal::instance()->musicConfigure();
    //Initial the models.
    initialTemporaryModel();
    initialShadowModel();

    //Link the apply preference request signal and apply preference.
    connect(KNPreferenceItemGlobal::instance(), &KNPreferenceItemGlobal::requireApplyPreference,
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
        //!TODO: add current playing index.
        //Restore the playing position.
        m_backend->setPosition(m_backupPosition);
        //Reset the backup position.
        m_backupPosition=-1;
    }
}

void KNMusicNowPlaying2::resetCurrentPlaying()
{

}

void KNMusicNowPlaying2::restoreConfigure()
{
    //Restore the configure from cache data.
    //1. Recover the loop state.
    setLoopState(m_cacheConfigure->getData("LoopState",
                                           NoRepeat).toInt());
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
    //We don't need to redirect the current playing index and music model,
    //because the music model never changed.
}

void KNMusicNowPlaying2::playMusicRow(KNMusicProxyModel *model,
                                      int row,
                                      KNMusicTab *tab)
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
    //Play the row.
    playRow(row);
}

void KNMusicNowPlaying2::onActionPlayingFinished()
{
    ;
}

void KNMusicNowPlaying2::onActionCantLoad()
{
    ;
}

void KNMusicNowPlaying2::onActionLoaded()
{
    ;
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

inline void KNMusicNowPlaying2::playRow(const int &proxyRow)
{
    Q_ASSERT(m_playingModel!=nullptr &&
            proxyRow>-1 &&
            proxyRow<m_playingModel->rowCount());
    //Get the source index, and save the source index as a persistent index.
    QPersistentModelIndex sourceIndex=
            QPersistentModelIndex(m_playingModel->mapToSource(
                                      m_playingModel->index(proxyRow,
                                                            m_playingModel->playingItemColumn())));
    //First we need to reanalysis that row, if we cannot analysis that row,
    //means we cannot .
    KNMusicAnalysisItem currentAnalysisItem;
    if(KNMusicModelAssist::reanalysisRow(m_playingMusicModel,
                                         sourceIndex,
                                         currentAnalysisItem))
    {
        //Get the detail info.
        KNMusicDetailInfo &currentInfo=currentAnalysisItem.detailInfo;
        //Update the music model row.
        m_playingMusicModel->updateMusicRow(sourceIndex.row(), currentInfo);
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
