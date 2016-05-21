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

#ifndef KNMUSICPLAYLISTVIEWER_H
#define KNMUSICPLAYLISTVIEWER_H

#include <QWidget>

class QLabel;
class KNOpacityAnimeButton;
class KNScrollLabel;
class KNConnectionHandler;
class KNSideShadowWidget;
class KNMusicPlaylistModel;
class KNMusicTab;
class KNMusicPlaylistTreeView;
/*!
 * \brief The KNMusicPlaylistViewer class provides a widget which could display
 * the detail information of a playlist. Set the playlist model to this widget
 * it will automatically update the data from the playlist model.
 */
class KNMusicPlaylistViewer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicPlaylistViewer widget.
     * \param parent The parent widget pointer.
     * \param tab The tab pointer, this will be given to the tree view.
     */
    explicit KNMusicPlaylistViewer(QWidget *parent = 0,
                                   KNMusicTab *tab = 0);
    ~KNMusicPlaylistViewer();

    /*!
     * \brief Get the current playlist model pointer.
     * \return The playlist model pointer of the tree view widget.
     */
    KNMusicPlaylistModel *playlist();

    /*!
     * \brief Set the playlist model to the tree view widget.
     * \param model The model pointer.
     */
    void setPlaylist(KNMusicPlaylistModel *model);

    /*!
     * \brief Select a specific song on the tree view.
     * \param sourceIndex The index of the song in the source music model.
     */
    void selectSong(const QModelIndex &sourceIndex);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionModelRowCountChanged();
    void onActionSearch();
    void onActionPlayCurrent();
    void onActionShuffle();
    void onActionAddToPlaylist();
    void onActionRemoveFromPlaylist();
    void updateTitle();

private:
    inline void updateDetailInfo();
    inline KNOpacityAnimeButton *generateButton(const QString &iconPath);
    QString m_songCount[3],
            m_minuteCount[3],
            m_hourCount[2],
            m_hourCountWithoutMinuate[2],
            m_searchCount[3];
    QString m_searchResultIn;

    KNMusicPlaylistTreeView *m_treeView;
    KNScrollLabel *m_title;
    QLabel *m_detail;
    KNSideShadowWidget *m_leftShadow;
    KNConnectionHandler *m_modelLinkHandler;
    KNOpacityAnimeButton *m_playPlaylist, *m_shufflePlaylist, *m_addToPlaylist,
                         *m_removeFromPlaylist;
};

#endif // KNMUSICPLAYLISTVIEWER_H
