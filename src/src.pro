# Copyright (C) Kreogist Dev Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

# Basic informations about the main mu project.
TEMPLATE = app
TARGET = mu
INSTALLS += target

# Add Qt modules, the Qt major version should be greater than 5.
QT += \
    core \
    gui \
    widgets \
    xml \
    network

# Enabled C++ 11 configures.
CONFIG += c++11

# Add translation paths for Qt Linguist to generate translation files.
TRANSLATIONS += \
    i18n/Simplified_Chinese.ts \
    i18n/Traditional_Chinese.ts

# Enabled processor instruction sets compile switches when using release mode
# for different compilers.
msvc: {
    QMAKE_CXXFLAGS_RELEASE += -GL -Qpar -Qpar-report:1  -favor:INTEL64
}
gcc: {
    CONFIG += mmx sse sse2 sse3
    QMAKE_CXXFLAGS_RELEASE += -mmmx -msse -msse2 -msse3 -finline-functions
    # Qt will use clang on Mac OS X, but it seems that it treats the clang as
    # gcc, it will use all the flags we set to gcc on clang. In clang, it
    # doesn't have -fforce-addr switch, so we need to set the switcher
    # especially for Windows and Linux.
    win32:{
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
    }
    linux:{
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
    }
}

# Platform Specific Configuration.
win32 : {
    # Application icon.
    RC_FILE += resource/icon/win_resource.rc \
               resource/icon/mu.ico
    ICON = resource/icon/mu.ico
    # Enable the backend and analysiser.
    CONFIG += backend-bass analysiser-ffmpeg
}

macx: {
    # Application icon.
    RC_FILE += resource/icon/mu.icns
    ICON = resource/icon/mu.icns
    # Enable the backend and analysiser.
    CONFIG += backend-bass analysiser-ffmpeg
    # Nearly all the audio library will use CoreAudio on Mac OS X, so import
    # CoreAudio library to LFLAGS and LIBS.
    QMAKE_LFLAGS += -framework CoreFoundation
    LIBS += -framework CoreFoundation
    # Brew configure. Use brew to install all your libs, e.g. FFMpeg.
    INCLUDEPATH += /usr/local/include/
    LIBS += -L/usr/local/lib/
}

# Backend Specific Configuration
backend-bass: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the backend enabled flag.
    DEFINES += ENABLE_BACKEND_BASS BACKEND_ENABLED
    # Add backend library to the project.
    LIBS += -lbass
    # Add backend files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.h \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.h
}

# Analysiser Specific Configuration
analysiser-ffmpeg: {
    # Add libraries.
    LIBS += -lavformat -lavcodec -lavutil
    # For Mac OS X, we have to enabled swresample and swscale.
    macx: {
        LIBS += -lswresample -lswscale
    }
    # Define the ffmpeg enabled flag.
    DEFINES += ENABLED_FFMPEG_ANALYSISER
    # Add analysiser files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicffmpeganalysiser/knmusicffmpeganalysiser.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicffmpeganalysiser/knmusicffmpeganalysiser.h
}

# Add sdk directory to include path.
INCLUDEPATH += \
sdk \
plugin/knmusicplugin/sdk/

