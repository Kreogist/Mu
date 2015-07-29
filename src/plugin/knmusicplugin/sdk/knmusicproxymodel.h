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

#ifndef KNMUSICPROXYMODEL_H
#define KNMUSICPROXYMODEL_H

#include "knmusicutil.h"

#include <QSortFilterProxyModel>

using namespace MusicUtil;

class KNMusicModel;
class KNMusicProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KNMusicProxyModel(QObject *parent = 0);
    KNMusicModel *musicModel();

signals:

public slots:
    void setSearchBlocks(const QList<KNMusicSearchBlock> &blockList);

protected:
    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const Q_DECL_OVERRIDE;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    inline bool checkColumnRule(const QModelIndex &index,
                                const KNMusicSearchBlock &block) const;
    inline bool checkRule(QAbstractItemModel *model,
                                const int &row,
                                const KNMusicSearchBlock &block) const;
    QList<KNMusicSearchBlock> m_blocks;
};

#endif // KNMUSICPROXYMODEL_H
