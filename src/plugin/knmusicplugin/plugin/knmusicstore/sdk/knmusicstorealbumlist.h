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
#ifndef KNMUSICSTOREALBUMLIST_H
#define KNMUSICSTOREALBUMLIST_H

#include <QAbstractItemView>

class KNHighLightLabel;
class KNSideShadowWidget;
class KNMusicStoreAlbumListModel;
/*!
 * \brief The KNMusicStoreAlbumList class provides a five album graphics display
 * widget for main widget. It could manage all kinds of the data which the
 * module provides.
 */
class KNMusicStoreAlbumList : public QAbstractItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicStoreAlbumList widget.
     * \param parent The parent widgets.
     */
    explicit KNMusicStoreAlbumList(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractItemView::indexAt().
     */
    QModelIndex indexAt(const QPoint &point) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::scrollTo().
     */
    void scrollTo(const QModelIndex &index,
                  ScrollHint hint = EnsureVisible) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRect().
     */
    QRect visualRect(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::setModel().
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief When user clicked an album, this signal will be emitted.
     * \param albumData THe album information data.
     */
    void albumClick(QVariant albumData);

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractItemView::mouseMoveEvent().
     */
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::moveCursor().
     */
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::horizontalOffset().
     */
    int horizontalOffset() const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::verticalOffset().
     */
    int verticalOffset() const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::isIndexHidden().
     */
    bool isIndexHidden(const QModelIndex &index) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::setSelection().
     */
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command)
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRegionForSelection().
     */
    QRegion visualRegionForSelection(const QItemSelection &selection) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief Reimplemented from QAbstractItemView::updateGeometries().
     */
    void updateGeometries() Q_DECL_OVERRIDE;

private:
    inline int indexScrollBarValue(const QModelIndex &index,
                                   QAbstractItemView::ScrollHint hint);
    inline QRect itemContentRect(const QModelIndex &index) const;

    QPersistentModelIndex m_pressedIndex;
    KNMusicStoreAlbumListModel *m_model;
    KNSideShadowWidget *m_leftShadow, *m_rightShadow;
    int m_itemHeight;
};

#endif // KNMUSICSTOREALBUMLIST_H
