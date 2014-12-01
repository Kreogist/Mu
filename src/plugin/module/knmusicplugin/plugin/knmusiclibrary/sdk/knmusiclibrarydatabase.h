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
#ifndef KNMUSICLIBRARYDATABASE_H
#define KNMUSICLIBRARYDATABASE_H

#include <QList>
#include <QStandardItem>

#include "knmusicglobal.h"

#include "knjsondatabase.h"

using namespace KNMusic;

class KNGlobal;
class KNMusicLibraryDatabase : public KNJSONDatabase
{
    Q_OBJECT
public:
    explicit KNMusicLibraryDatabase(QObject *parent = 0);
    void recoverModel();
    void appendMusicRow(const QList<QStandardItem *> &musicRow);
    void updateMusicRow(const int &row,
                        const QList<QStandardItem *> &musicRow);
    void updateArtworkKey(const int &row, const QString &artworkKey);
    void removeMusicRow(const int &row);

signals:
    void requireRecoverMusicRow(const QList<QStandardItem *> &musicRow);

public slots:

private:
    void generateObject(const QList<QStandardItem *> &musicRow,
                        QJsonObject &musicObject);
    void generateRow(const QJsonObject &musicObject,
                     QList<QStandardItem *> &musicRow);
    KNGlobal *m_global;
};

#endif // KNMUSICLIBRARYDATABASE_H
