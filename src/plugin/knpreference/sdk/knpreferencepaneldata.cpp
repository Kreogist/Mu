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

#include "knglobal.h"
#include "knconfiguremanager.h"

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
        addItem(block, tr("Minimize to system tray"),
                "User/Global/SystemTray/MinimizeToTray", false, TypeBoolean,
                tr("When click the minimize button of the window, Mu won't "
                   "simply minimize the main window to task bar but minimize "
                   "to the system tray if this option is enabled.\n"
                   "When click the icon in the system tray, Mu will pop up "
                   "back."),
                false);
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
        addFontItem(block, tr("Application font"),
                    "User/Global/ApplicationFont", QApplication::font(),
                    tr("This option will change the font used for the whole "
                       "application.\n"
                       "This option will be applied after the application "
                       "restarted."), false, false, false);
        addItem(block, tr("Show status bar"),
                "User/Global/Appearance/ShowStatusBar", true, TypeBoolean,
                tr("Show the status bar at the top right corner."), false);
        panelData.append(block);
        //Behaviour settings.
        block=generateBlock(tr("Behaviour"));
        addItem(block, tr("Enable tooltip preview"),
                "User/Music/TooltipPreview", true, TypeBoolean,
                tr("When enable this feature and moving mouse on a song, Mu "
                   "will popup a tooltip allows to preview this song without "
                   "pausing the current song."), false);
        addItem(block, tr("Save last played"),
                "User/Music/SaveLastPlayed", true, TypeBoolean,
                tr("When next time launch Mu, Mu will restore the last playing "
                   "song state."), false);
        addIntItem(block, tr("Search delay (ms)"),
                   "User/Global/Behaviour/SearchDelay", 150,
                   tr("Mu will start to search once user change the content of "
                      "the search box.\nWhen set the delay larger than 0ms, Mu "
                      "will not start to search if user change the text less "
                      "than the delay duration."),
                   0, 500, true);
        panelData.append(block);
        //Library settings.
        block=generateBlock(tr("Library"));
        addItem(block, tr("Ignore CUE data file"),
                "User/Music/MusicLibrary/IgnoreCueData", true, TypeBoolean,
                tr("When adding the data file of one CUE file, "
                   "ignore the data file."), false);
        addItem(block, tr("Category by album artist"),
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
        //Playing parameters.
        block=generateBlock(tr("Output Parameter"));
#ifdef Q_OS_WIN64
        addItem(block, tr("Use exclusive mode WASAPI"),
                "System/Backend/WASAPI", false, TypeBoolean,
                tr("Using WASAPI with exclusive mode instead of DirectX for "
                   "sound output.\nWhen this feature is enabled, the output "
                   "sample rate will be changed to the sample rate of the real "
                   "device.\nWhen this feature is enabled, all the music "
                   "preview feature will be disabled.\nThis option will be "
                   "applied after the application restarted."), false);
#endif
#ifdef ENABLE_BACKEND_BASS
        addItem(block, tr("Use 32-bit floating-point"),
                "System/Backend/Float", true, TypeBoolean,
                tr("Using 32-bit floating-point instead of 8/16 bit for the "
                   "sample data.\nWDM drivers are required to use this feature "
                   "in Windows.\nThis option will be applied after the "
                   "application restarted."), false);
#endif
        addItem(block, tr("Use buffering"),
                "System/Backend/Buffer", true, TypeBoolean,
                tr("Enable the playback buffering.\nA playing music is "
                   "normally asked to render data to its playback buffer in "
                   "advance to produce the final signal that is given to the "
                   "output device.\nWhen this option is off, buffering is "
                   "skipped and the playing thread will only be asked to "
                   "produce data as it is needed during the generation of the "
                   "output data.\nThis option will"
                   " be applied after the application restarted."), false);
        addIntItem(block, tr("Buffer length"),
                   "System/Backend/BufferLength", 500,
                   tr("The buffer length in milliseconds.\n"
                      "Increasing the length, decreases the chance of the "
                      "sound possibly breaking-up on slower computers, \nbut "
                      "also increases the latency for DSP/FX.\nThis option "
                      "will be applied after the application restarted."),
                   10, 5000, false);
        addItem(block, tr("Force stereo output"),
                "System/Backend/Stereo", false, TypeBoolean,
                tr("Limit the output to stereo, saving some CPU if the device "
                   "has more speakers available.\nThis option will be applied "
                   "after the application restarted."), true);
        addItem(block, tr("Output audio device"),
                "System/Backend/OutputDevice", QVariant(), TypeAudioDevice,
                tr("Select the audio output device for music playing.\nThis "
                   "option will be applied after the application restarted."),
                false);
        QStringList sampleRates;
        QString defaultSampleRate=tr("Use device default sample rate");
        sampleRates << defaultSampleRate << "8000" << "11025" << "22050"
                    << "32000" << "44100" << "47250" << "48000" << "50000"
                    << "50400" << "96000" << "192000";
        addComboItem(block, tr("Device sample rate (Hz)"),
                     "System/Backend/SampleRate", defaultSampleRate,
                     tr("This option will change the output sample rate of the "
                        "audio device.\nThis option will be applied after the "
                        "application restarted."),
                     sampleRates, false, true);
        panelData.append(block);
        //Playing parameters.
        block=generateBlock(tr("Player Parameters"));
        addItem(block, tr("Auto play next song"),
                "User/Music/NowPlaying/NormalAutoNext", false, TypeBoolean,
                tr("Automatically play the next song on normal no repeat "
                   "mode."), true);
        addIntItem(block, tr("Volume level (%)"),
                   "User/Backend/VolumeLevel", 10,
                   tr("Mu supports changing volume via shortcut key or mouse "
                      "scroll wheel.\nThis option could change the volume size "
                      "increase or decrease for each shortcut pressing and "
                      "wheel scrolling.\nThe minimum size is 1%, the maximum "
                      "size is 20%."),
                   1, 20, false);
        panelData.append(block);
        break;
    }
    case PanelLibrary:
    {
        //Monitor settings.
        block=generateBlock(tr("Folder Monitor Settings"));
        ;
        panelData.append(block);
        block=generateBlock(tr("Monitored Folders"));
        ;
        panelData.append(block);
        break;
    }
    case PanelLyrics:
    {
        //Local lyrics.
        block=generateBlock(tr("Local Lyrics"));
        addItem(block, tr("Local lyrics directory"),
                "System/Music/LyricsDirPath",
                knGlobal->dirPath(KNGlobal::LibraryDir) + "/Music/Lyrics",
                TypeDirectory,
                tr("The directory that stores all the downloaded lyrics, or "
                   "any other local lyrics."), false);
        panelData.append(block);
        //Header lyrics.
        block=generateBlock(tr("Header Lyrics"));
        addFontItem(block, tr("Header lyrics font"),
                    "User/Music/MusicHeaderPlayer/Lyrics/Font",
                    QApplication::font(),
                    tr("This option will change the font used on the header "
                       "player lyrics."), true, true, false);
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
        //Global shortcut settings.
        bool useGlobalShortcut=true;
#ifdef ENABLE_GLOBAL_NULL
        //For using the null backend, disable this feature.
        useGlobalShortcut=false;
#endif
        if(useGlobalShortcut)
        {
            //Construct the global shortcut block.
            block=generateBlock(tr("Global Shortcut"));
            addItem(block, tr("Use global shortcut"),
                    "User/Shortcut/Global", false, TypeBoolean,
                    tr("Allow the shortcut key bindings to be triggered system "
                       "wide."), false);
            panelData.append(block);
        }
        //Main window controls.
        block=generateBlock(tr("Window Controls"));
        addItem(block, tr("Full Screen"),
                "User/Shortcut/FullScreen",
                QVariant::fromValue(QKeySequence(QKeySequence::FullScreen)),
                TypeShortcut,
                tr("This shortcut will be used to control the shortcut for "
                   "entering or exiting the full screen state."), false);
        panelData.append(block);
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
                   "increase amount of volume."), false);
        addItem(block, tr("Volume Down"),
                "User/Shortcut/VolumeDown",
                QVariant::fromValue(QKeySequence()),
                TypeShortcut,
                tr("This shortcut will be used in the whole application to "
                   "decrease amount of volume."), false);
        panelData.append(block);
        break;
    }
