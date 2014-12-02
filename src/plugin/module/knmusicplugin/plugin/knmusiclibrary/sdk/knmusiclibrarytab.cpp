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
#include <QAction>

#include "knmusiclibrarytab.h"

KNMusicLibraryTab::KNMusicLibraryTab(QObject *parent) :
    KNMusicTab(parent)
{
}

void KNMusicLibraryTab::onActionTabShow()
{
    //Emit load request.
    emit requireLoadLibrary();
    //Hide the current show in action.
    showInAction()->setVisible(false);
}

void KNMusicLibraryTab::onActionTabHide()
{
    //Show the current show in action.
    showInAction()->setVisible(true);
}
