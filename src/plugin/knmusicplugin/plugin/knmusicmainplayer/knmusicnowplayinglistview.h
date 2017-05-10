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

#ifndef KNMUSICNOWPLAYINGLISTVIEW_H
#define KNMUSICNOWPLAYINGLISTVIEW_H

#include <QListView>

class QTimeLine;
class KNMusicModel;
class KNMusicProxyModel;
/*!
 * \brief The KNMusicNowPlayingListView class provides the playing playlist view
 * widget for the main player. It will only show the name of song and the
 * duration of each music.
 */
class KNMusicNowPlayingListView : public QListView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicNowPlayingListView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicNowPlayingListView(QWidget *parent = 0);

    /*!
     * \brief Get the proxy of the model.
     * \return The proxy music model pointer.
     */
    KNMusicProxyModel *proxyModel();

    /*!
     * \brief Get the music model of the model. It will simply return the
     * managing music model of the proxy model.
     * \return The music model pointer.
     */
    KNMusicModel *musicModel();

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QListView::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionActivate(const QModelIndex &index);
    void onActionMouseInOut(int frame);

private:
    inline void playIndex(const QModelIndex &index);
    inline void updateVerticalScrollBarGeometry();
    inline void startAnime(int endFrame);
    QScrollBar *m_vScrollBar;
    QTimeLine *m_mouseAnime;
    int m_currentFrame;
};

#endif // KNMUSICNOWPLAYINGLISTVIEW_H
