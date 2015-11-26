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
    i18n/Traditional_Chinese.ts \
    i18n/Japanese.ts

# Enabled processor instruction sets compile switches when using release mode
# for different compilers.
msvc: {
    # MSVC will only used for Windows 64-bit version.
    # Whole Program Optimization
    QMAKE_CXXFLAGS_RELEASE += -GL -Gw -GA
    # Auto-Parallelizer
    QMAKE_CXXFLAGS_RELEASE += -Qpar
    # CPU Instrcutions.
    QMAKE_CXXFLAGS_RELEASE += -arch:AVX
    # CPU Architecture, turn on for Mu's.
    # Intel Processor, Sandy bridge or later.
    QMAKE_CXXFLAGS_RELEASE += -favor:INTEL64
    # AMD Processor,
    #QMAKE_CXXFLAGS_RELEASE += -favor:AMD64

    # Linker Options.
    # Link-time Code Generation, use with -GL
    QMAKE_LFLAGS_RELEASE += -LTCG
}
gcc: {
    # Instruction optimized
    CONFIG += mmx sse sse2 sse3
    QMAKE_CXXFLAGS_RELEASE += -mmmx -msse -msse2 -msse3 -finline-functions
    # Vectorize optimization
    QMAKE_CXXFLAGS_RELEASE += -fivopts -ftree-vectorize
    # Concurrency
    QMAKE_CXXFLAGS_RELEASE += -funroll-loops
    # Qt will use clang on Mac OS X, but it seems that it treats the clang as
    # gcc, it will use all the flags we set to gcc on clang. In clang, it
    # doesn't have -fforce-addr switch, so we need to set the switcher
    # especially for Windows and Linux.
    win32:{
        # Vectorize optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-vectorizer-verbose=2
        # Concurrency
        QMAKE_CXXFLAGS_RELEASE += -floop-parallelize-all
        # Cache optimization.
        QMAKE_CXXFLAGS_RELEASE += -fbranch-target-load-optimize
        # Loop optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-loop-linear
        # GCC will only used for Windows 32-bit version.
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
    }
    linux:{
        # Vectorize optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-vectorizer-verbose=2
        # Loop optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-loop-linear
        # Cache optimization.
        QMAKE_CXXFLAGS_RELEASE += -fbranch-target-load-optimize
        # Concurrency
        QMAKE_CXXFLAGS_RELEASE += -floop-parallelize-all
        # GCC will only used for Linux 64-bit version.
        # Enabled force-addr.
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
        # Use sse to calculate the float operation.
        QMAKE_CXXFLAGS_RELEASE += -mfpmath=sse
        # Enabled 64-bit.
        QMAKE_CXXFLAGS_RELEASE += -m64
        # Others
        QMAKE_CXXFLAGS_RELEASE += -ftracer
    }
}

# Platform Specific Configuration.
win32: {
    # Application icon.
    RC_FILE += resource/icon/win_resource.rc \
               resource/icon/mu.ico
    ICON = resource/icon/mu.ico
    # Enable the backend and analysiser.
    CONFIG += backend-bass analysiser-ffmpeg
    # Add Windows platform special extras.
    QT += winextras
    SOURCES += plugin/knwindowsextras/knwindowsextras.cpp
    HEADERS += plugin/knwindowsextras/knwindowsextras.h
}

