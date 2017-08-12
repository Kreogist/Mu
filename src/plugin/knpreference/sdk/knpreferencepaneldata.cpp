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
#include <QJsonObject>
#include <QJsonArray>
#include <QKeySequence>

#include "knpreferencepaneldata.h"

#include <QDebug>

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
#ifdef Q_OS_WIN
        //System Tray Settings.
        block=generateBlock(tr("System Tray Icon"));
        addItem(block, tr("Close to system tray"),
                "User/Global/SystemTray/CloseToTray", false, TypeBoolean,
                tr("When click the close button of the window, Mu won't quit "
                   "but minimize to the system tray.\n"
                   "When click the icon in the system tray, Mu will pop up "
                   "back."),
                false);
        panelData.append(block);
#endif
        //Appearance settings.
        block=generateBlock(tr("Appearance"));
        addItem(block, tr("Show status bar"),
                "User/Global/Appearance/ShowStatusBar", true, TypeBoolean,
                tr("Show the status bar at the top right corner."), false);
        panelData.append(block);
        //Library settings.
        block=generateBlock(tr("Library"));
        addItem(block, tr("Ignore CUE Data File"),
                "User/Music/MusicLibrary/IgnoreCueData", true, TypeBoolean,
                tr("When adding the data file of one CUE file, "
                   "ignore the data file."), false);
        addItem(block, tr("Category by Album Artist"),
                "User/Music/MusicLibrary/UseAlbumArt", true, TypeBoolean,
                tr("When category the album, use the album artist metadata "
                   "field instead of statistic all the artists.\n"
                   "This option will be applied after the application "
                   "restarted."), true);
        panelData.append(block);
        break;
    }
    case PanelPlayback:
    {
        //Header lyrics.
        block=generateBlock(tr("Output Parameter"));
#ifdef Q_OS_WIN64
        addItem(block, tr("Use WASAPI"),
                "System/Backend/WASAPI", false, TypeBoolean,
                tr("Using WASAPI with exclusive mode instead of DirectX for "
                   "sound output.\nWhen this feature is enabled, the output "
                   "sample rate will be changed to the sample rate of the real "
                   "device.\nWhen this feature is enabled, all the music "
                   "preview feature will be disabled.\nThis option will be "
                   "applied after the application restarted."), false);
#endif
        addItem(block, tr("Use 32-bit float point"),
                "System/Backend/Float", true, TypeBoolean,
                tr("Produce 32-bit floating-point output.\nWDM drivers are "
                   "required to use this feature in Windows.\nThis option will"
                   " be applied after the application restarted."), false);
        addItem(block, tr("Stereo Output"),
                "System/Backend/Stero", false, TypeBoolean,
                tr("Limit the output to stereo, saving some CPU if the device "
                   "has more speakers available.\nThis option will be applied "
                   "after the application restarted."), true);
        QStringList sampleRates;
        QString defaultSampleRate=tr("Use Device Default Sample Rate");
        sampleRates << defaultSampleRate << "8000" << "11025" << "22050"
                    << "32000" << "44100" << "47250" << "48000" << "50000"
                    << "50400" << "96000" << "192000";
        addComboItem(block, tr("Sample Rate (Hz)"),
                     "System/Backend/SampleRate", defaultSampleRate,
                     tr("This option will change the output sample rate of the "
                        "playback backend.\nThis option will be applied after "
                        "the application restarted."),
                     sampleRates, false, true);
        panelData.append(block);
        break;
    }
    case PanelLyrics:
    {
        //Get the default font.
        QVariant font=QApplication::font();
        //Header lyrics.
        block=generateBlock(tr("Header Lyrics"));
        addItem(block, tr("Header lyrics font"),
                "User/Music/MusicHeaderPlayer/Lyrics/Font", font, TypeFont,
                tr("This option will change the font used on the header player "
                   "lyrics."), false);
        addIntItem(block, tr("Header lyrics spacing"),
                   "User/Music/MusicHeaderPlayer/Lyrics/Spacing", 2,
                   tr("This option will change the spacing of the text line on "
                      "the header player lyrics."), 1, 20, true);
        panelData.append(block);
        //Online lyrics.
        block=generateBlock(tr("Online Lyrics"));
        addItem(block, tr("Allow downloading lyrics"),
                "User/Music/Lyrics/Online/Enable", true, TypeBoolean,
                tr("If there is no lyrics existed in the lyrics search folder,"
                   "\nallows Mu to download lyrics from the Internet."), false);
        addItem(block, tr("Combine translation"),
                "User/Music/Lyrics/Online/CombineTranslation", false,
                TypeBoolean,
                tr("Some lyrics downloader will provide translated version. \n"
                   "This option will allow Mu to combine the translated version"
                   " and the raw lyrics into a single file."), false);
        panelData.append(block);
        break;
    }
    case PanelShortcuts:
    {
        //Playback controls.
        block=generateBlock(tr("Playing Controls"));
        addItem(block, tr("Play and Pause"),
                "User/Shortcut/PlayNPause",
                QVariant::fromValue(QKeySequence(Qt::Key_F8,
                                                 Qt::Key_MediaTogglePlayPause)),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "play or pause the current playing music."), false);
        addItem(block, tr("Previous Song"),
                "User/Shortcut/Previous",
                QVariant::fromValue(QKeySequence(Qt::Key_F7,
                                                 Qt::Key_MediaPrevious)),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "switch the current playing music to the previous song."),
                false);
        addItem(block, tr("Next Song"),
                "User/Shortcut/Next",
                QVariant::fromValue(QKeySequence(Qt::Key_F9,
                                                 Qt::Key_MediaNext)),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "switch the current playing music to the next song."),
                false);
        panelData.append(block);
        //Volume controls.
        block=generateBlock(tr("Volume Controls"));
        addItem(block, tr("Mute"),
                "User/Shortcut/Mute",
                QVariant::fromValue(QKeySequence()),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "enter and exit the mute state."), false);
        addItem(block, tr("Volume Up"),
                "User/Shortcut/VolumeUp",
                QVariant::fromValue(QKeySequence()),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "increase 10% volume."), false);
        addItem(block, tr("Volume Down"),
                "User/Shortcut/VolumeDown",
                QVariant::fromValue(QKeySequence()),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "decrease 10% volume."), false);
        panelData.append(block);
        break;
    }
    case PanelFileAsso:
    {
        //For some quick controls.
        block=generateBlock(tr("Auto Set File Association"));
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
                                           const QString &title,
                                           const QString &path,
                                           const QVariant &defaultValue,
                                           PreferencePanelOptionType type,
                                           const QString &explain,
                                           bool isAdvanced)
{
    //Generate an item.
    PreferencePanelOption option;
    //Update the item data.
    setItemData(option, title, path, defaultValue, explain, type, isAdvanced);
    //No preference will be added.
    //Add data to item.
    block.options.append(option);
}

