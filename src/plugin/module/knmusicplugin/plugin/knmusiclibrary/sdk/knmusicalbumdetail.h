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
#ifndef KNMUSICALBUMDETAIL_H
#define KNMUSICALBUMDETAIL_H

#include <QModelIndex>
#include <QEasingCurve>
#include <QWidget>

class QGraphicsOpacityEffect;
class QLabel;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class KNSideShadowWidget;
class KNMusicAlbumModel;
class KNMusicCategoryModel;
class KNMusicLibraryModel;
class KNMusicAlbumTreeView;
class KNMusicAlbumDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumDetail(QWidget *parent = 0);
    void setAlbumModel(KNMusicCategoryModel *model);
    void setLibraryModel(KNMusicLibraryModel *model);
    void setAnimeParameter(const QRect &albumRect,
                           const int &iconSize);
    void displayAlbumIndex(const QModelIndex &index);

    int sizeParameter() const;
    void setSizeParameter(int sizeParameter);

signals:

public slots:
    void foldDetail();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionExpandStep1(const QVariant &position);
    void onActionExpandStep1InFinished();
    void onActionExpandStep2(const QVariant &position);
    void onActionFold(const QVariant &position);
    void showContentWidgets();
    void hideContentWidgets();

private:
    void generateStep1FinalPosition(QRect &albumArtGeometry,
                                    QRect &contentGeometry);
    void generateStep2FinalPosition(QRect &albumArtGeometry,
                                    QRect &contentGeometry);
    void updateWidgetGeometries();
    void updateShadowGeometries(const QRect &contentPosition);
    void updateAlbumCaptions();
    QLabel *m_albumArt, *m_albumTitle, *m_albumDetails;
    QWidget *m_albumContent;
    KNMusicAlbumTreeView *m_albumTreeView;
    KNMusicAlbumModel *m_albumModel;
    KNMusicLibraryModel *m_libraryModel;
    KNSideShadowWidget *m_rightShadow, *m_leftShadow;
    QModelIndex m_currentAlbumIndex;
    QRect m_animeStartRect;
    QPropertyAnimation *m_albumArtIn1, *m_albumContentIn1,
                       *m_albumArtIn2, *m_albumContentIn2,
                       *m_albumArtOut, *m_albumContentOut;
    QGraphicsOpacityEffect *m_opacityEffect;
    QParallelAnimationGroup *m_expandStep1, *m_expandStep2,
                            *m_foldAnime;
    QSequentialAnimationGroup *m_expandAnime;
    QEasingCurve m_inCurve;
    QModelIndex m_currentIndex;
    bool m_pressed=false;

    int m_iconSize, m_sizeParameter=0, m_shadowWidth=15;
};

#endif // KNMUSICALBUMDETAIL_H
