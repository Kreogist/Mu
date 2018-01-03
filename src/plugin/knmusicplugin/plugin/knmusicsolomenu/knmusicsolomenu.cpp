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
#include <QFileInfo>
#include <QDir>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knutil.h"
#include "knfilenamemessagebox.h"

#include "knmusicproxymodel.h"
#include "knmusicmodel.h"
#include "knmusicglobal.h"
#include "knmusicsearchbase.h"
#include "knmusicbackend.h"
#include "knmusicnowplayingbase.h"
#include "knmusicdetaildialog.h"
#include "knmusiclyricsdownloaddialogbase.h"

#include "knmusicsolomenu.h"

#include <QDebug>

KNMusicSoloMenu::KNMusicSoloMenu(QWidget *parent) :
    KNMusicSoloMenuBase(parent),
    m_model(nullptr),
    m_itemIndex(QModelIndex()),
    m_itemText(QString()),
    m_filePath(QString()),
    m_preferFileName(QString())
{
    setObjectName("MusicSoloMenu");
    //Set the separator color.
    setSeparatorColor(QColor(255, 255, 255, 100));
    //Link to the theme manager.
    knTheme->registerWidget(this);
    //Generate the actions.
    for(int i=0; i<SoloMenuActionCount; ++i)
    {
        //Generate the action.
        m_actions[i]=new QAction(this);
    }
    //Link actions.
    connect(m_actions[PlayCurrent], &QAction::triggered,
            this, &KNMusicSoloMenu::requirePlayCurrent);
    connect(m_actions[Open], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionOpen);
    connect(m_actions[GetInfo], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionGetInfo);
    connect(m_actions[ShowInGraphicShell], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionShowInGraphicShell);
    connect(m_actions[SearchItemText], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionSearchItemText);
    connect(m_actions[DownloadLyrics], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionDownloadLyrics);
    connect(m_actions[CopyFilePath], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionCopyFilePath);
    connect(m_actions[CopyItemText], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionCopyItemText);
    connect(m_actions[Rename], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionRename);
    connect(m_actions[RenameToArtistHyphonName], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionRenameToPrefer);
    connect(m_actions[Delete], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionRemove);

    //Add actions to the menu.
    addAction(m_actions[PlayCurrent]);
    addAction(m_actions[Open]);
    addSeparator();
    addAction(m_actions[GetInfo]);
    addAction(m_actions[ShowInGraphicShell]);
    addSeparator();
    addAction(m_actions[SearchItemText]);
    addSeparator();
    addAction(m_actions[DownloadLyrics]);
    addSeparator();
    addAction(m_actions[Rename]);
    addAction(m_actions[RenameToArtistHyphonName]);
    addSeparator();
    addAction(m_actions[CopyFilePath]);
    addAction(m_actions[CopyItemText]);
    addSeparator();
    addAction(m_actions[Delete]);

    //Link the locale manager.
    knI18n->link(this, &KNMusicSoloMenu::retranslate);
    retranslate();
}

