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
#ifndef KNMUSICLIBRARYALBUMTAB_H
#define KNMUSICLIBRARYALBUMTAB_H

#include "knmusiclibrarycategorytab.h"

class KNMusicLibraryAlbumTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryAlbumTab(QObject *parent = 0);
    QString caption();
    QPixmap icon();
    QWidget *widget();

signals:

public slots:
    void setLibraryModel(KNMusicLibraryModel *model);
    void onActionSearch(const QString &text);

private:
    QWidget *m_widget;
};

#endif // KNMUSICLIBRARYALBUMTAB_H
