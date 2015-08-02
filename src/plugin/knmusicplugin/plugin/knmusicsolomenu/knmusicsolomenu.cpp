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
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knutil.h"

#include "knmusicproxymodel.h"
#include "knmusicglobal.h"
#include "knmusicsearchbase.h"
#include "knmusicdetaildialog.h"

#include "knmusicsolomenu.h"

#include <QDebug>

KNMusicSoloMenu::KNMusicSoloMenu(QWidget *parent) :
    KNMusicSoloMenuBase(parent),
    m_model(nullptr),
    m_itemIndex(QModelIndex()),
    m_itemText(QString()),
    m_filePath(QString())
{
    setObjectName("MusicSoloMenu");
    //Set the separator color.
    setSeparatorColor(QColor(255, 255, 255, 100));
    //Link to the theme manager.
    knTheme->registerWidget(this);
    //Generate the actions.
    for(int i=0; i<SoloMenuActionCount; i++)
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
    connect(m_actions[CopyFilePath], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionCopyFilePath);
    connect(m_actions[CopyItemText], &QAction::triggered,
            this, &KNMusicSoloMenu::onActionCopyItemText);

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
    //Get the current item text data and file path from the model.
    m_itemText=m_model->textData(row, m_itemIndex.column());
    m_filePath=m_model->propertyData(row, FilePathRole).toString();
    //Get the file name data from the model.
    QString fileName=m_model->propertyData(row, FileNameRole).toString();
    //Update action captions.
    m_actions[PlayCurrent]->setText(
                m_actionTitles[PlayCurrent].arg(m_model->textData(row, Name)));
    m_actions[Open]->setText(m_actionTitles[Open].arg(fileName));
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

void KNMusicSoloMenu::retranslate()
{
    //Translate all the action caption. Save them in title caption string list.
    m_actionTitles[PlayCurrent]=tr("Play %1");
    m_actionTitles[Open]=tr("Open %1");
    m_actionTitles[GetInfo]=tr("Get Info");
#ifdef Q_OS_WIN32
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
    knMusicGlobal->detailDialog()->showDialog(item);
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

void KNMusicSoloMenu::onActionCopyFilePath()
{
    KNUtil::setClipboardText(m_filePath);
}

void KNMusicSoloMenu::onActionCopyItemText()
{
    KNUtil::setClipboardText(m_itemText);
}
