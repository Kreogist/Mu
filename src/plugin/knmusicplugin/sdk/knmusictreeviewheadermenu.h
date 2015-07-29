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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICTREEVIEWHEADERMENU_H
#define KNMUSICTREEVIEWHEADERMENU_H

#include "knmusicglobal.h"

#include "knanimationmenu.h"

class QSignalMapper;
class KNMusicTreeViewHeaderMenu : public KNAnimationMenu
{
    Q_OBJECT
public:
    explicit KNMusicTreeViewHeaderMenu(QWidget *parent = 0);
    void setVisibleState(const int &logicalIndex,
                         const bool &visible);

signals:
    void requireResize(const int &index);
    void requireResizeAll();
    void requireSetColumnVisible(const int &column,
                                 const bool &ok);

public slots:
    void setMouseDownLogicalIndex(const int &index);

private slots:
    void retranslate();
    void onActionVisibleChange(const int &index);
    void onActionResizeCurrent();

private:
    void createWidthTweakMenu();
    void createVisibleMenu();
    QAction *m_tweakAllWidth, *m_tweakWidth, *m_itemVisible[MusicDataCount];
    QSignalMapper *m_visibleMapper;
    int m_mouseDownLogicalIndex;
};

#endif // KNMUSICTREEVIEWHEADERMENU_H
