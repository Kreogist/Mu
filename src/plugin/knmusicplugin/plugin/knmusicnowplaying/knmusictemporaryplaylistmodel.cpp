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
#include <QFileInfo>

#include "knmusicparser.h"

#include "knmusicglobal.h"

#include "knmusictemporaryplaylistmodel.h"

KNMusicTemporaryPlaylistModel::KNMusicTemporaryPlaylistModel(QObject *parent) :
    KNMusicModel(parent)
{

}

void KNMusicTemporaryPlaylistModel::appendTemporaryFiles(QStringList filePaths)
{
    //Get the parser from the music global.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Add files to temporary model.
    while(!filePaths.isEmpty())
    {
        //Process event to make GUI thread not stack.
        qApp->processEvents();
        //Take one file from the file path list.
        QFileInfo fileInfo(filePaths.takeFirst());
        //Add one file.
        if(knMusicGlobal->isMusicFile(fileInfo.suffix().toLower()))
        {
            //Generate an analysis item.
            KNMusicAnalysisItem item;
            //Parse the file.
            parser->parseFile(fileInfo, item);
            //Add the detail info to the model.
            appendRow(item.detailInfo);
        }
        else if(knMusicGlobal->isMusicListFile(fileInfo.suffix().toLower()))
        {
            //Gernerate the list of analysis item.
            QList<KNMusicAnalysisItem> items;
            //Parse the list file.
            parser->parseTrackList(fileInfo.absoluteFilePath(),
                                   items);
            //Translate the list to detail list.
            QList<KNMusicDetailInfo> detailInfos;
            //Translate that to detail info.
            for(auto i=items.begin(); i!=items.end(); ++i)
            {
                detailInfos.append((*i).detailInfo);
            }
            //Add the items to the model.
            appendRows(detailInfos);
        }
    }
}

