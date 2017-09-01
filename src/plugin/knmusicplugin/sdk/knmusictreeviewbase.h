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

#ifndef KNMUSICTREEVIEWBASE_H
#define KNMUSICTREEVIEWBASE_H

#include "knmousesensetreeview.h"

class KNMusicModel;
class KNMusicProxyModel;
class KNMusicTab;
/*!
 * \brief The KNMusicTreeViewBase class is the basic tree view of all the music
 * tree view. It provides mouse sense alternative row color. It can
 * automatically link to the solo and multi menu to process the music row.\n
 * Remember this is only a base class, it means you have to reimplement it. Set
 * the object name and called onActionThemeUpdate() slot.
 */
class KNMusicTreeViewBase : public KNMouseSenseTreeView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicTreeViewBase widget.
     * \param parent The parent widget.
     */
    explicit KNMusicTreeViewBase(QWidget *parent = 0,
                                 KNMusicTab *tab = 0);
    ~KNMusicTreeViewBase();

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
    void selectSourceSong(int sourceRow);

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
    void scrollToSourceRow(int sourceRow);

    /*!
     * \brief Scroll the tree view to the position of a specific row.
     * \param row The row of the index.
     */
    void scrollToRow(int row);

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
     * \brief Reimplemented from KNMouseSenseTreeView::drawRow().
     */
    void drawRow(QPainter *painter,
                 const QStyleOptionViewItem &options,
                 const QModelIndex &index) const  Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::startDrag().
     */
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::dragEnterEvent().
     */
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::dragMoveEvent().
     */
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::dragLeaveEvent().
     */
    void dragLeaveEvent(QDragLeaveEvent *) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::dropEvent().
     */
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::keyReleaseEvent().
     */
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reset the header column state to a default state. Including the
     * width and visible.
     */
    virtual void resetHeaderState();

    /*!
     * \brief Move one column to the first column.
     * \param logicalIndex The logical index of the header column.
     */
    void moveToFirst(const int &logicalIndex);

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::event().
     */
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMouseSenseTreeView::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Ask to play an index of the music model.
     * \param index The music proxy index in the music model.
     */
    void playIndex(const QModelIndex &index);

    /*!
     * \brief Set whether we should process the drag move event.
     * \param dropInline To let the tree view process the drag move event,
     * return true. Or else return false.
     */
    void setAcceptDragMove(bool dropInline);

private slots:
    void onActionActivate(const QModelIndex &index);
    void playCurrent();
    void removeCurrent();

private:
    QAbstractItemView::DropIndicatorPosition dropPosition(const QPoint &pos,
            const QRect &rect) const;
    inline void scrollToIndex(const QModelIndex &proxyIndex);
    inline bool dropOn(QDropEvent *event, int &dropRow);
    inline void showMenu(QPoint position,
                         const QModelIndex &pressedIndex=QModelIndex());
    void showSoloMenu(const QPoint &position, const QModelIndex &pressedIndex);
    void showMultiMenu(const QPoint &position);
    bool showDetailTooltip(const QPoint &indexPosition);

    KNMusicTab *m_musicTab;
    KNMusicProxyModel *m_proxyModel;
    int m_dragMoveRow;
    QAbstractItemView::DropIndicatorPosition m_dragIndicatorPos;

    bool m_initialLoad, m_pressed, m_notAcceptDragMove;
};

#endif // KNMUSICTREEVIEWBASE_H
