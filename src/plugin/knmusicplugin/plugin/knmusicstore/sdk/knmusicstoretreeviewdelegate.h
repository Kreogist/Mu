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

#ifndef KNMUSICSTORETREEVIEWDELEGATE_H
#define KNMUSICSTORETREEVIEWDELEGATE_H

#include <QStyledItemDelegate>

#define StoreItemDelegateHeight 25

/*!
 * \brief The KNMusicStoreTreeViewDelegate class provides the delegate policy
 * for all the tree view in the music store.\n
 * This delegate change the height of each row. And rendering different column
 * with different policy.
 */
class KNMusicStoreTreeViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreTreeViewDelegate object.
     * \param parent The parent object.
     */
    explicit KNMusicStoreTreeViewDelegate(QObject *parent = nullptr);

    /*!
     * \brief Reimplemented from QStyledItemDelegate::sizeHint().
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
};

#endif // KNMUSICSTORETREEVIEWDELEGATE_H
