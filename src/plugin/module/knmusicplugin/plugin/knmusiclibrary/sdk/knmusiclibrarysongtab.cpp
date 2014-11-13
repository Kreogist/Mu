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
#include <QBoxLayout>

#include "kndropproxycontainer.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibrarytreeview.h"

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
}

KNMusicLibrarySongTab::~KNMusicLibrarySongTab()
{
    //Recover widget if there's no parent.
    if(m_dropProxy->parent()==0)
    {
        delete m_dropProxy;
    }
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

void KNMusicLibrarySongTab::setLibraryModel(KNMusicLibraryModel *model)
{
    //Set the music model.
    m_treeview->setMusicModel(model);
    //Connect analysis requirement signal.
    connect(m_dropProxy, &KNDropProxyContainer::requireAnalysisFiles,
            model, &KNMusicLibraryModel::addFiles);
    //Reset the header state.
    m_treeview->resetHeaderState();
}

void KNMusicLibrarySongTab::onActionSearch(const QString &text)
{
    m_treeview->searchText(text);
}
