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

#ifndef KNMUSICLISTVIEWBASE_H
#define KNMUSICLISTVIEWBASE_H

#include <QListView>

class QTimeLine;
class KNMusicTab;
class KNMusicModel;
class KNMusicProxyModel;
/*!
 * \brief The KNMusicListViewBase class provides a listview which will be used
 * for display a music model. It supports all the basic operation of the music
 * models.
 */
class KNMusicListViewBase : public QListView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicListViewBase widget.
     * \param parent The parent widget.
     * \param tab The music tab object.
     */
    explicit KNMusicListViewBase(QWidget *parent = 0,
                                  KNMusicTab *tab = 0);

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

    /*!
     * \brief Set the music model. DO NOT use setModel() function. Use this
     * instead.
     * \param musicModel The music model pointer.
     */
    void setMusicModel(KNMusicModel *musicModel);

    /*!
     * \brief Select a song by providing the song row of the source model.
     * \param sourceRow The source row of the song. If the row is invalid or
     * there's no valid model, it will do nothing.
     */
    void selectSourceSong(const int &sourceRow);

    /*!
     * \brief This is a override function.\n
     * Select a song by providing one index of the the song row of the source
     * model.
     * \param index One of the index of the source row.
     */
    inline void selectSourceSong(const QModelIndex &index)
    {
        selectSourceSong(index.row());
    }

    /*!
     * \brief Scroll the tree view to position of a index via given the row in
     * the source model.
     * \param sourceRow The index row in source model.
     */
    void scrollToSourceRow(const int &sourceRow);

    /*!
     * \brief Scroll the tree view to the position of a specific row.
     * \param row The row of the index.
     */
    void scrollToRow(const int &row);

    /*!
     * \brief sortByColumn
     * \param column
     * \param order
     */
    void sortByColumn(int column, Qt::SortOrder order);

signals:
    /*!
     * \brief When dragging is started, this signal will be emitted.
     */
    void startDraggingSong();

    /*!
     * \brief When dragging is stopped, this signal will be emitted.
     */
    void endDraggingSong();

public slots:

protected:
    /*!
     * \brief Reimplemented from QListView::startDrag().
     */
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimpelemnt from QListView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::keyReleaseEvent().
     */
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::event().
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTreeView::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionActivate(const QModelIndex &index);
    void onActionMouseInOut(int frame);
    void playCurrent();
    void removeCurrent();
    void renameCurrent();

private:
    inline void scrollToIndex(const QModelIndex &proxyIndex);
    inline void playIndex(const QModelIndex &index);
    inline void startAnime(int endFrame);
    void showSoloMenu(const QPoint &position);
    void showMultiMenu(const QPoint &position);
    bool showDetailTooltip(const QPoint &indexPosition);

    KNMusicTab *m_musicTab;
    KNMusicProxyModel *m_proxyModel;
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
    bool m_pressed;
};

#endif // KNMUSICLISTVIEWBASE_H
