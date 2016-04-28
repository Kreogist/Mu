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
#ifndef KNMUSICMINIPLAYERLYRICS_H
#define KNMUSICMINIPLAYERLYRICS_H

#include <QUrl>

#include "knmusichscrolllyrics.h"

/*!
 * \brief The KNMusicMiniPlayerLyrics class is a simple implemented widget from
 * the horizontal lyrics widget.\n
 * It will accept the drag and drop event and emit a signal for the drag'n drop
 * url links.
 */
class KNMusicMiniPlayerLyrics : public KNMusicHScrollLyrics
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicMiniPlayerLyrics widget.
     * \param parent The parent widget.
     */
    explicit KNMusicMiniPlayerLyrics(QWidget *parent = 0);

signals:
    /*!
     * \brief When urls dropped on this widget, this signal will be emitted.
     * \param urls The url list.
     */
    void urlsDropped(QList<QUrl> urls);

public slots:

protected:
    /*!
     * \brief Reimpelment from KNMusicHScrollLyrics::dragEnterEvent().
     */
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelment from KNMusicHScrollLyrics::dropEvent().
     */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
};

#endif // KNMUSICMINIPLAYERLYRICS_H
