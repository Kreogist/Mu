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

#include <QEasingCurve>
#include <QModelIndex>

#include "knconnectionhandler.h"

#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class KNMusicTab;
class KNMouseUnclickableWidget;
class KNMusicAlbumTitle;
class KNSideShadowWidget;
class KNScrollLabel;
class KNMusicAlbumListView;
class KNMusicLibraryModel;
class KNMusicLibraryImageManager;
/*!
 * \brief The KNMusicAlbumDetail widget can display a specific album detail
 * information and it could also contains the ability to work as a music tree
 * view.\n
 * This is the official album detail display widget. It should work with album
 * view.
 */
class KNMusicAlbumDetail : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicAlbumDetail widget with given parent and music
     * tab.
     * \param parent The parent widget pointer.
     * \param tab The album music tab pointer.
     */
    explicit KNMusicAlbumDetail(QWidget *parent = 0, KNMusicTab *tab = 0);

    /*!
     * \brief Update the album captions if any of it is changed.
     */
    void updateAlbumCaptions();

    /*!
     * \brief Update the album artwork when the artwork is changed.
     */
    void updateAlbumArtwork();

signals:
    /*!
     * \brief When the user click on any other point outside of the album detail
     * widget, this signal will be emitted for asking showing the album at the
     * specific point.
     * \param indexPoint The mouse clicked point.
     */
    void requireShowAlbum(QPoint indexPoint);
    void requireClearSelection();

public slots:
    void setLibraryModel(KNMusicLibraryModel *model);
    void setAnimeParameter(const QRect &albumRect, const int &iconSize);
    void updateFoldEndValue(const QRect &position, const int &iconSize);
    void displayAlbumDetail(const QModelIndex &index);
    void foldAlbumDetail();
    void flyAwayAlbumDetail();
    void scrollToSourceRow(const int &row);
    void setImageManager(KNMusicLibraryImageManager *imageManager);
    void onActionAlbumArtUpdate(const QModelIndex &updatedIndex);
    void onActionAlbumRemoved(const QModelIndex &removedIndex);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::hideEvent().
     */
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionExpandFinished();
    void onActionShowAlbumArt();
    void onActionHideAlbumArt();
    void onActionExpandStep1(const QVariant &position);
    void onActionExpandStep1InFinished();
    void onActionContentMove(const QVariant &position);
    void onActionFoldFinished();
    void onActionFold(const QVariant &position);
    void onActionFlyAwayFinished();
    void onActionFlyAway(const QVariant &position);

private:
    inline void showContentWidgets();
    inline void hideContentWidgets();
    inline void generateStep1FinalPosition(QRect &albumArtGeometry,
                                           QRect &contentGeometry);
    inline void generateStep2FinalPosition(QRect &albumArtGeometry,
                                           QRect &contentGeometry);
    inline void stopAllAnimations();
    inline void stopShowHideArtworkAnimations();
    inline void updatePanelSize();
    inline void updateWidgetGeometries();
    inline void updateShadowGeometries(const QRect &contentPosition);
    inline void updateExpandAlbumParameter();
    inline void updateFoldAlbumParameter();
    inline QPropertyAnimation *generateAnime(
            QObject *target,
            QEasingCurve::Type type=QEasingCurve::OutCubic);
    KNConnectionHandler m_detailHandler;
    QModelIndex m_currentIndex;
    QEasingCurve m_inCurve;
    QRect m_animeStartRect;
    KNMouseUnclickableWidget *m_albumContent;
    KNMusicAlbumTitle *m_albumArt;
    KNSideShadowWidget *m_rightShadow, *m_leftShadow;
    KNScrollLabel *m_albumTitle, *m_albumDetails;
    KNMusicAlbumListView *m_albumListView;
    KNMusicLibraryModel *m_libraryModel;
    QSequentialAnimationGroup *m_expandAnime;
    QParallelAnimationGroup *m_expandStep1, *m_expandStep2,
                            *m_foldAnime, *m_showAlbumArt, *m_hideAlbumArt,
                            *m_flyAwayAnime;
    QPropertyAnimation *m_albumArtIn1, *m_albumContentIn1,
                       *m_albumArtIn2, *m_albumContentIn2,
                       *m_albumArtOut, *m_albumContentOut,
                       *m_albumArtFlyAway, *m_albumContentFlyAway,
                       *m_showAlbumArtLabel, *m_showAlbumContent,
                       *m_hideAlbumArtLabel, *m_hideAlbumContent;

    KNMusicLibraryImageManager *m_imageManager;
    int m_iconSize, m_panelSize;
    bool m_backgroundAnime, m_pressed;
};

#endif // KNMUSICALBUMDETAIL_H
