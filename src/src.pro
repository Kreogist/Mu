# Copyright (C) Kreogist Dev Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
    plugin/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.cpp

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
    plugin/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.h

RESOURCES += \
    resource/res.qrc
