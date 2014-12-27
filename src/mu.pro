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
      xml \
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
    CONFIG += libbass FFMpeg
    libbass{
        LIBS += -lbass
    }
    FFMpeg{
        LIBS += -lavformat -lavcodec -lavutil
    }
    RC_FILE += resource/icon/windows/resource.rc
    ICON += resource/icon/windows/mu.ico
    #Windows special extras.
#    QT += winextras
#    SOURCES += plugin/module/knwindowsextras/knwindowsextras.cpp
#    HEADERS += plugin/module/knwindowsextras/knwindowsextras.h
}

#UNIX common configure
unix{
    LIBS += -ldl
}

#Mac OS X configure
macx{
    INCLUDEPATH += /usr/include
#    LIBS += -L/usr/lib
    CONFIG += libbass FFMpeg
    libbass{
        LIBS += /usr/lib/libbass.dylib
    }
    FFMpeg{
        LIBS += /usr/lib/libavformat.dylib \
                /usr/lib/libavcodec.dylib \
                /usr/lib/libavutil.dylib
    }
    QMAKE_LFLAGS += -framework CoreFoundation
    QMAKE_INFO_PLIST = resource/icon/mac/Info.plist
    ICON += resource/icon/mac/mu.icns
}

#Linux configure
linux{
    CONFIG += libbass
    libbass{
        LIBS += -lbass
    }
}

FFMpeg{
    DEFINES += ENABLE_FFMPEG
    SOURCES += plugin/sdk/knffmpegglobal.cpp \
               plugin/module/knmusicplugin/plugin/knmusicffmpeganalysiser/knmusicffmpeganalysiser.cpp
    HEADERS += plugin/sdk/knffmpegglobal.h \
               plugin/module/knmusicplugin/plugin/knmusicffmpeganalysiser/knmusicffmpeganalysiser.h
}

libVLC{
    contains(CONFIG, libbass){
        error("You can't enable more than one backend at the same time.")
    }
    DEFINES += ENABLE_LIBVLC
    SOURCES += plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicbackendvlc.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicvlcglobal.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicbackendvlcthread.cpp
    HEADERS += plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicbackendvlc.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicvlcglobal.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendvlc/knmusicbackendvlcthread.h
}

libbass{
    contains(CONFIG, libvlc){
        error("You can't enable more than one backend at the same time.")
    }
    DEFINES += ENABLE_LIBBASS
    SOURCES += plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassglobal.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.cpp \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.cpp
    HEADERS += plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassglobal.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbass.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbassanalysiser.h \
               plugin/module/knmusicplugin/plugin/knmusicbackendbass/knmusicbackendbassthread.h
}