void KNPreferencePanelData::addIntItem(PreferencePanelBlock &block,
                                       const QString &title,
                                       const QString &path,
                                       const QVariant &defaultValue,
                                       const QString &explain,
                                       int min, int max, bool isAdvanced)
{
    //Generate an item.
    PreferencePanelOption option;
    //Update the item data.
    setItemData(option, title, path, defaultValue, explain, TypeInt,
                isAdvanced);
    //Set the preference.
    QJsonObject config;
    config.insert("min", min);
    config.insert("max", max);
    option.configure=config;
    //Add data to item.
    block.options.append(option);
}

inline void KNPreferencePanelData::addComboItem(PreferencePanelBlock &block,
                                                const QString &title,
                                                const QString &path,
                                                const QVariant &defaultValue,
                                                const QString &explain,
                                                const QStringList &candidates,
                                                bool allowEdit,
                                                bool isAdvanced)
{
    //Generate an item.
    PreferencePanelOption option;
    //Update the item data.
    setItemData(option, title, path, defaultValue, explain, TypeCombo,
                isAdvanced);
    //Set the preference.
    QJsonObject config;
    config.insert("edit", allowEdit);
    //Translate the string list to json array.
    QJsonArray candidateList;
    for(auto i : candidates)
    {
        //Add the item to the candidate json array.
        candidateList.append(i);
    }
    config.insert("candidates", candidateList);
    option.configure=config;
    //Add data to item.
    block.options.append(option);
}

inline void KNPreferencePanelData::setItemData(PreferencePanelOption &option,
                                               const QString &title,
                                               const QString &path,
                                               const QVariant &defaultValue,
                                               const QString &explain,
                                               PreferencePanelOptionType type,
                                               bool isAdvanced)
{
    //Save the data.
    option.title=title;
    option.path=path.split('/');
    option.defaultValue=defaultValue;
    option.explain=explain;
    option.advanced=isAdvanced;
    //Set the type of the item.
    option.type=type;
}