macx: {
    # Application icon.
    RC_FILE += resource/icon/mu.icns
    ICON = resource/icon/mu.icns
    # Set the info plist.
    QMAKE_INFO_PLIST = resource/platforms/mac/Info.plist
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

linux: {
    # Enable the backend and analysiser.
    CONFIG += backend-gstreamer analysiser-ffmpeg
    # Set the destination directory for the Linux special.
    DESTDIR = ../bin
    # This options is added for Linux specially.
    INSTALLS += target
}

# International Configureations.
i18n: {
    # MAKE_QM_FILES is a extra execution to called the lrelease to update the
    # qm file according to the original ts file real-time.
    MAKE_QM_FILES.input = TRANSLATIONS
    MAKE_QM_FILES.output = ${QMAKE_FILE_BASE}.qm
    MAKE_QM_FILES.commands = lrelease ${QMAKE_FILE_NAME} -qm i18n/${QMAKE_FILE_BASE}.qm
    MAKE_QM_FILES.CONFIG += no_link target_predeps
    # Add MAKE_QM_FILES to the extra compilers.
    QMAKE_EXTRA_COMPILERS += MAKE_QM_FILES
}

# Backend Specific Configuration
backend-gstreamer: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the backend enabled flag.
    DEFINES += ENABLE_BACKEND_GSTREAMER BACKEND_ENABLED
    # Add backend library to the project.
    CONFIG += link_pkgconfig
    PKGCONFIG += gstreamer-1.0
    # Add backend files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicbackendgstreamer/knmusicbackendgstreamer.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendgstreamer/knmusicbackendgstreamerthread.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendgstreamer/knmusicbackendgstreamer.h \
        plugin/knmusicplugin/plugin/knmusicbackendgstreamer/knmusicbackendgstreamerthread.h
}

backend-mpv: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the backend enabled flag.
    DEFINES += ENABLE_BACKEND_MPV BACKEND_ENABLED
    # Add backend library to the project.
    LIBS += -lmpv
    # Add backend files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicbackendmpv/knmusicbackendmpv.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendmpv/knmusicbackendmpvthread.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendmpv/knmusicbackendmpv.h \
        plugin/knmusicplugin/plugin/knmusicbackendmpv/knmusicbackendmpvthread.h
}
backend-av: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the backend enabled flag.
    DEFINES += ENABLE_BACKEND_QTAV BACKEND_ENABLED
    # Add QT modules.
    QT += av
    # Add backend files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicbackendqtav/knmusicbackendqtav.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendqtav/knmusicbackendqtavthread.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendqtav/knmusicbackendqtav.h \
        plugin/knmusicplugin/plugin/knmusicbackendqtav/knmusicbackendqtavthread.h
}

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
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.h \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.h \
        plugin/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.h
}

