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
#include <QAction>
#include <QBoxLayout>

#include "kndropproxycontainer.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibrarytreeview.h"
#include "knmusicsolomenubase.h"

#include "knlocalemanager.h"

#include "knmusiclibrarysongtab.h"

KNMusicLibrarySongTab::KNMusicLibrarySongTab(QObject *parent) :
    KNMusicLibraryTab(parent)
{
    //Initial the drop proxy.
    m_dropProxy=new KNDropProxyContainer;
    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, m_dropProxy);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_dropProxy->setLayout(mainLayout);

    //Initial the library treeview.
    m_treeview=new KNMusicLibraryTreeView(m_dropProxy);
    mainLayout->addWidget(m_treeview);

    //Initial the show in tab.
    initialShowInAction();
    //Connect retranslate request.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicLibrarySongTab::retranslate);
    //Retranslate.
    retranslate();
}

KNMusicLibrarySongTab::~KNMusicLibrarySongTab()
{
    //Recover widget if there's no parent.
    if(m_dropProxy->parent()==0)
    {
        delete m_dropProxy;
    }
}

QAction *KNMusicLibrarySongTab::showInAction()
{
    return m_showInSongTab;
}

QString KNMusicLibrarySongTab::caption()
{
    return tr("Songs");
}

QPixmap KNMusicLibrarySongTab::icon()
{
    return QPixmap(":/plugin/music/category/01_musics.png");
}

QWidget *KNMusicLibrarySongTab::widget()
{
    return m_dropProxy;
}

void KNMusicLibrarySongTab::retranslate()
{
    m_showInSongTab->setText(tr("Go to Songs"));
}

void KNMusicLibrarySongTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Save the music library model.
    m_musicLibrary=model;
    //Set the music model.
    m_treeview->setMusicModel(m_musicLibrary);
    //Connect analysis requirement signal.
    connect(m_dropProxy, &KNDropProxyContainer::requireAnalysisFiles,
            m_musicLibrary, &KNMusicLibraryModel::addFiles);
    //Reset the header state.
    m_treeview->resetHeaderState();
    //Set default sort state.
    m_treeview->sortByColumn(Name, Qt::AscendingOrder);
}

void KNMusicLibrarySongTab::onActionSearch(const QString &text)
{
    m_treeview->searchText(text);
}

void KNMusicLibrarySongTab::onActionShowInSong()
{
    //Get the row of the file.
    int musicRow=m_musicLibrary->rowFromFilePath(KNMusicGlobal::soloMenu()->currentFilePath());
    //If the row is available.
    if(musicRow!=-1)
    {
        //Scroll to the song.
        m_treeview->scrollToSourceSongRow(musicRow);
        //Ask to show current tab.
        emit requireShowTab();
    }
}

void KNMusicLibrarySongTab::initialShowInAction()
{
    //Initial the show in actions.
    m_showInSongTab=new QAction(this);
    connect(m_showInSongTab, SIGNAL(triggered()),
            this, SLOT(onActionShowInSong()));
}