//    case PanelFileAsso:
//    {
//        //For some quick controls.
//        block=generateBlock(tr("Auto Set File Types Association"));
//        panelData.append(block);
//        block=generateBlock(tr("Lossless Compressed File Types"));
//        addFileTypeItem(block, "ape", "", QVariant(), "",
//                        ".ape", "org.kreogist.monkeysaudio-format", false);
//        addFileTypeItem(block, "caf", "", QVariant(), "",
//                        ".caf", "com.apple.coreaudio-format", false);
//        addFileTypeItem(block, "dsd", "", QVariant(), "",
//                        ".dsd", "org.kreogist.dsd-format", false);
//        addFileTypeItem(block, "flac", "", QVariant(), "",
//                        ".flac", "org.kreogist.flac-format", false);
//        addFileTypeItem(block, "tta", "", QVariant(), "",
//                        ".tta", "org.kreogist.trueaudio-format", false);
//        addFileTypeItem(block, "wav", "", QVariant(), "",
//                        ".wav", "com.microsoft.waveform-audio", false);
//        addFileTypeItem(block, "wv", "", QVariant(), "",
//                        ".wv", "org.kreogist.wavpack-audio", false);
//        panelData.append(block);
//        block=generateBlock(tr("Lossy Compressed File Types"));
//        addFileTypeItem(block, "aac", "", QVariant(), "",
//                        ".aac", "public.aac-audio", false);
//        addFileTypeItem(block, "ac3", "", QVariant(), "",
//                        ".ac3", "public.ac3-audio", false);
//        addFileTypeItem(block, "aiff", "", QVariant(), "",
//                        ".aiff", "public.aiff-audio", false);
//        addFileTypeItem(block, "mp1", "", QVariant(), "",
//                        ".mp1", "public.mp1", false);
//        addFileTypeItem(block, "mp2", "", QVariant(), "",
//                        ".mp2", "public.mp2", false);
//        addFileTypeItem(block, "mp3", "", QVariant(), "",
//                        ".mp3", "public.mp3", false);
//        addFileTypeItem(block, "mpc", "", QVariant(), "",
//                        ".mpc", "org.kreogist.musepack-format", false);
//        addFileTypeItem(block, "ofr", "", QVariant(), "",
//                        ".ofr", "org.kreogist.optimfrog-format", false);
//        addFileTypeItem(block, "opus", "", QVariant(), "",
//                        ".opus", "org.kreogist.opus-format", false);
//        addFileTypeItem(block, "spx", "", QVariant(), "",
//                        ".spx", "org.kreogist.speex-format", false);
//        panelData.append(block);
//        block=generateBlock(tr("Other File Types Association"));
//        addFileTypeItem(block, "m4a", "", QVariant(), "",
//                        ".m4a", "com.apple.m4a-audio", false);
//        addFileTypeItem(block, "mid", "", QVariant(), "",
//                        ".mid", "public.midi-audio", false);
//        addFileTypeItem(block, "midi", "", QVariant(), "",
//                        ".midi", "public.midi-audio", false);
//        addFileTypeItem(block, "ogg", "", QVariant(), "",
//                        ".ogg", "org.kreogist.ogg-format", false);
//#ifdef Q_OS_WIN
//        addFileTypeItem(block, "wma", "", QVariant(), "",
//                        ".wma", "com.microsoft.windows-media-wma", false);
//#endif
//        panelData.append(block);
//        break;
//    }
    default:
        break;
    }
    //Give back the panel data.
    return panelData;
}

