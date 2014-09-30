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

#Add modules
QT += core \
      gui \
      widgets \
      multimedia \
      network

#Enable c++11
CONFIG += c++11

#Enable processor instruction sets when using release mode.
release{
    CONFIG += mmx sse sse2 sse3
    QMAKE_CXXFLAGS += -mmmx -msse -msse2 -msse3 -finline-functions
}

#Windows configure
win32{
    DESTDIR =../mu
}

#Add translations
TRANSLATIONS += locale/Simplified_Chinese.ts \
                locale/Traditional_Chinese.ts \
                locale/English.ts

#Add public path
INCLUDEPATH += public
#Add plugin and sdk path
INCLUDEPATH += plugin \
               plugin/sdk

SOURCES += \
    main.cpp \
    core/knsingleapplication.cpp \
    core/knpluginmanager.cpp \
    public/knglobal.cpp \
    public/knconfigure.cpp \
    plugin/base/knmainwindow/knmainwindow.cpp \
    plugin/base/knmainwindow/knmainwindowcontainer.cpp \
    plugin/base/knmainwindowheader/knmainwindowheader.cpp \
    plugin/base/knmainwindowheader/knheadercontainer.cpp \
    plugin/base/knmainwindowheader/knheaderbutton.cpp \
    plugin/sdk/knanimecheckedbutton.cpp \
    plugin/base/knmainwindowheader/knheaderswitcher.cpp \
    plugin/sdk/knwidgetswitcher.cpp \
    plugin/base/knmainwindowcategorystack/knmainwindowcategorystack.cpp \
    plugin/sdk/knhwidgetswitcher.cpp \
    plugin/base/knmainwindowcategoryswitcher/knmainwindowcategoryswitcher.cpp \
    plugin/base/knmainwindowcategoryswitcher/kncategoryswitcherwidget.cpp \
    plugin/base/knmainwindowcategoryswitcher/knabstractcategorybutton.cpp \
    plugin/base/knmainwindowcategoryswitcher/kncategorysettingbutton.cpp \
    plugin/base/knpreference/knpreference.cpp \
    plugin/base/knpreference/knpreferencecategory.cpp \
    plugin/base/knpreference/knpreferencecontents.cpp \
    plugin/base/knpreference/knpreferencepanel.cpp \
    plugin/base/knpreference/knpreferencetitle.cpp

HEADERS += \
    core/knsingleapplication.h \
    core/knpluginmanager.h \
    public/knglobal.h \
    plugin/sdk/knmainwindowplugin.h \
    public/knconfigure.h \
    plugin/base/knmainwindow/knmainwindow.h \
    plugin/base/knmainwindow/knmainwindowcontainer.h \
    plugin/sdk/knmainwindowheaderplugin.h \
    plugin/base/knmainwindowheader/knmainwindowheader.h \
    plugin/base/knmainwindowheader/knheadercontainer.h \
    plugin/base/knmainwindowheader/knheaderbutton.h \
    plugin/sdk/knabstractbutton.h \
    plugin/sdk/knanimecheckedbutton.h \
    plugin/base/knmainwindowheader/knheaderswitcher.h \
    plugin/sdk/knwidgetswitcher.h \
    plugin/sdk/knmainwindowcategorystackplugin.h \
    plugin/base/knmainwindowcategorystack/knmainwindowcategorystack.h \
    plugin/sdk/knhwidgetswitcher.h \
    plugin/base/knmainwindowcategoryswitcher/knmainwindowcategoryswitcher.h \
    plugin/base/knmainwindowcategoryswitcher/kncategoryswitcherwidget.h \
    plugin/base/knmainwindowcategoryswitcher/knabstractcategorybutton.h \
    plugin/base/knmainwindowcategoryswitcher/kncategorysettingbutton.h \
    plugin/sdk/knmainwindowcategoryswitcherplugin.h \
    plugin/sdk/knpreferenceplugin.h \
    plugin/base/knpreference/knpreference.h \
    plugin/base/knpreference/knpreferencecategory.h \
    plugin/base/knpreference/knpreferencecontents.h \
    plugin/base/knpreference/knpreferencepanel.h \
    plugin/base/knpreference/knpreferencetitle.h

RESOURCES += \
    resource/res.qrc

