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

#ifndef KNMUSICPLAYLISTLISTVIEW_H
#define KNMUSICPLAYLISTLISTVIEW_H

#include "knmusiccategorylistviewbase.h"

/*!
 * \brief The KNMusicPlaylistListView class is a widget which can show the list
 * of playlists in the playlist list model. It will use a delegate to avoid the
 * icon selected feature in the default style.
 */

class KNMusicPlaylistListView : public KNMusicCategoryListViewBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistListView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicPlaylistListView(QWidget *parent = 0);

    /*!
     * \brief Get the title text of the current playlist.
     * \return The title text string.
     */
    QString currentPlaylistTitle() const;

signals:
    /*!
     * \brief When this widget is shown, this signal is called for asking to
     * initial the playlist.
     */
    void requireInitialPlaylist();

protected:
    /*!
     * \brief Reimplemented from KNMusicCategoryListViewBase::dragEnterEvent().
     */
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryListViewBase::dragMoveEvent().
     */
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryListViewBase::dragLeaveEvent().
     */
    void dragLeaveEvent(QDragLeaveEvent *) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryListViewBase::dropEvent().
     */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicCategoryListViewBase::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    QAbstractItemView::DropIndicatorPosition dropPosition(const QPoint &pos,
            const QRect &rect) const;
    inline bool dropOn(QDropEvent *event,
                       int &dropRow,
                       QModelIndex &dropIndex);
    int m_dropSideMargin;
};

#endif // KNMUSICPLAYLISTLISTVIEW_H
