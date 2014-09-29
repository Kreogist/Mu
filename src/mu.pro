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

#Add translations
TRANSLATIONS += locale/Simplified_Chinese.ts \
                locale/Traditional_Chinese.ts \
                locale/English.ts

SOURCES += \
    main.cpp \
    core/knsingleapplication.cpp \
    core/knpluginmanager.cpp

HEADERS += \
    core/knsingleapplication.h \
    core/knpluginmanager.h