#Add translations
TRANSLATIONS += locale/Simplified_Chinese.ts \
                locale/Traditional_Chinese.ts

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
    plugin/base/knmainwindowheader/knheaderswitcher.cpp \
    plugin/sdk/knanimecheckedbutton.cpp \
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
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusicheaderlyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclyricsmanager.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclrcparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmanager.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylisttab.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistdisplay.cpp \
    plugin/module/knmusicplugin/sdk/knmusiccategorylistviewbase.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistview.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistvieweditor.cpp \
    plugin/sdk/knanimationmenu.cpp \
    plugin/sdk/knlinearsensewidget.cpp \
    plugin/sdk/kndropproxycontainer.cpp \
    plugin/sdk/knmousesensewidget.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewbase.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheader.cpp \
    plugin/sdk/knmousesenseheader.cpp \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheadermenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylisttreeview.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistindex.cpp \
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
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistmodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistitem.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlist.cpp \
    plugin/sdk/knconnectionhandler.cpp \
    plugin/sdk/knanimecolorswitcher.cpp \
    plugin/sdk/preference/knpreferenceitemlineedit.cpp \
    plugin/sdk/preference/knpreferenceitemglobal.cpp \
    plugin/sdk/preference/knpreferenceitempathbrowser.cpp \
    plugin/sdk/knpathlineedit.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistdelegate.cpp \
    plugin/module/knmusicplugin/sdk/knmusicratingeditor.cpp \
    plugin/module/knmusicplugin/sdk/knmusicratingdelegate.cpp \
    plugin/module/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusicmultimenu/knmusicmultimenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusiccueparser/knmusiccueparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistassistant.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistloader.cpp \
    plugin/sdk/knmessagebox.cpp \
    plugin/sdk/messagebox/knmessageboxconfigure.cpp \
    plugin/sdk/messagebox/knmessagecontent.cpp \
    plugin/sdk/messagebox/knmessageblock.cpp \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetaildialog.cpp \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetailpanel.cpp \
    core/knexpandmainwindow.cpp \
    plugin/sdk/knemptystatewidget.cpp \
    public/knlocalemanager.cpp \
    plugin/base/knpreference/knpreferencelanguageitem.cpp \
    plugin/base/knpreference/knpreferencelanguagepanel.cpp \
    plugin/base/knpreference/knpreferencelanguagepanelitem.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistemptyhint.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagflac/knmusictagflac.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagid3v2/knmusictagid3v2.cpp \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetailoverview.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagm4a/knmusictagm4a.cpp \
    plugin/sdk/knfilepathlabel.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagwma/knmusictagwma.cpp \
    plugin/sdk/knsearchbox.cpp \
    plugin/module/knmusicplugin/plugin/knmusicsearch/knmusicsearch.cpp \
    plugin/module/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltip.cpp \
    plugin/sdk/knmousedetectheader.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagapev2/knmusictagapev2.cpp \
    plugin/module/knmusicplugin/plugin/knmusictagid3v2/knmusictagwav.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/knmusiclibrary.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarysongtab.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryartisttab.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryalbumtab.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarygenretab.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytreeview.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarymodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytab.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorymodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategoryproxymodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorytab.cpp \
    plugin/sdk/knhashpixmaplist.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorydisplay.cpp \
    plugin/module/knmusicplugin/sdk/knmusicanalysisextend.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryanalysisextend.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicgenremodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumview.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbummodel.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicxspfparser/knmusicxspfparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicttplparser/knmusicttplparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicm3uparser/knmusicm3uparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumdetail.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtreeview.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumindexdelegate.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtitle.cpp \
    plugin/sdk/knjsondatabase.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarydatabase.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagemanager.cpp \
    plugin/sdk/knngnlbutton.cpp \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryemptyhint.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicwplparser/knmusicwplparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltipartwork.cpp \
    plugin/sdk/preference/knpreferenceitemnumber.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/sdk/knmusiclyricsdownloader.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicxiamilyrics/knmusicxiamilyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/sdk/knmusiclyricsglobal.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicqqlyrics/knmusicqqlyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicbaidulyrics/knmusicbaidulyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicitunesxmlparser/knmusicitunesxmlparser.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.cpp \
    plugin/module/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayer.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicttpodlyrics/knmusicttpodlyrics.cpp \
    plugin/sdk/sao/knsaostyle.cpp \
    plugin/sdk/sao/knsaosubmenu.cpp \
    plugin/module/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayerappendmenu.cpp \
    plugin/module/knmusicplugin/sdk/knmusiccategorytabwidget.cpp \
    plugin/sdk/preference/knpreferenceitemfont.cpp \
    plugin/sdk/knfontdialog.cpp

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
    plugin/module/knmusicplugin/sdk/knmusicheaderlyricsbase.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusicheaderlyrics.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclyricsmanager.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/knmusiclrcparser.h \
    plugin/module/knmusicplugin/sdk/knmusictab.h \
    plugin/module/knmusicplugin/sdk/knmusicplaylistmanagerbase.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/knmusicplaylistmanager.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylisttab.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistdisplay.h \
    plugin/module/knmusicplugin/sdk/knmusiccategorylistviewbase.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistview.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistvieweditor.h \
    plugin/sdk/knanimationmenu.h \
    plugin/sdk/knlinearsensewidget.h \
    plugin/sdk/kndropproxycontainer.h \
    plugin/sdk/knmousesensewidget.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewbase.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheader.h \
    plugin/sdk/knmousesenseheader.h \
    plugin/module/knmusicplugin/sdk/knmusictreeviewheadermenu.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylisttreeview.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistindex.h \
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
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistmodel.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistitem.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlist.h \
    plugin/sdk/knconnectionhandler.h \
    plugin/sdk/knanimecolorswitcher.h \
    plugin/sdk/preference/knpreferenceitemlineedit.h \
    plugin/sdk/preference/knpreferenceitemglobal.h \
    plugin/sdk/preference/knpreferenceitempathbrowser.h \
    plugin/sdk/knpathlineedit.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistdelegate.h \
    plugin/module/knmusicplugin/sdk/knmusicratingeditor.h \
    plugin/module/knmusicplugin/sdk/knmusicratingdelegate.h \
    plugin/module/knmusicplugin/sdk/knmusicsolomenubase.h \
    plugin/module/knmusicplugin/plugin/knmusicsolomenu/knmusicsolomenu.h \
    plugin/module/knmusicplugin/sdk/knmusicmultimenubase.h \
    plugin/module/knmusicplugin/plugin/knmusicmultimenu/knmusicmultimenu.h \
    plugin/module/knmusicplugin/plugin/knmusiccueparser/knmusiccueparser.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistlistassistant.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistparser.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistloader.h \
    plugin/sdk/knmessagebox.h \
    plugin/sdk/messagebox/knmessageboxconfigure.h \
    plugin/sdk/messagebox/knmessagecontent.h \
    plugin/sdk/messagebox/knmessageblock.h \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetaildialog.h \
    plugin/module/knmusicplugin/sdk/knmusicdetaildialogbase.h \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetailpanel.h \
    core/knexpandmainwindow.h \
    plugin/sdk/knemptystatewidget.h \
    public/knlocalemanager.h \
    plugin/base/knpreference/knpreferencelanguageitem.h \
    plugin/base/knpreference/knpreferencelanguagepanel.h \
    plugin/base/knpreference/knpreferencelanguagepanelitem.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/sdk/knmusicplaylistemptyhint.h \
    plugin/module/knmusicplugin/plugin/knmusictagid3v1/knmusictagid3v1.h \
    plugin/module/knmusicplugin/plugin/knmusictagflac/knmusictagflac.h \
    plugin/module/knmusicplugin/plugin/knmusictagid3v2/knmusictagid3v2.h \
    plugin/module/knmusicplugin/plugin/knmusicdetaildialog/knmusicdetailoverview.h \
    plugin/module/knmusicplugin/plugin/knmusictagm4a/knmusictagm4a.h \
    plugin/sdk/knfilepathlabel.h \
    plugin/module/knmusicplugin/plugin/knmusictagwma/knmusictagwma.h \
    plugin/sdk/knsearchbox.h \
    plugin/module/knmusicplugin/plugin/knmusicsearch/knmusicsearch.h \
    plugin/module/knmusicplugin/sdk/knmusicsearchbase.h \
    plugin/module/knmusicplugin/sdk/knmusicdetailtooltipbase.h \
    plugin/module/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltip.h \
    plugin/sdk/knplatformextras.h \
    plugin/sdk/knmousedetectheader.h \
    plugin/module/knmusicplugin/plugin/knmusictagapev2/knmusictagapev2.h \
    plugin/module/knmusicplugin/plugin/knmusictagid3v2/knmusictagwav.h \
    plugin/module/knmusicplugin/sdk/knmusiclibrarybase.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/knmusiclibrary.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarysongtab.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryartisttab.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryalbumtab.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarygenretab.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytreeview.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarymodel.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarytab.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorymodel.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategoryproxymodel.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarycategorytab.h \
    plugin/sdk/knhashpixmaplist.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiccategorydisplay.h \
    plugin/module/knmusicplugin/sdk/knmusicanalysisextend.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryanalysisextend.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicgenremodel.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumview.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbummodel.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicxspfparser/knmusicxspfparser.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicttplparser/knmusicttplparser.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicm3uparser/knmusicm3uparser.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumdetail.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtreeview.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumindexdelegate.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusicalbumtitle.h \
    plugin/sdk/knjsondatabase.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibrarydatabase.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryimagemanager.h \
    plugin/sdk/knngnlbutton.h \
    plugin/module/knmusicplugin/plugin/knmusiclibrary/sdk/knmusiclibraryemptyhint.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicwplparser/knmusicwplparser.h \
    plugin/module/knmusicplugin/plugin/knmusicdetailtooltip/knmusicdetailtooltipartwork.h \
    plugin/sdk/preference/knpreferenceitemnumber.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/sdk/knmusiclyricsdownloader.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicxiamilyrics/knmusicxiamilyrics.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/sdk/knmusiclyricsglobal.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicqqlyrics/knmusicqqlyrics.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicbaidulyrics/knmusicbaidulyrics.h \
    plugin/module/knmusicplugin/plugin/knmusicplaylistmanager/plugin/knmusicitunesxmlparser/knmusicitunesxmlparser.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicttplayerlyrics/knmusicttplayerlyrics.h \
    plugin/module/knmusicplugin/sdk/knmusicmainplayerbase.h \
    plugin/module/knmusicplugin/plugin/knmusicmainplayer/knmusicmainplayer.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderlyrics/plugin/knmusicttpodlyrics/knmusicttpodlyrics.h \
    plugin/sdk/sao/knsaostyle.h \
    plugin/sdk/sao/knsaosubmenu.h \
    plugin/module/knmusicplugin/plugin/knmusicheaderplayer/knmusicheaderplayerappendmenu.h \
    plugin/module/knmusicplugin/sdk/knmusiccategorytabwidget.h \
    plugin/sdk/preference/knpreferenceitemfont.h \
    plugin/sdk/knfontdialog.h

RESOURCES += \
    resource/res.qrc

