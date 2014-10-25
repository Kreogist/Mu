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

#Mac OS X configure
macx{
    CONFIG += libbass
    libbass{
        LIBS += /usr/lib/libbass.dylib
    }
    QMAKE_LFLAGS += -framework CoreFoundation
}

#Linux configure
linux{
    CONFIG += libbass
    libbass{
        LIBS += -lbass
    }
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
    plugin/sdk/kncancellineedit.cpp \
    plugin/module/knmusicplugin/plugin/knmusicnowplaying/knmusicnowplaying.cpp \
    plugin/base/knpreference/knpreferencecategorylist.cpp \
    plugin/base/knpreference/knpreferencecategoryitem.cpp \
    plugin/module/knmusicplugin/sdk/knmusicproxymodel.cpp \
    plugin/module/knmusicplugin/sdk/knmusicmodel.cpp \
    plugin/module/knmusicplugin/sdk/knmusicproxymodelpool.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusicheaderlyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclyricsmanager.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclrcparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmanager.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylisttab.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistdisplay.cpp \
    plugin/module/knmusicplugin/sdk/knmusiccategorylistviewbase.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistview.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistvieweditor.cpp \
    plugin/sdk/knanimationmenu.cpp \
    plugin/sdk/knlinearsensewidget.cpp \
    plugin/sdk/kndropproxycontainer.cpp \
    plugin/sdk/knmousesensewidget.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewbase.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheader.cpp \
    plugin/sdk/knmousesenseheader.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheadermenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylisttreeview.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistindex.cpp \
    plugin/module/knmusicplugin/sdk/knmusicsearcher.cpp \
    plugin/sdk/knfilesearcher.cpp \
    plugin/module/knmusicplugin/sdk/knmusicmodelassist.cpp \
    plugin/module/knmusicplugin/sdk/knmusicanalysiscache.cpp \
    plugin/sdk/knpreferencewidgetspanel.cpp \
    plugin/sdk/knvwidgetswitcher.cpp \
    plugin/sdk/preference/knpreferenceitembase.cpp \
    plugin/sdk/preference/knpreferenceitemswitcher.cpp \
    plugin/sdk/knsideshadowwidget.cpp \
    plugin/sdk/knanimeroundswitcher.cpp \
    plugin/module/knmusicplugin/plugin/knmusicnowplaying/knmusicsingleplaylistmodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistitem.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlist.cpp \
    plugin/sdk/knconnectionhandler.cpp \
    plugin/sdk/knanimecolorswitcher.cpp \
    plugin/sdk/preference/knpreferenceitemlineedit.cpp \
    plugin/sdk/preference/knpreferenceitemglobal.cpp \
    plugin/sdk/preference/knpreferenceitempathbrowser.cpp \
    plugin/sdk/knpathlineedit.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistdelegate.cpp \
    plugin/module/knmusicplugin/sdk/knmusicratingeditor.cpp \
    plugin/module/knmusicplugin/sdk/knmusicratingdelegate.cpp \
    plugin/module/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.cpp

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
    plugin/sdk/kncancellineedit.h \
    plugin/module/knmusicplugin/sdk/knmusicnowplayingbase.h \
    plugin/module/knmusicplugin/plugin/knmusicnowplaying/knmusicnowplaying.h \
    plugin/base/knpreference/knpreferencecategorylist.h \
    plugin/base/knpreference/knpreferencecategoryitem.h \
    plugin/module/knmusicplugin/sdk/knmusicproxymodel.h \
    plugin/module/knmusicplugin/sdk/knmusicmodel.h \
    plugin/module/knmusicplugin/sdk/knmusicproxymodelpool.h \
    plugin/module/knmusicplugin/sdk/knmusicheaderlyricsbase.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusicheaderlyrics.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclyricsmanager.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclrcparser.h \
    plugin/module/knmusicplugin/sdk/knmusictab.h \
    plugin/module/knmusicplugin/sdk/knmusicplaylistmanagerbase.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmanager.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylisttab.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistdisplay.h \
    plugin/module/knmusicplugin/sdk/knmusiccategorylistviewbase.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistview.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistvieweditor.h \
    plugin/sdk/knanimationmenu.h \
    plugin/sdk/knlinearsensewidget.h \
    plugin/sdk/kndropproxycontainer.h \
    plugin/sdk/knmousesensewidget.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewbase.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheader.h \
    plugin/sdk/knmousesenseheader.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheadermenu.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylisttreeview.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistindex.h \
    plugin/module/knmusicplugin/sdk/knmusicsearcher.h \
    plugin/sdk/knfilesearcher.h \
    plugin/module/knmusicplugin/sdk/knmusicmodelassist.h \
    plugin/module/knmusicplugin/sdk/knmusicanalysiscache.h \
    plugin/sdk/preference/knpreferenceitembase.h \
    plugin/sdk/knpreferencewidgetspanel.h \
    plugin/sdk/knvwidgetswitcher.h \
    plugin/sdk/preference/knpreferenceitemswitcher.h \
    plugin/sdk/knsideshadowwidget.h \
    plugin/sdk/knanimeroundswitcher.h \
    plugin/module/knmusicplugin/plugin/knmusicnowplaying/knmusicsingleplaylistmodel.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmodel.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistitem.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlist.h \
    plugin/sdk/knconnectionhandler.h \
    plugin/sdk/knanimecolorswitcher.h \
    plugin/sdk/preference/knpreferenceitemlineedit.h \
    plugin/sdk/preference/knpreferenceitemglobal.h \
    plugin/sdk/preference/knpreferenceitempathbrowser.h \
    plugin/sdk/knpathlineedit.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistlistdelegate.h \
    plugin/module/knmusicplugin/sdk/knmusicratingeditor.h \
    plugin/module/knmusicplugin/sdk/knmusicratingdelegate.h \
    plugin/module/knmusicplugin/sdk/knmusicsolomenubase.h \
    plugin/module/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.h \
    plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.h

RESOURCES += \
    resource/res.qrc