backend-phonon: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the backend enabled flag.
    DEFINES += ENABLE_BACKEND_PHONON BACKEND_ENABLED
    # Add backend library to the project.
    LIBS += -lphonon4qt5
    # Add backend files to the project.
    SOURCES += \
        plugin/knmusicplugin/plugin/knmusicbackendphonon/knmusicbackendphonon.cpp \
        plugin/knmusicplugin/plugin/knmusicbackendphonon/knmusicbackendphononthread.cpp
    HEADERS += \
        plugin/knmusicplugin/plugin/knmusicbackendphonon/knmusicbackendphonon.h \
        plugin/knmusicplugin/plugin/knmusicbackendphonon/knmusicbackendphononthread.h
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
    plugin/knmusicplugin/sdk/knmusicutil.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayerpanel.cpp \
    sdk/knglassanimebutton.cpp \
    sdk/knloopscrolllabel.cpp \
    plugin/knmusicplugin/sdk/knmusicdetailtageditpanel.cpp \
    sdk/sao/knsaobutton.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicnowplayinglistview.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicnowplayingdelegate.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayercontent.cpp \
    plugin/knmusicplugin/sdk/knmusiclibrarybase.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/knmusiclibrary.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagemanager.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarymodel.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarysongtab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytreeview.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryemptyhint.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorytab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorymodel.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategoryproxymodel.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryartisttab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorydisplay.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagesaver.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorydelegate.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarygenretab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicgenremodel.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbummodel.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryalbumtab.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumview.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumdetail.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtitle.cpp \
    sdk/knmouseunclickablewidget.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumlistview.cpp \
    plugin/knmusicplugin/sdk/knmusiclistviewbase.cpp \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumlistdelegate.cpp \
    sdk/knlabellineedit.cpp \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloaddialog.cpp \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloadwidget.cpp \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloadlist.cpp \
    sdk/knclockwheel.cpp \
    plugin/knmusicplugin/plugin/knmusictagwma/knmusictagwma.cpp \
    plugin/knmusicplugin/plugin/knmusictagid3v2/knmusictagwav.cpp \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdetaillistmodel.cpp \
    plugin/knmusicplugin/sdk/knmusiconlinelyricsdownloader.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusiccodeclabel.cpp \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayercontentswitcher.cpp \
    sdk/knlabelbutton.cpp \
    plugin/knmusicplugin/sdk/knmusiccategorysearch.cpp \
    plugin/knpreference/knpreferencelanguagepanel.cpp \
    plugin/knpreference/knpreferencelanguagepanelitem.cpp \
    plugin/knpreference/knpreferencelanguageitem.cpp \
    sdk/knpreferencepanel.cpp \
    sdk/knpreferencepanelitem.cpp \
    sdk/knselectanyfiledialog.cpp \
    sdk/knfiledialog.cpp \
    plugin/knmacextras/knmacextras.cpp \
    sdk/knimagelabel.cpp \
    plugin/knmusicplugin/plugin/knmusictagm4a/knmusictagm4a.cpp \
    plugin/knmusicplugin/plugin/knmusicminiplayer/knmusicminiplayer.cpp \
    plugin/knmusicplugin/sdk/knmusichscrolllyrics.cpp \
    plugin/knmusicplugin/plugin/knmusickgmusiclyrics/knmusickgmusiclyrics.cpp

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
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayer.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayerpanel.h \
    sdk/knglassanimebutton.h \
    sdk/knloopscrolllabel.h \
    plugin/knmusicplugin/sdk/knmusicdetailtageditpanel.h \
    sdk/sao/knsaobutton.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicnowplayinglistview.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicnowplayingdelegate.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayercontent.h \
    plugin/knmusicplugin/sdk/knmusiclibrarybase.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/knmusiclibrary.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagemanager.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarymodel.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarysongtab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytreeview.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryemptyhint.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorytab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorymodel.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategoryproxymodel.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryartisttab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorydisplay.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagesaver.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorydelegate.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarygenretab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicgenremodel.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorymodelbase.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbummodel.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryalbumtab.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumview.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumdetail.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtitle.h \
    sdk/knmouseunclickablewidget.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumlistview.h \
    plugin/knmusicplugin/sdk/knmusiclistviewbase.h \
    plugin/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumlistdelegate.h \
    sdk/knlabellineedit.h \
    plugin/knmusicplugin/sdk/knmusiclyricsdownloaddialogbase.h \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloaddialog.h \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloadwidget.h \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdownloadlist.h \
    sdk/knclockwheel.h \
    plugin/knmusicplugin/plugin/knmusictagwma/knmusictagwma.h \
    plugin/knmusicplugin/plugin/knmusictagid3v2/knmusictagwav.h \
    plugin/knmusicplugin/plugin/knmusiclyricsdownloaddialog/knmusiclyricsdetaillistmodel.h \
    plugin/knmusicplugin/sdk/knmusiconlinelyricsdownloader.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusiccodeclabel.h \
    plugin/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayercontentswitcher.h \
    sdk/knlabelbutton.h \
    sdk/knplatformextras.h \
    plugin/knmusicplugin/sdk/knmusiccategorysearch.h \
    plugin/knpreference/knpreferencelanguagepanel.h \
    plugin/knpreference/knpreferencelanguagepanelitem.h \
    plugin/knpreference/knpreferencelanguageitem.h \
    sdk/knpreferencepanel.h \
    sdk/knpreferencepanelitem.h \
    sdk/knselectanyfiledialog.h \
    sdk/knfiledialog.h \
    plugin/knmacextras/knmacextras.h \
    sdk/knimagelabel.h \
    plugin/knmusicplugin/plugin/knmusictagm4a/knmusictagm4a.h \
    plugin/knmusicplugin/sdk/knmusicminiplayerbase.h \
    plugin/knmusicplugin/plugin/knmusicminiplayer/knmusicminiplayer.h \
    plugin/knmusicplugin/sdk/knmusichscrolllyrics.h \
    plugin/knmusicplugin/plugin/knmusickgmusiclyrics/knmusickgmusiclyrics.h

RESOURCES += \
    resource/res.qrc

DISTFILES +=
