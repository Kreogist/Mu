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
#include <QDir>
#include <QFileInfo>

#include "knglobal.h"

#include "knmusicsearchbase.h"
#include "knmusicdetaildialogbase.h"
#include "knmusicproxymodel.h"
#include "knmusicglobal.h"

#include "knlocalemanager.h"

#include "knmusicsolomenu.h"

#include <QDebug>

using namespace KNMusic;

KNMusicSoloMenu::KNMusicSoloMenu(QWidget *parent) :
    KNMusicSoloMenuBase(parent)
{
    //Set seperator style sheet, I can't solve this bug in coding way.
    setStyleSheet("QMenu::separator {height:1px;"
                  "background: rgba(255, 255, 255, 100);"
                  "margin-left: 5px;"
                  "margin-right: 5px;}");
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    pal.setColor(QPalette::Text, QColor(255,255,255,224));
    pal.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    pal.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(pal);
    //Create Actions.
    createActions();

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicSoloMenu::retranslate);
    //Get the translation.
    retranslate();
}

void KNMusicSoloMenu::setProxyModel(KNMusicProxyModel *model)
{
    m_proxyModel=model;
}

void KNMusicSoloMenu::setCurrentIndex(const QModelIndex &itemIndex)
{
    int row=itemIndex.row();
    m_actions[PlayCurrent]->setText(
                m_actionTitles[PlayCurrent].arg(
                    m_proxyModel->itemText(row, Name)));
    m_actions[Open]->setText(
                m_actionTitles[Open].arg(
                    m_proxyModel->fileNameFromRow(row)));
    m_itemText=m_proxyModel->itemText(itemIndex.row(), itemIndex.column());
    m_filePath=m_proxyModel->filePathFromRow(row);
    //Generate the prefer name.
    m_preferFileName=generatePreferFileName(itemIndex);
    m_actions[RenameToArtistHyphonName]->setText(
                m_actionTitles[RenameToArtistHyphonName].arg(m_preferFileName));
    if(m_itemText.isEmpty())
    {
        m_actions[CopyItemText]->setVisible(false);
        m_actions[SearchItemText]->setVisible(false);
    }
    else
    {
        m_actions[CopyItemText]->setVisible(true);
        m_actions[SearchItemText]->setVisible(true);
        m_actions[CopyItemText]->setText(m_actionTitles[CopyItemText].arg(m_itemText));
        m_actions[SearchItemText]->setText(m_actionTitles[SearchItemText].arg(m_itemText));
    }
}

void KNMusicSoloMenu::setDetailDialog(KNMusicDetailDialogBase *dialog)
{
    m_detailDialog=dialog;
}

QString KNMusicSoloMenu::currentFilePath() const
{
    return m_filePath;
}

void KNMusicSoloMenu::retranslate()
{
    //Update the caption first.
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
    m_actionTitles[RenameToArtistHyphonName]=tr("Rename to %1");
    m_actionTitles[CopyFilePath]=tr("Copy location");
    m_actionTitles[CopyItemText]=tr("Copy '%1'");
    m_actionTitles[SearchItemText]=tr("Search '%1'");
    m_actionTitles[Delete]=tr("Delete");

    /*
     * Q: Why we don't need to set the data?
     * A: Because you can't change the language when you operate the menu.
     */
    //Apply the translation.
    for(int i=0; i<SoloMenuActionCount; i++)
    {
        m_actions[i]->setText(m_actionTitles[i]);
    }
}

void KNMusicSoloMenu::addMusicActions(QList<QAction *> actions)
{
    //Add the actions before custom seperator.
    insertActions(m_customSeperator, actions);
    //Add a seperator for these actions.
    insertSeparator(m_customSeperator);
}

void KNMusicSoloMenu::onActionOpenCurrentFile()
{
    KNGlobal::openLocalFile(m_filePath);
}

void KNMusicSoloMenu::onActionShowInGraphicsShell()
{
    KNGlobal::showInGraphicalShell(m_filePath);
}

void KNMusicSoloMenu::onActionCopyFilePath()
{
    KNGlobal::setClipboardText(QDir::toNativeSeparators(m_filePath));
}

void KNMusicSoloMenu::onActionCopyItemText()
{
    KNGlobal::setClipboardText(m_itemText);
}

void KNMusicSoloMenu::onActionSearchItemText()
{
    KNMusicGlobal::musicSearch()->search(m_itemText);
}

void KNMusicSoloMenu::onActionShowDetail()
{
    m_detailDialog->showDialog(m_filePath);
}

void KNMusicSoloMenu::createActions()
{
    for(int i=0; i<SoloMenuActionCount; i++)
    {
        m_actions[i]=new QAction(this);
    }
    //Play current.
    connect(m_actions[PlayCurrent], SIGNAL(triggered()),
            this, SIGNAL(requirePlayCurrent()));
    addAction(m_actions[PlayCurrent]);

    //Open.
    connect(m_actions[Open], SIGNAL(triggered()),
            this, SLOT(onActionOpenCurrentFile()));
    addAction(m_actions[Open]);

    addSeparator();

    //Get the detail info of the file.
    connect(m_actions[GetInfo], SIGNAL(triggered()),
            this, SLOT(onActionShowDetail()));
    addAction(m_actions[GetInfo]);

    //Show in graphics shell.
    connect(m_actions[ShowInGraphicShell], SIGNAL(triggered()),
            this, SLOT(onActionShowInGraphicsShell()));
    addAction(m_actions[ShowInGraphicShell]);

    addSeparator();

    connect(m_actions[SearchItemText], SIGNAL(triggered()),
            this, SLOT(onActionSearchItemText()));
    addAction(m_actions[SearchItemText]);
    m_customSeperator=m_actions[SearchItemText];

    addSeparator();

    addAction(m_actions[RenameToArtistHyphonName]);

    addSeparator();

    //Copy item text.
    connect(m_actions[CopyFilePath], SIGNAL(triggered()),
            this, SLOT(onActionCopyFilePath()));
    addAction(m_actions[CopyFilePath]);
    connect(m_actions[CopyItemText], SIGNAL(triggered()),
            this, SLOT(onActionCopyItemText()));
    addAction(m_actions[CopyItemText]);

    addSeparator();

    //Delete file.
    connect(m_actions[Delete], SIGNAL(triggered()),
            this, SIGNAL(requireRemoveCurrent()));
    addAction(m_actions[Delete]);
}

inline QString KNMusicSoloMenu::generatePreferFileName(
        const QModelIndex &itemIndex)
{
    QFileInfo currentFile(
                m_proxyModel->rowProperty(itemIndex.row(), FilePathRole).toString());
    QString preferString=m_proxyModel->itemText(itemIndex.row(), Artist) +
                         " - " +
                         m_proxyModel->itemText(itemIndex.row(), Name) +
                         "." +
                         currentFile.suffix();
//    preferString.replace(QRegExp(), "_");
    return preferString;
}