inline PreferencePanelBlock KNPreferencePanelData::generateBlock(
        const QString &title, bool isAdvanced)
{
    //Generate the block.
    PreferencePanelBlock block;
    //Set the name.
    block.blockName=title;
    block.advanced=isAdvanced;
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

inline void KNPreferencePanelData::addFileTypeItem(PreferencePanelBlock &block,
                                                   const QString &title,
                                                   const QString &path,
                                                   const QVariant &defaultValue,
                                                   const QString &explain,
                                                   const QString &suffix,
                                                   const QString &typeHandler,
                                                   bool isAdvanced)
{
    //Generate an item.
    PreferencePanelOption option;
    //Update the item data.
    setItemData(option, title, path, defaultValue, explain, TypeFileType,
                isAdvanced);
    //Set the preference.
    QJsonObject config;
    config.insert("suffix", suffix);
    config.insert("handler", typeHandler);
    option.configure=config;
    //Add data to item.
    block.options.append(option);
}

inline void KNPreferencePanelData::addFontItem(PreferencePanelBlock &block,
                                               const QString &title,
                                               const QString &path,
                                               const QVariant &defaultValue,
                                               const QString &explain,
                                               bool showSize,
                                               bool showAttribute,
                                               bool isAdvanced)
{
    //Generate an item.
    PreferencePanelOption option;
    //Update the item data.
    setItemData(option, title, path, defaultValue, explain, TypeFont,
                isAdvanced);
    //Set the preference.
    QJsonObject config;
    config.insert("size", showSize);
    config.insert("attribute", showAttribute);
    option.configure=config;
    //Add data to item.
    block.options.append(option);
}

inline void KNPreferencePanelData::addIntItem(PreferencePanelBlock &block,
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