void KNMusicSoloMenu::setMusicRow(KNMusicProxyModel *model,
                                  const QModelIndex &itemIndex)
{
    //Check the model and the item index.
    if(model==nullptr || !itemIndex.isValid())
    {
        return;
    }
    //Save the model and the item index.
    m_model=model;
    m_itemIndex=itemIndex;
    //Save a copy of the row of the index.
    int row=m_itemIndex.row();
    //Get the detail info from the model.
    KNMusicDetailInfo &&detailInfo=model->rowDetailInfo(row);
    //Get the current item text data and file path from the model.
    m_itemText=m_model->textData(row, m_itemIndex.column());
    m_filePath=detailInfo.filePath;
    //Update action captions.
    m_actions[PlayCurrent]->setText(
                m_actionTitles[PlayCurrent].arg(
                    detailInfo.textLists[Name].toString()));
    m_actions[Open]->setText(m_actionTitles[Open].arg(detailInfo.fileName));
    //Get the prefer name.
    //Check the aritist is empty or not.
    QString &&artistText=detailInfo.textLists[Artist].toString();
    if(artistText.isEmpty())
    {
        //When the artist is empty, there's a thing we have to check.
        //If the title is the file name, means there's no tag in the file.
        //We won't make it to be CutiePanther.flac.flac, simply set the file
        //name to be prefer file name.
        //Get the file name.
        QString &&titleText=detailInfo.textLists[Name].toString();
        //Check the title text is the same or not.
        if(titleText==detailInfo.fileName)
        {
            //We won't change the prefer name text.
            m_preferFileName=detailInfo.fileName;
        }
        else
        {
            //Simply named it as Title.Suffix.
            m_preferFileName=
                    KNUtil::legalFileName(titleText + "." +
                                          QFileInfo(m_filePath).suffix());
        }
    }
    else
    {
        //Named it as Artist - Title.Suffix.
        m_preferFileName=
                KNUtil::legalFileName(artistText +
                                      " - " +
                                      detailInfo.textLists[Name].toString() +
                                      "." +
                                      QFileInfo(m_filePath).suffix());
    }
    //Check the track file path row, if the item contains track list file, hide
    //the rename action.
    bool renameGlobalFlag=detailInfo.trackFilePath.isEmpty();
    //Set the rename flag
    m_actions[Rename]->setVisible(renameGlobalFlag);
    //Check out the prefer name.
    m_actions[RenameToArtistHyphonName]->setVisible(
                !(m_preferFileName==detailInfo.fileName) && renameGlobalFlag);
    //Update the title.
    m_actions[RenameToArtistHyphonName]->setText(
                m_actionTitles[RenameToArtistHyphonName].arg(m_preferFileName));
    //Check the item text, set the visible of the item text related actions.
    if(m_itemText.isEmpty())
    {
        //Set the action invisible.
        m_actions[CopyItemText]->setVisible(false);
        m_actions[SearchItemText]->setVisible(false);
    }
    else
    {
        //Set the action visible.
        m_actions[CopyItemText]->setVisible(true);
        m_actions[SearchItemText]->setVisible(true);
        //Update the caption.
        m_actions[CopyItemText]->setText(
                    m_actionTitles[CopyItemText].arg(m_itemText));
        m_actions[SearchItemText]->setText(
                    m_actionTitles[SearchItemText].arg(m_itemText));
    }
}

KNMusicProxyModel *KNMusicSoloMenu::triggeredModel()
{
    return m_model;
}

QModelIndex KNMusicSoloMenu::triggeredIndex() const
{
    return m_itemIndex;
}

void KNMusicSoloMenu::appendMusicActions(const QList<QAction *> &actions)
{
    //Insert the actions before .
    insertActions(m_actions[SearchItemText], actions);
    //Insert seperator.
    insertSeparator(m_actions[SearchItemText]);
}

void KNMusicSoloMenu::retranslate()
{
    //Translate all the action caption. Save them in title caption string list.
    m_actionTitles[PlayCurrent]=tr("Play %1");
    m_actionTitles[Open]=tr("Open %1");
    m_actionTitles[GetInfo]=tr("Get Info");
#ifdef Q_OS_WIN
    m_actionTitles[ShowInGraphicShell]=tr("Show in Explorer");
#endif
#ifdef Q_OS_MAC
    m_actionTitles[ShowInGraphicShell]=tr("Show in Finder");
#endif
#ifdef Q_OS_LINUX
    m_actionTitles[ShowInGraphicShell]=tr("Show the contains folder");
#endif
    m_actionTitles[DownloadLyrics]=tr("Download Lyrics");
    m_actionTitles[Rename]=tr("Rename");
    m_actionTitles[RenameToArtistHyphonName]=tr("Rename to %1");
    m_actionTitles[CopyFilePath]=tr("Copy location");
    m_actionTitles[CopyItemText]=tr("Copy \"%1\"");
    m_actionTitles[SearchItemText]=tr("Search %1");
    m_actionTitles[Delete]=tr("Delete");

    //Apply the translation.
    //Because you can't change the language when you operate the menu. so we
    //don't need to care with the data of %1.
    for(int i=0; i<SoloMenuActionCount; i++)
    {
        m_actions[i]->setText(m_actionTitles[i]);
    }
}

void KNMusicSoloMenu::onActionOpen()
{
    //Use system default file associate to open the file.
    KNUtil::openLocalFile(m_filePath);
}

void KNMusicSoloMenu::onActionGetInfo()
{
    //Generate an analysis item.
    KNMusicAnalysisItem item;
    //Set the detail info to item.
    item.detailInfo=m_model->rowDetailInfo(m_itemIndex);
    //Show the detail dialog.
    knMusicGlobal->detailDialog()->showDialog(item, m_model, m_itemIndex);
}

void KNMusicSoloMenu::onActionShowInGraphicShell()
{
    //Show the current file in graphics shell.
    KNUtil::showInGraphicalShell(m_filePath);
}

