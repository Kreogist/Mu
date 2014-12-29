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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNMUSICHEADERPLAYERAPPENDMENU_H
#define KNMUSICHEADERPLAYERAPPENDMENU_H

#include "sao/knsaosubmenu.h"

namespace KNMusicAppendMenu
{
enum MenuActions
{
    AppendRatingNoStar,
    AppendRatingOneStar,
    AppendRatingTwoStar,
    AppendRatingThreeStar,
    AppendRatingFourStar,
    AppendRatingFiveStar,
    AppendShowInGraphicShell,
    AppendShowDetail,
    AppendLocateNowPlaying,
    AppendShowInSongs,
    AppendShowInArtists,
    AppendShowInAlbums,
    AppendShowInGenres,
    MenuActionsCount
};
}

using namespace KNMusicAppendMenu;

class QSignalMapper;
class KNMusicHeaderPlayerAppendMenu : public KNSAOSubMenu
{
    Q_OBJECT
public:
    explicit KNMusicHeaderPlayerAppendMenu(QWidget *parent = 0);
    ~KNMusicHeaderPlayerAppendMenu();

signals:
    void requireDoAction(int actionIndex);

public slots:
    void retranslate();

private:
    enum SubMenus
    {
        SubMenuRating,
        SubMenuLocate,
        SubMenuCount
    };
    inline void initialSubMenus();
    inline void initialActions();
    QSignalMapper *m_actionTrigger;
    QAction *m_menuActions[MenuActionsCount];
    KNSAOSubMenu *m_subMenus[SubMenuCount];
};

#endif // KNMUSICHEADERPLAYERAPPENDMENU_H
