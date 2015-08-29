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

#ifndef KNMUSICMULTIMENU_H
#define KNMUSICMULTIMENU_H

#include <QModelIndex>

#include "knmusicmultimenubase.h"

class KNMusicModel;
/*!
 * \brief The KNMusicMultiMenu class provides a basic realization for multi
 * item menu.
 */
class KNMusicMultiMenu : public KNMusicMultiMenuBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicMultiMenu widget with given parent.
     * \param parent The parent widget.
     */
    explicit KNMusicMultiMenu(QWidget *parent = 0);

    /*!
     * \brief Implemented from KNMusicMultiMenuBase::setMusicRows().
     */
    void setMusicRows(KNMusicProxyModel *model,
                      const QList<int> &rows,
                      const QModelIndex &clickedIndex) Q_DECL_OVERRIDE;

signals:

public slots:

private slots:
    void retranslate();
    void onActionCopyFilePaths();
    void onActionCopyItems();
    void onActionDelete();

private:
    enum MultiMenuActions
    {
        CopyFilePaths,
        CopyItems,
        Delete,
        MultiMenuActionCount
    };
    QAction *m_actions[MultiMenuActionCount];
    QString m_actionTitles[MultiMenuActionCount];

    KNMusicModel *m_model;
    QModelIndex m_itemIndex;
    QList<int> m_rows;
};

#endif // KNMUSICMULTIMENU_H
