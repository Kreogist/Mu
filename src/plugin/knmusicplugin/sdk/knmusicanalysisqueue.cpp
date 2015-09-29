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
#include "knmusicparser.h"

#include "knmusicanalysisqueue.h"

KNMusicAnalysisQueue::KNMusicAnalysisQueue(QObject *parent) :
    QObject(parent)
{
    //Connect analysis loop.
    connect(this, &KNMusicAnalysisQueue::analysisNext,
            this, &KNMusicAnalysisQueue::onActionAnalysisNext,
            Qt::QueuedConnection);
}

void KNMusicAnalysisQueue::addFile(const QFileInfo &fileInfo)
{
    //Add the file path to the analysis item list.
    m_filePathQueue.append(fileInfo);
    //Start analysis queue.
    emit analysisNext();
}

void KNMusicAnalysisQueue::onActionAnalysisNext()
{
    //Check the analysis queue first.
    if(m_filePathQueue.isEmpty())
    {
        return;
    }
    //Get the parser.
    KNMusicParser *parser=knMusicGlobal->parser();
    //Take the first file QFileInfo from the queue.
    QFileInfo fileInfo=m_filePathQueue.takeFirst();
    //If the suffix of the file appears in the music file.
    if(knMusicGlobal->isMusicFile(fileInfo.suffix().toLower()))
    {
        //Generate a simple analysis item.
        KNMusicAnalysisItem analysisItem;
        //Parse the file as a single music file.
        parser->parseFile(fileInfo, analysisItem);
        //Emit analysis complete signal.
        emit analysisComplete(analysisItem);
    }
    //Then, it should be a music list.
    else
    {
        //Generate a track item list.
        QList<KNMusicAnalysisItem> trackItems;
        //Parse the file as a list.
        parser->parseTrackList(fileInfo.absoluteFilePath(), trackItems);
        //Treat each the analysis item as one analysis complete.
        while(!trackItems.isEmpty())
        {
            emit analysisComplete(trackItems.takeFirst());
        }
    }
    //Ask to analysis next item.
    emit analysisNext();
}