void KNMusicSoloMenu::onActionSearchItemText()
{
    //Get the searcher.
    KNMusicSearchBase *searcher=knMusicGlobal->search();
    //Check if it's valid.
    if(searcher==nullptr)
    {
        return;
    }
    //Generate the search block.
    KNMusicSearchBlock block;
    //Set data to block.
    block.isColumn=true;
    block.index=m_itemIndex.column();
    block.value=m_itemText;
    //Generate the search block list.
    QList<KNMusicSearchBlock> blocks;
    blocks.append(block);
    //Do search.
    searcher->search(blocks);
}

void KNMusicSoloMenu::onActionDownloadLyrics()
{
    //Check the pointer of download dialog and model.
    //Check the validation of the item index.
    if(knMusicGlobal->lyricsDownloadDialog() && m_model
            && m_itemIndex.isValid())
    {
        //Get the download dialog.
        KNMusicLyricsDownloadDialogBase *downloadDialog=
                knMusicGlobal->lyricsDownloadDialog();
        //Set the detail info to the download dialog.
        downloadDialog->setDetailInfo(
                    m_model->rowDetailInfo(m_itemIndex.row()));
        //Show up the download lyrics dialog, and give it the detail info.
        downloadDialog->exec();
    }
}

void KNMusicSoloMenu::onActionRenameToPrefer()
{
    //Rename the file to prefer file name.
    renameFile(m_preferFileName);
}

void KNMusicSoloMenu::onActionRename()
{
    //Get the file name.
    QString &&newName=KNFileNameMessageBox::getFileName(
                tr("Please input the new file name"),
                "Rename",
                m_filePath);
    //Check out the new file name.
    if(newName.isEmpty())
    {
        return;
    }
    //Rename the file to the new file.
    renameFile(newName);
}

void KNMusicSoloMenu::onActionCopyFilePath()
{
    //Set the clipboard text to be the file path.
    KNUtil::setClipboardText(QDir::toNativeSeparators(m_filePath));
}

void KNMusicSoloMenu::onActionCopyItemText()
{
    //Set the item text to clipboard.
    KNUtil::setClipboardText(m_itemText);
}

void KNMusicSoloMenu::onActionRemove()
{
    //Remove the row from the model.
    if(m_model && m_itemIndex.isValid())
    {
        //Remove the row.
        m_model->removeRow(m_itemIndex.row());
    }
}

inline void KNMusicSoloMenu::renameFile(const QString &newName)
{
    //Check whether the new name is valid.
    //Get the file info.
    QFileInfo fileInfo(m_filePath),
              //Get the new name file.
              newNameFile(fileInfo.absoluteDir().filePath(newName));
    //Check the existence of the new name file.
    if(newNameFile.exists())
    {
        //Show up the message.
        KNMessageBox::information(
                    tr("File %1 already exists.").arg(newNameFile.fileName()),
                    "Error",
                    Qt::AlignCenter);
        //Mission complete.
        return;
    }
    //Initial the save flag of the backend.
    bool saveFlag=false;
    //Check if the current file is the playing file.
    //Get the now playing.
    KNMusicNowPlayingBase *nowPlaying=knMusicGlobal->nowPlaying();
    //Check now playing pointer.
    if(nowPlaying)
    {
        //Check out the file path and the now playing file path.
        saveFlag=(nowPlaying->playingItem().detailInfo.filePath==m_filePath);
    }
    //Get the backend pointer.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the save flag first.
    if(saveFlag)
    {
        //Save the backend.
        backend->save();
    }
    //Get the current file path.
    QString &&currentPath=fileInfo.absoluteDir().filePath(newName);
    //Rename the file to the new name.
    QFile::rename(m_filePath, currentPath);
    //Get the file info.
    QFileInfo currentFileInfo(currentPath);
    //Check existence.
    if(currentFileInfo.exists())
    {
        //Get the current detail info.
        KNMusicDetailInfo detailInfo=m_model->rowDetailInfo(m_itemIndex);
        //Change the file path.
        detailInfo.filePath=currentFileInfo.absoluteFilePath();
        //Change the file name.
        detailInfo.fileName=currentFileInfo.fileName();
        //Check out the title string.
        if(detailInfo.textLists[Name].toString()==fileInfo.fileName())
        {
            //Change the name to be the new file name.
            detailInfo.textLists[Name]=currentFileInfo.fileName();
        }
        //Update the detail info.
        m_model->musicModel()->replaceRow(
                    m_model->mapToSource(m_itemIndex).row(),
                    detailInfo);
    }
    //Check the save flag.
    if(saveFlag)
    {
        //Restore the backend, give the new file path.
        backend->restore(currentFileInfo.absoluteFilePath());
    }
}
