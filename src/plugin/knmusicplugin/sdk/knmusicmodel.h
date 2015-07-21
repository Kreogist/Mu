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

#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QList>

#include "knmusicglobal.h"

#include <QAbstractTableModel>

using namespace MusicUtil;

class KNMusicModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);

    void appendRow(const KNMusicDetailInfo &detailInfo);
    void appendRows(const QList<KNMusicDetailInfo> &detailInfos);
    bool removeRows(int position,
                    int rows,
                    const QModelIndex &index = QModelIndex()) Q_DECL_OVERRIDE;

    void clear();

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;

    QVariant data(const QModelIndex &index, int role) const;

signals:

public slots:

private:
    QList<KNMusicDetailInfo> m_detailInfos;
};

#endif // KNMUSICMODEL_H
