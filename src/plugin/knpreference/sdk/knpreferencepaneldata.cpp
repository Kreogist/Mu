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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QApplication>
#include <QFont>

#include "knpreferencepaneldata.h"

KNPreferencePanelData::KNPreferencePanelData(QObject *parent) :
    QObject(parent)
{
}

QList<PreferencePanelBlock> KNPreferencePanelData::getPanelData(
        int panelIndex)
{
    //Prepare the panel data and block data cache.
    QList<PreferencePanelBlock> panelData;
    PreferencePanelBlock block;
    //Check the panel.
    switch(panelIndex)
    {
    case PanelGeneral:
    {
        break;
    }
    case PanelLyrics:
    {
        //Get the default font.
        QVariant font=QApplication::font();
        //Header lyrics.
        block=generateBlock(tr("Header Lyrics"));
        addItem(block, "Font", tr("Header lyrics font"),
                "User/Music/MusicHeaderPlayer/Lyrics/Font", font, TypeFont,
                tr("This option will change the font used on the header player "
                   "lyrics."));
        panelData.append(block);
        //Online lyrics.
        block=generateBlock(tr("Online Lyrics"));
        addItem(block, "Enable", tr("Allow downloading lyrics"),
                "User/Music/Lyrics/Online", true, TypeBoolean,
                tr("If there is no lyrics existed in the lyrics search folder,"
                   "\nallows Mu to download lyrics from the Internet."));
        panelData.append(block);
        break;
    }
    default:
        break;
    }
    //Give back the panel data.
    return panelData;
}

inline PreferencePanelBlock KNPreferencePanelData::generateBlock(
        const QString &title)
{
    //Generate the block.
    PreferencePanelBlock block;
    //Set the name.
    block.blockName=title;
    //Give back the block.
    return block;
}

inline void KNPreferencePanelData::addItem(PreferencePanelBlock &block,
                                           const QString &name,
                                           const QString &title,
                                           const QString &path,
                                           const QVariant &defaultValue,
                                           PreferencePanelOptionType type,
                                           const QString &explain)
{
    //Generate an item.
    PreferencePanelOption option;
    //Save the data.
    option.path=path.split('/');
    option.defaultValue=defaultValue;
    option.name=name;
    option.title=title;
    option.explain=explain;
    option.type=type;
    //Add data to item.
    block.options.append(option);
}
