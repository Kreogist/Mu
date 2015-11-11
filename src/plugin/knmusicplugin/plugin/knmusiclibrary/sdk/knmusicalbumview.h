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

#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QAbstractItemView>

class QTimeLine;
class KNMusicCategoryProxyModel;
class KNMusicAlbumModel;
class KNMusicAlbumDetail;
/*!
 * \brief The KNMusicAlbumView class is a special category view for album
 * category. It can display all the album with a block. When you click on one
 * album, it will expand it to show the detail of the album.
 */
class KNMusicAlbumView : public QAbstractItemView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumView widget with given parent.
     * \param parent The parent widget.
     */
    explicit KNMusicAlbumView(QWidget *parent = 0);

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
     * \brief This function works very familiar with the scrollTo function. But
     * this function won't move to the target position by animation. It will
     * simply change the value of vertical scroll bar.
     * \param index The target index.
     * \param hint The shown hint.
     */
    void locateTo(const QModelIndex &index,
                  ScrollHint hint = EnsureVisible);

    /*!
     * \brief Reimplemented from QAbstractItemView::visualRect().
     */
    QRect visualRect(const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractItemView::setModel().
     */
    void setModel(QAbstractItemModel *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Select an album to display the detail. It will launch the
     * animation right after calling this function.
     * \param albumIndex The index of the album in album model.
     */
    void selectAlbum(const QModelIndex &albumIndex);

    KNMusicAlbumDetail *albumDetail() const;

signals:

public slots:
    /*!
     * \brief clearSelection
     */
    void clearSelection();

    void setAlbumDetail(KNMusicAlbumDetail *albumDetail);

protected:
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

private slots:
    void retranslate();
    void onActionScrolling();
    void displayAlbum(const QPoint &point);
    void onActionSearch();

private:
    inline int indexScrollBarValue(const QModelIndex &index,
                                   ScrollHint hint = EnsureVisible);
    inline QRect itemContentRect(const QModelIndex &index) const;
    inline void paintAlbum(QPainter &painter,
                           const int &x,
                           const int &y,
                           const QModelIndex &index);
    inline void updateUIElements();
    inline QPixmap generateShadow(int shadowWidth, int shadowHeight);
    QPixmap m_shadowSource, m_albumArtShadow, m_albumBase, m_scaledAlbumBase,
            m_noAlbumArt, m_scaledNoAlbumArt;
    const QModelIndex m_nullIndex;
    QModelIndex m_selectedIndex, m_mouseDownIndex;
    QTimeLine *m_scrollAnime;
    KNMusicCategoryProxyModel *m_proxyModel;
    KNMusicAlbumModel *m_model;
    KNMusicAlbumDetail *m_albumDetail;
    const int m_spacing, m_itemMinimalWidth, minimalWidth;
    int m_lineCount, m_textSpacing, m_itemHeight, m_itemWidth,
        m_itemSpacingHeight, m_itemSpacingWidth, m_maxColumnCount;
};

#endif // KNMUSICALBUMVIEW_H