# Source and Headers.
SOURCES += \
    main.cpp \
    sdk/knsingletonapplication.cpp \
    core/knpluginmanager.cpp \
    sdk/knmainwindow.cpp \
    sdk/knglobal.cpp \
    sdk/knconfiguremanager.cpp \
    sdk/knconfigure.cpp \
    sdk/knutil.cpp \
    sdk/knfontmanager.cpp \
    sdk/knlocalemanager.cpp \
    sdk/knthememanager.cpp \
    sdk/knmainwindowcontainer.cpp \
    core/knmainwindowheader.cpp \
    core/knmainwindowiconbutton.cpp \
    sdk/knwidgetswitcher.cpp \
    sdk/knhwidgetswitcher.cpp \
    plugin/knpreference/knpreference.cpp \
    plugin/knmusicplugin/knmusicplugin.cpp \
    plugin/knmusicplugin/sdk/knmusicglobal.cpp \
    plugin/knpreference/knpreferencesidebar.cpp \
    plugin/knpreference/knpreferencetitlebar.cpp \
    plugin/knpreference/knpreferenceheaderbutton.cpp \
    sdk/knsideshadowwidget.cpp \
    sdk/knlinearsensewidget.cpp \
    sdk/knvwidgetswitcher.cpp \
    sdk/knpreferenceitem.cpp \
    sdk/knshadowscrollarea.cpp \
    plugin/knpreference/knpreferenceitemlist.cpp \
    sdk/sao/knsaostyle.cpp \
    plugin/knpreference/knpreferenceabout.cpp \
    sdk/kncategorytab.cpp \
    sdk/kncategorytabbar.cpp \
    sdk/knabstracttabgroup.cpp \
    sdk/knmessagebox.cpp \
    sdk/sao/knmessageboxblock.cpp \
    sdk/sao/knmessageboxcontent.cpp \
    sdk/knopacityanimebutton.cpp \
    plugin/knmusicplugin/sdk/knmusicdetaildialog.cpp \
    sdk/knhtabgroup.cpp \
    plugin/knmusicplugin/plugin/knmusicdetailpaneloverview/knmusicdetailpaneloverview.cpp \
    sdk/kncircleiconbutton.cpp \
    plugin/knmusicplugin/sdk/knmusicparser.cpp \
    sdk/knmousedetectheader.cpp \
    plugin/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.cpp \
    plugin/knmusicplugin/plugin/knmusictagid3v2/knmusictagid3v2.cpp \
    sdk/knanimationmenu.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/knmusicplaylist.cpp \
    plugin/knmusicplugin/sdk/knmusictreeviewbase.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylisttreeview.cpp \
    sdk/knemptystatewidget.cpp \
    sdk/kndropproxycontainer.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistemptyhint.cpp \
    sdk/knglassbutton.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlist.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistviewer.cpp \
    plugin/knmusicplugin/sdk/knmusiccategorylistviewbase.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistview.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistdelegate.cpp \
    sdk/knmousesensewidget.cpp \
    plugin/knmusicplugin/sdk/knmusicmodel.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistmodel.cpp \
    plugin/knmusicplugin/sdk/knmusictreeviewheader.cpp \
    sdk/knmousesenseheader.cpp \
    sdk/knjsondatabase.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistmanager.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistengine.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistmodel.cpp \
    plugin/knmusicplugin/sdk/knmusicproxymodel.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistindexdelegate.cpp \
    plugin/knmusicplugin/sdk/knmusicratingdelegate.cpp \
    plugin/knmusicplugin/sdk/knmusicratingeditor.cpp \
    sdk/knconnectionhandler.cpp \
    sdk/knsearchbox.cpp \
    plugin/knmusicplugin/plugin/knmusicsearch/knmusicsearch.cpp \
    plugin/knmusicplugin/plugin/knmusicsearch/knmusicsearchsyntaxengine.cpp \
    plugin/knmusicplugin/sdk/knmusictreeviewheadermenu.cpp \
    plugin/knmusicplugin/plugin/knmusicnowplaying/knmusicnowplaying.cpp \
    plugin/knmusicplugin/plugin/knmusicnowplaying/knmusictemporaryplaylistmodel.cpp \
    sdk/knfilesearcher.cpp \
    plugin/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.cpp \
    plugin/knmusicplugin/plugin/knmusictagapev2/knmusictagapev2.cpp \
    plugin/knmusicplugin/sdk/knmusicsearcher.cpp \
    plugin/knmusicplugin/sdk/knmusicanalysisqueue.cpp \
    plugin/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayer.cpp \
    sdk/knhighlightlabel.cpp \
    sdk/knscrolllabel.cpp \
    sdk/kngraphicsgloweffect.cpp \
    sdk/knabstractslider.cpp \
    sdk/knprogressslider.cpp \
    sdk/kneditablelabel.cpp \
    sdk/kncancellineedit.cpp \
    sdk/knopacitybutton.cpp \
    sdk/knvolumeslider.cpp \
    plugin/knmusicplugin/sdk/knmusicstandardbackend.cpp \
    plugin/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltip.cpp \
    plugin/knmusicplugin/sdk/knmusicalbumlabel.cpp \
    plugin/knmusicplugin/sdk/knmusicvscrolllyrics.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistxspfparser/knmusicplaylistxspfparser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistwplparser/knmusicplaylistwplparser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistm3uparser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistm3u8parser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistextm3uparser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistttplparser/knmusicplaylistttplparser.cpp \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistitunesxmlparser/knmusicplaylistitunesxmlparser.cpp \
    plugin/knmusicplugin/plugin/knmusicmultimenu/knmusicmultimenu.cpp \
    plugin/knmusicplugin/plugin/knmusiccuelistparser/knmusiccuelistparser.cpp \
    plugin/knmusicplugin/plugin/knmusicdetailpanelartwork/knmusicdetailpanelartwork.cpp \
    plugin/knmusicplugin/sdk/knmusicscrolllyrics.cpp \
    plugin/knmusicplugin/sdk/knmusiclyricsbackend.cpp \
    plugin/knmusicplugin/sdk/knmusiclyricsmanager.cpp \
    plugin/knmusicplugin/plugin/knmusictagflac/knmusictagflac.cpp \
    plugin/knmusicplugin/sdk/knmusiclrcparser.cpp \
    plugin/knmusicplugin/sdk/knmusiconlinelyrics.cpp \
    plugin/knmusicplugin/sdk/knmusiclyricsdownloader.cpp \
    plugin/knmusicplugin/plugin/knmusicqqlyrics/knmusicqqlyrics.cpp \
    plugin/knmusicplugin/plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.cpp \
    plugin/knmusicplugin/plugin/knmusicttpodlyrics/knmusicttpodlyrics.cpp \
    plugin/knmusicplugin/plugin/knmusicxiamilyrics/knmusicxiamilyrics.cpp \
    sdk/sao/knsaosubmenu.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayer.cpp \
    plugin/knmusicplugin/sdk/knmusicutil.cpp

