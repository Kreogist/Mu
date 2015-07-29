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
#include <QStringList>

#include "knlocalemanager.h"

#include "knmusicglobal.h"

#include "knmusicsearchsyntaxengine.h"

#include <QDebug>

KNMusicSearchSyntaxEngine::KNMusicSearchSyntaxEngine(QObject *parent) :
    QObject(parent)
{
    //Link with the locale manager.
    knI18n->link(this, &KNMusicSearchSyntaxEngine::retranslate);
    retranslate();
}

QList<KNMusicSearchBlock> KNMusicSearchSyntaxEngine::parseSearch(
        const QString &text)
{
    //Generate a search block list.
    QList<KNMusicSearchBlock> blockList;
    //Check the text is empty or not first.
    if(text.isEmpty())
    {
        return blockList;
    }

    //Split the text with the split word to get all the possible blocks.
    QStringList blockDataList=text.split(", ", QString::SkipEmptyParts);
    //Check all the blocks.
    for(auto i=blockDataList.begin(); i!=blockDataList.end(); ++i)
    {
        //Generate a search block.
        KNMusicSearchBlock block;
        //Find the '|' character(splitter) in the text.
        int splitterPosition=(*i).indexOf('|');
        //If we cannot find the splitter, then means this is not a block.
        if(splitterPosition==-1)
        {
            //Add the code as a normal block.
            block.index=-1;
            block.value=(*i).simplified();
            //Add the block to block list.
            blockList.append(block);
            //Continue to next block.
            continue;
        }
        //If there's a splitter, get the column or the property(we are calling
        //it target) first, tried to find it in the hash list.
        int targetIndex=m_columnMap.value(
                    (*i).left(splitterPosition).simplified().toLower(),
                    -1);
        bool columnFind=(targetIndex!=-1);

        //Check the result, if we cannot find it in the column map, then tried
        //to find it in the property map.
        if(!columnFind)
        {
            //!FIXME: add property check code here.
            //Add the code as a normal block.
            block.index=-1;
            block.value=(*i).simplified();
            //Add the block to block list.
            blockList.append(block);
            //Continue to next block.
            continue;
        }
        //Set the index, isColumn data.
        block.index=targetIndex;
        block.isColumn=columnFind;
        //Now parse the search value data.
        block.value=textToVariant(targetIndex, (*i).mid(splitterPosition+1));
        //Add this block to the block list.
        blockList.append(block);
    }
    //Give back the block list.
    return blockList;
}

void KNMusicSearchSyntaxEngine::retranslate()
{
    //Clear the target map.
    m_columnMap.clear();
    //Insert the column into the map.
    for(int i=0; i<MusicColumnCount; ++i)
    {
        m_columnMap.insert(knMusicGlobal->treeViewHeaderText(i).toLower(),
                           i);
    }
    //Update the splitter.
    m_splitter.setPattern(tr(", "));
}

QVariant KNMusicSearchSyntaxEngine::textToVariant(const int &column,
                                                  QString text)
{
    //Ignore the empty data.
    if(text.isEmpty())
    {
        return QVariant();
    }
    //Process the column.
    switch(column)
    {
    default:
        //Simply treat it as variant.
        return QVariant(text.simplified());
    }
}

