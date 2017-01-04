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

#ifndef KNMUSICSTOREHOMEITEMVIEW_H
#define KNMUSICSTOREHOMEITEMVIEW_H

#include <QAbstractItemView>

class QTimeLine;
class KNMusicStoreHomeListModel;
/*!
 * \brief The KNMusicStoreHomeItemView class provides the basic function of the
 * music store item model. This view is only an abstract implementation. It only
 * provides the management of the model, however, for the specific user
 * interface part, each view needs to implement their own functions.
 */
class KNMusicStoreHomeItemView : public QAbstractItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMusicStoreHomeItemView widget.
     * \param parent The parent widget.
     */
    explicit KNMusicStoreHomeItemView(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractItemView::scrollTo().
     */
    void scrollTo(const QModelIndex &index,
                  ScrollHint hint = EnsureVisible) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::moveCursor().
     */
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::setSelection().
     */
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::setModel().
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractItemView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::horizontalOffset().
     */
    int horizontalOffset() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::verticalOffset().
     */
    int verticalOffset() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::isIndexHidden().
     */
    bool isIndexHidden(const QModelIndex &index) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRegionForSelection().
     */
    QRegion visualRegionForSelection(const QItemSelection &selection) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::wheelEvent().
     */
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStoreHomeItemView::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the casted list model.
     * \return The music store list model pointer, when there is no model, it
     * will be nullptr.
     */
    KNMusicStoreHomeListModel *listModel() const;

    /*!
     * \brief Get the customized scroll bar widget pointer.
     * \return The widget pointer.
     */
    QScrollBar *scrollBar();

    /*!
     * \brief Get the current hover index.
     * \return The hover index. This should be used in the paint event.
     */
    QModelIndex hoverIndex() const;

private slots:
    void onMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    QModelIndex m_hoverIndex;
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
    KNMusicStoreHomeListModel *m_listModel;
};

#endif // KNMUSICSTOREHOMEITEMVIEW_H
