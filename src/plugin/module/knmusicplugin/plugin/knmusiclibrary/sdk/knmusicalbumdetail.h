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
class KNConnectionHandler;
class KNSideShadowWidget;
class KNMusicAlbumTitle;
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
    void updateFoldEndValue(const QRect &position,
                            const int &iconSize);

    int sizeParameter() const;
    void setSizeParameter(int sizeParameter);

signals:
    void requireShowAlbum(QPoint indexPoint);
    void requireClearSelection();

public slots:
    void displayAlbumDetail(const QModelIndex &index);
    void foldAlbumDetail();
    void flyAwayAlbumDetail();
    void scrollToSourceRow(const int &row);

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void onActionExpandStep1(const QVariant &position);
    void onActionExpandStep1InFinished();
    void onActionExpandFinished();
    void onActionContentMove(const QVariant &position);
    void onActionFold(const QVariant &position);
    void onActionFoldFinished();
    void onActionFlyAway(const QVariant &position);
    void onActionFlyAwayFinished();
    void onActionAlbumRemoved(const QModelIndex &removedIndex);
    void onActionAskToFold();
    void showContentWidgets();
    void hideContentWidgets();
    void onActionShowAlbumArt();
    void onActionHideAlbumArt();
    void updateAlbumCaptions();

private:
    inline void initialInfrastructure();
    inline void initialAlbumShadow();
    inline void initialAlbumContentWidgets();
    inline void initialAnimations();
    inline void initialShortCuts();
    inline void generateAlbumArtAnimation(QPropertyAnimation *anime);
    inline void generateContentAnimation(QPropertyAnimation *anime);
    inline void generateStep1FinalPosition(QRect &albumArtGeometry,
                                           QRect &contentGeometry);
    inline void generateStep2FinalPosition(QRect &albumArtGeometry,
                                           QRect &contentGeometry);
    inline void stopAllAnimations();
    inline void stopShowHideArtworkAnimations();
    inline void updateWidgetGeometries();
    inline void updateShadowGeometries(const QRect &contentPosition);
    KNMusicAlbumTitle *m_albumArt;
    QLabel *m_albumTitle, *m_albumDetails;
    QWidget *m_albumContent;
    KNMusicAlbumTreeView *m_albumTreeView;
    KNMusicAlbumModel *m_albumModel;
    KNMusicLibraryModel *m_libraryModel;
    KNSideShadowWidget *m_rightShadow, *m_leftShadow;
    QRect m_animeStartRect;
    QPropertyAnimation *m_albumArtIn1, *m_albumContentIn1,
                       *m_albumArtIn2, *m_albumContentIn2,
                       *m_albumArtOut, *m_albumContentOut,
                       *m_albumArtFlyAway, *m_albumContentFlyAway,
                       *m_showAlbumArtLabel, *m_showAlbumContent,
                       *m_hideAlbumArtLabel, *m_hideAlbumContent;
    QGraphicsOpacityEffect *m_opacityEffect;
    QParallelAnimationGroup *m_expandStep1, *m_expandStep2,
                            *m_foldAnime, *m_showAlbumArt, *m_hideAlbumArt,
                            *m_flyAwayAnime;
    QSequentialAnimationGroup *m_expandAnime;
    KNConnectionHandler *m_detailHandler;
    QEasingCurve m_inCurve;
    QModelIndex m_currentIndex;
    bool m_pressed=false, m_backgroundAnime=true;

    int m_iconSize, m_sizeParameter=0, m_shadowWidth=15;
};

#endif // KNMUSICALBUMDETAIL_H
