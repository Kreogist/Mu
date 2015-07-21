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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>

#include "knmousesensewidget.h"

#include "knmusicplaylisttreeview.h"
#include "knmusicplaylistmodel.h"

#include "knmusicplaylistviewer.h"

KNMusicPlaylistViewer::KNMusicPlaylistViewer(QWidget *parent) :
    QWidget(parent),
    m_treeView(new KNMusicPlaylistTreeView(this))
{
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Debug
    KNMusicPlaylistModel *testModel=new KNMusicPlaylistModel(this);
    KNMusicDetailInfo t;
    t.textLists[Name]="Cutie Panther";
    t.textLists[Artist]="BiBi";
    testModel->appendRow(t);
    QList<KNMusicDetailInfo> infoList;
    for(int i=0; i<20; i++)
    {
        t.textLists[AlbumArtist]=QString::number(i);
        infoList.append(t);
    }
    t.textLists[Name]="Music S.T.A.R.T!!";
    testModel->appendRow(t);
    testModel->removeRow(0);
    testModel->appendRows(infoList);
    m_treeView->setModel(testModel);

    //Initial the information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->updateObjectName("PlaylistInformationContainer");
    infoContainer->setContentsMargins(20, 12, 0, 8);
    //Add widget to layout.
    mainLayout->addWidget(infoContainer);
    mainLayout->addWidget(m_treeView, 1);
    //Initial the information container layout.
    QBoxLayout *informationLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                 infoContainer);
    informationLayout->setContentsMargins(0,0,0,0);
    infoContainer->setLayout(informationLayout);
}

