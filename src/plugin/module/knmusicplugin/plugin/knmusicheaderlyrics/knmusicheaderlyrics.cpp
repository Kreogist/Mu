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
#include <QTimeLine>
#include <QApplication>
#include <QPaintEvent>
#include <QPainter>

#include "knconfigure.h"

#include "knmusicheaderplayerbase.h"
#include "knmusiclyricsmanager.h"

#include "knmusicheaderlyrics.h"

#include <QDebug>

KNMusicHeaderLyrics::KNMusicHeaderLyrics(QWidget *parent) :
    KNMusicHeaderLyricsBase(parent)
{
    //Initial the music global, music configure and lyrics manager.
    m_musicGlobal=KNMusicGlobal::instance();
    connect(m_musicGlobal, &KNMusicGlobal::musicLibraryMoved,
            this, &KNMusicHeaderLyrics::onActionMusicLibraryMoved);

    m_musicConfigure=m_musicGlobal->musicConfigure();

    m_lyricsManager=KNMusicLyricsManager::instance();
    connect(m_lyricsManager, &KNMusicLyricsManager::lyricsReset,
            this, &KNMusicHeaderLyrics::onActionLyricsReset);
    connect(m_lyricsManager, &KNMusicLyricsManager::lyricsUpdate,
            this, &KNMusicHeaderLyrics::onActionLyricsUpdate);
#ifdef Q_OS_WIN32
    //Set line spacing specially for Windows. It seems that in Qt 5.4, line
    //spacing has been calculate into a text line.
    setSpacing(0);
#endif

    //Connect apply preference signal.
    connect(KNPreferenceItemGlobal::instance(), &KNPreferenceItemGlobal::requireApplyPreference,
            this, &KNMusicHeaderLyrics::applyPreference);
    //Load preference and retranslate.
    applyPreference();
    //Connect retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicHeaderLyrics::retranslate);
    retranslate();
}

KNMusicHeaderLyrics::~KNMusicHeaderLyrics()
{
}

void KNMusicHeaderLyrics::setHeaderPlayer(KNMusicHeaderPlayerBase *player)
{
    //Save the player.
    m_player=player;
    //Link the player.
    connect(m_player, &KNMusicHeaderPlayerBase::playerReset,
            this, &KNMusicHeaderLyrics::onActionLyricsReset);
    connect(m_player, &KNMusicHeaderPlayerBase::positionChanged,
            this, &KNMusicHeaderLyrics::onActionPositionChange);
}

void KNMusicHeaderLyrics::retranslate()
{
    //Get the latest title and item info.
    KNPreferenceTitleInfo lyricsTitle;
    QList<KNPreferenceItemInfo> itemList;
    generateTitleAndItemInfo(lyricsTitle, itemList);
    //Ask to insert the info list.
    KNMusicGlobal::instance()->insertItemInfoList(lyricsTitle, itemList);
}

void KNMusicHeaderLyrics::onActionLyricsUpdate()
{
    //Reset the lyrics first.
    onActionLyricsReset();
    //Check is the updated lyrics is the player's lyrics.
    if(m_lyricsManager->musicDetailInfo().filePath!=m_player->currentDetailInfo().filePath ||
            m_lyricsManager->musicDetailInfo().trackFilePath!=m_player->currentDetailInfo().trackFilePath ||
            m_lyricsManager->musicDetailInfo().trackIndex!=m_player->currentDetailInfo().trackIndex ||
            m_lyricsManager->positionList().isEmpty())
    {
        return;
    }
    //Set the lyrics data.
    setLyricsData(m_lyricsManager->positionList(),
                  m_lyricsManager->textList());
}

void KNMusicHeaderLyrics::applyPreference()
{
    //Update the lyrics folder.
    m_lyricsManager->setLyricsDir(
                m_musicConfigure->getData("LyricsFolder",
                                          m_lyricsManager->lyricsDir()).toString());
    //Update the download info.
    m_lyricsManager->setEnableOnlineLyrics(
                m_musicConfigure->getData("DownloadLyrics",
                                          m_lyricsManager->enableOnlineLyrics()).toBool());
    //Update the spacing.
    setSpacing(m_musicConfigure->getData("TextSpacing",
                                         spacing()).toInt());
    //Update the font.
    setFont(m_musicConfigure->getData("LyricsFont", font()).value<QFont>());
    //Update the lyrics.
    update();
}

void KNMusicHeaderLyrics::onActionMusicLibraryMoved(const QString &originalPath,
                                                    const QString &currentPath)
{
    //Check if lyrics manager's folder path is in the orginal path.
    QString managerFolderPath=m_lyricsManager->lyricsDir();
    if(managerFolderPath.left(originalPath.size())==originalPath)
    {
        //Set the lyrics manager to the new path.
        QString currentFolderPath=
                currentPath+managerFolderPath.mid(originalPath.size());
        m_lyricsManager->setLyricsDir(currentFolderPath);
        m_musicConfigure->setData("LyricsFolder",
                                  currentFolderPath);
        //Update the lyrics path value.
        m_musicGlobal->updateItemValue("LyricsFolder");
    }
}

inline void KNMusicHeaderLyrics::generateTitleAndItemInfo(KNPreferenceTitleInfo &listTitle,
                                                          QList<KNPreferenceItemInfo> &list)
{
    //Set the title.
    listTitle.advanced=false;
    listTitle.title=tr("Lyrics");
    listTitle.titleIdentifier="Lyrics";

    //Clear the list.
    list.clear();
    //Add the current info.
    list.append(KNPreferenceItemGlobal::generateInfo(PathEditItem,
                                                     tr("Lyrics Folder"),
                                                     "LyricsFolder",
                                                     m_lyricsManager->lyricsDir()));
    list.append(KNPreferenceItemGlobal::generateInfo(SwitcherItem,
                                                     tr("Download Lyrics"),
                                                     "DownloadLyrics",
                                                     m_lyricsManager->enableOnlineLyrics()));
    list.append(KNPreferenceItemGlobal::generateInfo(FontItem,
                                                     tr("Lyrics Font"),
                                                     "LyricsFont",
                                                     font()));
    KNPreferenceItemInfo currentInfo=KNPreferenceItemGlobal::generateInfo(NumberItem,
                                                                          tr("Text Spacing"),
                                                                          "TextSpacing",
                                                                          spacing(),
                                                                          spacing(),
                                                                          true);
    currentInfo.property.insert("Min", 0);
    currentInfo.property.insert("Max", 15);
    list.append(currentInfo);
}