HEADERS += \
    sdk/knsingletonapplication.h \
    core/knpluginmanager.h \
    sdk/knversion.h \
    sdk/knmainwindow.h \
    sdk/knglobal.h \
    sdk/knconfiguremanager.h \
    sdk/knconfigure.h \
    sdk/knutil.h \
    sdk/knfontmanager.h \
    sdk/knlocalemanager.h \
    sdk/knthememanager.h \
    sdk/knmainwindowcontainer.h \
    sdk/knmainwindowheaderbase.h \
    core/knmainwindowheader.h \
    core/knmainwindowiconbutton.h \
    sdk/knwidgetswitcher.h \
    sdk/knhwidgetswitcher.h \
    sdk/kncategoryplugin.h \
    sdk/knpreferenceplugin.h \
    plugin/knpreference/knpreference.h \
    sdk/knabstractmusicplugin.h \
    plugin/knmusicplugin/knmusicplugin.h \
    plugin/knmusicplugin/sdk/knmusicglobal.h \
    plugin/knpreference/knpreferencesidebar.h \
    plugin/knpreference/knpreferencetitlebar.h \
    plugin/knpreference/knpreferenceheaderbutton.h \
    sdk/knsideshadowwidget.h \
    sdk/knlinearsensewidget.h \
    sdk/knvwidgetswitcher.h \
    sdk/knpreferenceitem.h \
    sdk/knshadowscrollarea.h \
    plugin/knpreference/knpreferenceitemlist.h \
    sdk/sao/knsaostyle.h \
    plugin/knpreference/knpreferenceabout.h \
    sdk/kncategorytab.h \
    sdk/kncategorytabbar.h \
    sdk/knabstracttabgroup.h \
    sdk/knmessagebox.h \
    sdk/sao/knmessageboxblock.h \
    sdk/sao/knmessageboxcontent.h \
    sdk/knopacityanimebutton.h \
    plugin/knmusicplugin/sdk/knmusicdetaildialog.h \
    plugin/knmusicplugin/sdk/knmusicdetaildialogpanel.h \
    sdk/knhtabgroup.h \
    plugin/knmusicplugin/plugin/knmusicdetailpaneloverview/knmusicdetailpaneloverview.h \
    sdk/kncircleiconbutton.h \
    plugin/knmusicplugin/sdk/knmusicparser.h \
    plugin/knmusicplugin/sdk/knmusicutil.h \
    plugin/knmusicplugin/sdk/knmusicanalysiser.h \
    plugin/knmusicplugin/sdk/knmusictagparser.h \
    plugin/knmusicplugin/sdk/knmusiclistparser.h \
    sdk/knmousedetectheader.h \
    plugin/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.h \
    plugin/knmusicplugin/plugin/knmusictagid3v2/knmusictagid3v2.h \
    sdk/knanimationmenu.h \
    plugin/knmusicplugin/sdk/knmusicsolomenubase.h \
    plugin/knmusicplugin/sdk/knmusictab.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/knmusicplaylist.h \
    plugin/knmusicplugin/sdk/knmusicplaylistbase.h \
    plugin/knmusicplugin/sdk/knmusictreeviewbase.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylisttreeview.h \
    sdk/knemptystatewidget.h \
    sdk/kndropproxycontainer.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistemptyhint.h \
    sdk/knglassbutton.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlist.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistviewer.h \
    plugin/knmusicplugin/sdk/knmusiccategorylistviewbase.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistview.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistdelegate.h \
    sdk/knmousesensewidget.h \
    plugin/knmusicplugin/sdk/knmusicmodel.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistmodel.h \
    plugin/knmusicplugin/sdk/knmusictreeviewheader.h \
    sdk/knmousesenseheader.h \
    sdk/knjsondatabase.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistmanager.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistengine.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistlistmodel.h \
    plugin/knmusicplugin/sdk/knmusicproxymodel.h \
    plugin/knmusicplugin/sdk/knmusicnowplayingbase.h \
    plugin/knmusicplugin/sdk/knmusicbackend.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/sdk/knmusicplaylistindexdelegate.h \
    plugin/knmusicplugin/sdk/knmusicratingdelegate.h \
    plugin/knmusicplugin/sdk/knmusicratingeditor.h \
    sdk/knconnectionhandler.h \
    sdk/knsearchbox.h \
    plugin/knmusicplugin/sdk/knmusicsearchbase.h \
    plugin/knmusicplugin/plugin/knmusicsearch/knmusicsearch.h \
    plugin/knmusicplugin/plugin/knmusicsearch/knmusicsearchsyntaxengine.h \
    plugin/knmusicplugin/sdk/knmusictreeviewheadermenu.h \
    plugin/knmusicplugin/plugin/knmusicnowplaying/knmusicnowplaying.h \
    plugin/knmusicplugin/plugin/knmusicnowplaying/knmusictemporaryplaylistmodel.h \
    sdk/knfilesearcher.h \
    plugin/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.h \
    plugin/knmusicplugin/sdk/knmusicplayerbase.h \
    plugin/knmusicplugin/plugin/knmusictagapev2/knmusictagapev2.h \
    plugin/knmusicplugin/sdk/knmusicsearcher.h \
    plugin/knmusicplugin/sdk/knmusicanalysisqueue.h \
    plugin/knmusicplugin/sdk/knmusicheaderplayerbase.h \
    plugin/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayer.h \
    sdk/knhighlightlabel.h \
    sdk/knscrolllabel.h \
    sdk/kngraphicsgloweffect.h \
    sdk/knabstractslider.h \
    sdk/knprogressslider.h \
    sdk/kneditablelabel.h \
    sdk/kncancellineedit.h \
    sdk/knopacitybutton.h \
    sdk/knvolumeslider.h \
    plugin/knmusicplugin/sdk/knmusicstandardbackend.h \
    plugin/knmusicplugin/sdk/knmusicstandardbackendthread.h \
    plugin/knmusicplugin/sdk/knmusicdetailtooltipbase.h \
    plugin/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltip.h \
    plugin/knmusicplugin/sdk/knmusicalbumlabel.h \
    plugin/knmusicplugin/sdk/knmusiclyricsbase.h \
    plugin/knmusicplugin/sdk/knmusicvscrolllyrics.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistxspfparser/knmusicplaylistxspfparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistwplparser/knmusicplaylistwplparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistm3uparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistm3u8parser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistm3uparser/knmusicplaylistextm3uparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistttplparser/knmusicplaylistttplparser.h \
    plugin/knmusicplugin/plugin/knmusicplaylist/plugin/knmusicplaylistitunesxmlparser/knmusicplaylistitunesxmlparser.h \
    plugin/knmusicplugin/sdk/knmusicmultimenubase.h \
    plugin/knmusicplugin/plugin/knmusicmultimenu/knmusicmultimenu.h \
    plugin/knmusicplugin/plugin/knmusiccuelistparser/knmusiccuelistparser.h \
    plugin/knmusicplugin/plugin/knmusicdetailpanelartwork/knmusicdetailpanelartwork.h \
    plugin/knmusicplugin/sdk/knmusicscrolllyrics.h \
    plugin/knmusicplugin/sdk/knmusiclyricsbackend.h \
    plugin/knmusicplugin/sdk/knmusiclyricsmanager.h \
    plugin/knmusicplugin/plugin/knmusictagflac/knmusictagflac.h \
    plugin/knmusicplugin/sdk/knmusiclrcparser.h \
    plugin/knmusicplugin/sdk/knmusiconlinelyrics.h \
    plugin/knmusicplugin/sdk/knmusiclyricsdownloader.h \
    plugin/knmusicplugin/plugin/knmusicqqlyrics/knmusicqqlyrics.h \
    plugin/knmusicplugin/plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.h \
    plugin/knmusicplugin/plugin/knmusicttpodlyrics/knmusicttpodlyrics.h \
    plugin/knmusicplugin/plugin/knmusicxiamilyrics/knmusicxiamilyrics.h \
    sdk/sao/knsaosubmenu.h \
    plugin/knmusicplugin/sdk/knmusicmainplayerbase.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayer.h

RESOURCES += \
    resource/res.qrc

DISTFILES +=
