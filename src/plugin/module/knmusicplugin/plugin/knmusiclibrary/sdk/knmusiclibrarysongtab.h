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
#ifndef KNMUSICLIBRARYSONGTAB_H
#define KNMUSICLIBRARYSONGTAB_H

#include "knmusiclibrarytab.h"

class KNDropProxyContainer;
class KNMusicLibraryTreeView;
class KNMusicLibrarySongTab : public KNMusicLibraryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibrarySongTab(QObject *parent = 0);
    ~KNMusicLibrarySongTab();
    QAction *showInAction();
    QString caption();
    QPixmap icon();
    QWidget *widget();

signals:

public slots:
    void retranslate();
    void setLibraryModel(KNMusicLibraryModel *model);
    void onActionSearch(const QString &text);

private slots:
    void onActionShowInSong();

private:
    void initialShowInAction();
    KNDropProxyContainer *m_dropProxy;
    KNMusicLibraryTreeView *m_treeview;
    QAction *m_showInSongTab;
    KNMusicLibraryModel *m_musicLibrary;
};

#endif // KNMUSICLIBRARYSONGTAB_H
