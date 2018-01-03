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

#ifndef KNPREFERENCEUTIL_H
#define KNPREFERENCEUTIL_H

#include <QString>
#include <QVariant>
#include <QStringList>
#include <QList>

namespace PreferenceUtil
{
    enum PreferencePanels
    {
        PanelGeneral,
        PanelPlayback,
        PanelLyrics,
        PanelShortcuts,
//        PanelFileAsso,
        PreferencePanelCount
    };

    enum PreferencePanelOptionType
    {
        TypeCombo,
        TypeBoolean,
        TypeInt,
        TypeFont,
        TypeShortcut,
        TypeAudioDevice,
        TypeFileType,
        TypeDirectory
    };

    struct PreferencePanelOption
    {
        QStringList path;
        QVariant configure;
        QVariant defaultValue;
        QString title;
        QString explain;
        PreferencePanelOptionType type;
        bool advanced;
        PreferencePanelOption() :
            advanced(false)
        {
        }
    };

    struct PreferencePanelBlock
    {
        QList<PreferencePanelOption> options;
        QString blockName;
        bool advanced;
        PreferencePanelBlock() :
            advanced(false)
        {
        }
    };
}

#endif // KNPREFERENCEUTIL_H
