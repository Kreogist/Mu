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

#ifndef KNMUSICSEARCHBASE_H
#define KNMUSICSEARCHBASE_H

#include "knmusicutil.h"

#include <QObject>

using namespace MusicUtil;

class QAction;
class KNMusicProxyModel;
/*!
 * \brief The KNMusicSearchBase class provides the base port of the music search
 * module. It also provides some basic structures and signals of the search
 * module.
 */
class KNMusicSearchBase : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicSearchBase widget.
     * \param parent The parent widget.
     */
    KNMusicSearchBase(QObject *parent = 0) : QObject(parent){}

    /*!
     * \brief Get the search widget pointer.
     * \return The search widget pointer.
     */
    virtual QWidget *widget()=0;

    /*!
     * \brief Get all the search rules generate by the search box.
     * \return The search rule list.
     */
    virtual QList<KNMusicSearchBlock> rules()=0;

signals:
    /*!
     * \brief When the search rules update, this signal will be emitted.
     */
    void requireSearch();

public slots:
    /*!
     * \brief Clear up all the search and emit requireSearch() signal to update
     * the data.
     */
    virtual void clear()=0;

    /*!
     * \brief Search the particular rules. It should automatically replace the
     * current search.
     * \param blocks The search rule blocks.
     */
    virtual void search(const QList<KNMusicSearchBlock> &blocks)=0;

    /*!
     * \brief When the user pressed shortcut key at somewhere else, they can use
     * this slot to set the focus source widget and start searching.
     * When user hit ESC, it will automatically give the focus back to the
     * source widget.
     * \param sourceWidget The widget where the focus comes from.
     */
    virtual void onActionSearchShortcut(QWidget *sourceWidget)=0;
};

#endif // KNMUSICSEARCHBASE_H
