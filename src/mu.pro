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
    CONFIG += libbass
    libbass{
        LIBS += -lbass
    }
}

#UNIX common configure
unix{
    LIBS += -ldl
}

#Mac OS X
macx{
    CONFIG += libbass
    libbass{
        LIBS += /usr/lib/libbass.dylib
    }
    QMAKE_LFLAGS += -framework CoreFoundation
}


libbass{
    DEFINES += ENABLE_LIBBASS
    SOURCES += plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassglobal.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.cpp
    HEADERS += plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassglobal.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.h
}

#Add translations
TRANSLATIONS += locale/Simplified_Chinese.ts \
                locale/Traditional_Chinese.ts \
                locale/English.ts

#Add public path
INCLUDEPATH += public
#Add plugin and sdk path
INCLUDEPATH += plugin \
               plugin/sdk \
               plugin/module/knmusicplugin/sdk

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
    plugin/base/knpreference/knpreferencetitle.cpp \
    plugin/module/knmusicplugin/knmusicplugin.cpp \
    plugin/sdk/kncategorybutton.cpp \
    plugin/sdk/kncategorytabbar.cpp \
    plugin/sdk/kncategorytabwidget.cpp \
    plugin/base/knpreference/knpreferenceheaderbutton.cpp \
    public/knfontmanager.cpp \
    plugin/module/knmusicplugin/sdk/knmusicglobal.cpp \
    plugin/module/knmusicplugin/sdk/knmusicstandardbackend.cpp \
    plugin/module/knmusicplugin/sdk/knmusicparser.cpp \
    plugin/module/knmusicplugin/sdk/knmusictagpraser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayer.cpp \
    plugin/sdk/knhighlightlabel.cpp \
    plugin/sdk/knscrolllabel.cpp \
    plugin/sdk/kngraphicsgloweffect.cpp \
    plugin/sdk/knprogressslider.cpp \
    plugin/sdk/knabstractslider.cpp \
    plugin/sdk/kneditablelabel.cpp \
    plugin/sdk/knopacityanimebutton.cpp \
    plugin/sdk/knopacitybutton.cpp \
    plugin/sdk/knvolumeslider.cpp \
    plugin/sdk/kncancellineedit.cpp

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
    plugin/base/knpreference/knpreferencetitle.h \
    plugin/sdk/kncategoryplugin.h \
    plugin/module/knmusicplugin/knmusicplugin.h \
    plugin/sdk/kncategorybutton.h \
    plugin/sdk/kncategorytabbar.h \
    plugin/sdk/kncategorytabwidget.h \
    plugin/sdk/knabstractmusicplugin.h \
    plugin/base/knpreference/knpreferenceheaderbutton.h \
    public/knfontmanager.h \
    plugin/module/knmusicplugin/sdk/knmusicbackend.h \
    plugin/module/knmusicplugin/sdk/knmusicglobal.h \
    plugin/module/knmusicplugin/sdk/knmusicbackendthread.h \
    plugin/module/knmusicplugin/sdk/knmusicstandardbackend.h \
    plugin/module/knmusicplugin/sdk/knmusicparser.h \
    plugin/module/knmusicplugin/sdk/knmusicanalysiser.h \
    plugin/module/knmusicplugin/sdk/knmusictagpraser.h \
    plugin/module/knmusicplugin/sdk/knmusiclistparser.h \
    plugin/module/knmusicplugin/sdk/knmusicheaderplayerbase.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayer.h \
    plugin/sdk/knhighlightlabel.h \
    plugin/sdk/knscrolllabel.h \
    plugin/sdk/kngraphicsgloweffect.h \
    plugin/sdk/knprogressslider.h \
    plugin/sdk/knabstractslider.h \
    plugin/sdk/kneditablelabel.h \
    plugin/sdk/knopacityanimebutton.h \
    plugin/sdk/knopacitybutton.h \
    plugin/sdk/knvolumeslider.h \
    plugin/sdk/kncancellineedit.h

RESOURCES += \
    resource/res.qrc

