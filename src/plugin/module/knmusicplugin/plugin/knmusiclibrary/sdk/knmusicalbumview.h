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
class KNMusicLibraryModel;
class KNMusicAlbumModel;
class KNMusicCategoryProxyModel;
class KNMusicAlbumDetail;
class KNMusicAlbumView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit KNMusicAlbumView(QWidget *parent = 0);
    QModelIndex indexAt(const QPoint &point) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    void locateTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QRect visualRect(const QModelIndex &index) const;
    void setModel(QAbstractItemModel *model);
    void setLibraryModel(KNMusicLibraryModel *libraryModel);
    KNMusicAlbumDetail *albumDetail() const;
    void setAlbumDetail(KNMusicAlbumDetail *albumDetail);
    void selectAlbum(QModelIndex albumIndex);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                           Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

protected slots:
    void updateGeometries();

private slots:
    void displayAlbum(const QPoint &point);
    void onActionFoldComplete();

private:
    inline void paintAlbum(QPainter &painter,
                           const QRect &rect,
                           const QModelIndex &index);
    int indexScrollBarValue(const QModelIndex &index,
                            ScrollHint hint = EnsureVisible);
    QRect itemContentRect(const QModelIndex &index) const;
    void updateParameters();
    KNMusicAlbumModel *m_model=nullptr;
    KNMusicCategoryProxyModel *m_proxyModel=nullptr;
    QTimeLine *m_scrollTimeLine;
    KNMusicAlbumDetail *m_albumDetail;
    QModelIndex m_selectedIndex, m_mouseDownIndex;
    QColor m_backgroundColor=QColor(0x30, 0x30, 0x30);
    int m_outBrightness=0x30;
    //View parameters.
    int m_lineCount=0,
        m_spacing=30,
        m_itemMinimalWidth=124,
        m_itemHeight=154,
        m_itemWidth=134,
        m_itemIconSize=124,
        m_itemSpacingHeight=m_spacing+m_itemHeight,
        m_itemSpacingWidth=m_spacing+m_itemWidth,
        m_maxColumnCount=0;
};

#endif // KNMUSICALBUMVIEW_H
