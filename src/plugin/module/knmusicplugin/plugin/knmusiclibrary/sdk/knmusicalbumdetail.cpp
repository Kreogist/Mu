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
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPolygonF>
#include <QTransform>

#include "knmusicalbummodel.h"
#include "knmusiclibrarymodel.h"

#include "knmusicalbumdetail.h"

#include <QDebug>

KNMusicAlbumDetail::KNMusicAlbumDetail(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    //Set the background color.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    setPalette(pal);
    //Initial the album content.
    //Initial this first, and we don't need to raise it after.
    m_albumContent=new QWidget(this);
    m_albumContent->setAutoFillBackground(true);
    pal=m_albumContent->palette();
    pal.setColor(QPalette::Window, QColor(255,255,255,240));
    m_albumContent->setPalette(pal);
    //Initial the album art.
    m_albumArt=new QLabel(this);
    m_albumArt->setScaledContents(true);

    //Initial expand animation.
    m_expandAnime=new QSequentialAnimationGroup(this);
    //Initial expand step 1.
    m_expandStep1=new QParallelAnimationGroup(this);
    //Initial artwork-in step 1.
    m_albumArtIn1=new QPropertyAnimation(m_albumArt, "geometry", this);
    m_albumArtIn1->setEasingCurve(QEasingCurve::OutCubic);
    m_expandStep1->addAnimation(m_albumArtIn1);
    //Initial content-in step 1.
    m_albumContentIn1=new QPropertyAnimation(m_albumContent, "geometry", this);
    m_albumContentIn1->setEasingCurve(QEasingCurve::OutCubic);
    m_expandStep1->addAnimation(m_albumContentIn1);
    connect(m_expandStep1, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionAlbumArtInFinished);
    m_expandAnime->addAnimation(m_expandStep1);

    //Initial expand step 2.
    m_expandStep2=new QParallelAnimationGroup(this);
    //Initial artwork-in step 1.
    m_albumArtIn2=new QPropertyAnimation(m_albumArt, "geometry", this);
    m_albumArtIn2->setEasingCurve(QEasingCurve::OutCubic);
    m_expandStep2->addAnimation(m_albumArtIn2);
    //Initial content-in step 1.
    m_albumContentIn2=new QPropertyAnimation(m_albumContent, "geometry", this);
    m_albumContentIn2->setEasingCurve(QEasingCurve::OutCubic);
    m_expandStep2->addAnimation(m_albumContentIn2);
    m_expandAnime->addAnimation(m_expandStep2);
}

void KNMusicAlbumDetail::setAlbumModel(KNMusicCategoryModel *model)
{
    //Translate the model to album model.
    m_albumModel=static_cast<KNMusicAlbumModel *>(model);
}

void KNMusicAlbumDetail::setLibraryModel(KNMusicLibraryModel *model)
{
    m_libraryModel=model;
}

void KNMusicAlbumDetail::setAnimeParameter(const QRect &albumRect,
                                           const int &iconSize)
{
    m_iconSize=iconSize;
    m_animeStartRect=albumRect;
}

void KNMusicAlbumDetail::displayAlbumIndex(const QModelIndex &index)
{
    //Set the pixmap.
    m_albumArt->setPixmap(m_libraryModel->artwork(m_albumModel->data(index, CategoryArtworkKeyRole).toString()));
    //Set the position.
    QRect albumArtStartRect(m_animeStartRect.x(),
                            m_animeStartRect.y(),
                            m_iconSize,
                            m_iconSize);
    //Set the album art and album content to the initial geometry.
    m_albumArt->setGeometry(albumArtStartRect);
    m_albumContent->setGeometry(albumArtStartRect);
    //Set the start value of step 1.
    m_albumArtIn1->setStartValue(albumArtStartRect);
    m_albumContentIn1->setStartValue(albumArtStartRect);
    //Set the step 1 animation final position.
    int finalSizeParameter=(m_sizeParameter>>2)*3;
    QRect albumArtStepFinal=QRect((width()-(finalSizeParameter<<1))>>1,
                                   (height()-finalSizeParameter)>>1,
                                   finalSizeParameter,
                                   finalSizeParameter),
          albumContentStepFinal=QRect(width()>>1,
                                       (height()-finalSizeParameter)>>1,
                                       finalSizeParameter,
                                       finalSizeParameter);
    m_albumArtIn1->setEndValue(albumArtStepFinal);
    m_albumContentIn1->setEndValue(albumContentStepFinal);
    //Set the step 2 animation start position.
    m_albumArtIn2->setStartValue(albumArtStepFinal);
    m_albumContentIn2->setStartValue(albumContentStepFinal);
    //Set the step 2 animation final position.
    generateStep2FinalPosition(albumArtStepFinal,
                               albumContentStepFinal);
    m_albumArtIn2->setEndValue(albumArtStepFinal);
    m_albumContentIn2->setEndValue(albumContentStepFinal);
    //Make album art up.
    m_albumArt->raise();
    //Show the widget.
    show();
    //Start animation.
    m_expandAnime->start();
}

void KNMusicAlbumDetail::foldDetail()
{
    ;
}

void KNMusicAlbumDetail::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Update the child widget geometries.
    updateWidgetGeometries();
}

void KNMusicAlbumDetail::onActionAlbumArtInFinished()
{
    //Make it up.
    m_albumContent->raise();
}

void KNMusicAlbumDetail::generateStep2FinalPosition(QRect &albumArtGeometry,
                                                    QRect &contentGeometry)
{
    int finalSizeParameter=(m_sizeParameter>>2)*3;
    albumArtGeometry=QRect((width()-finalSizeParameter-(finalSizeParameter>>1))>>1,
                           (height()-finalSizeParameter)>>1,
                           finalSizeParameter,
                           finalSizeParameter);
    contentGeometry=QRect(albumArtGeometry.x()+(finalSizeParameter>>1),
                          albumArtGeometry.y(),
                          finalSizeParameter,
                          finalSizeParameter);
}

void KNMusicAlbumDetail::updateWidgetGeometries()
{
    QRect albumArtGeometry, contentGeometry;
    generateStep2FinalPosition(albumArtGeometry, contentGeometry);
    //Set the geometry.
    m_albumArt->setGeometry(albumArtGeometry);
    m_albumContent->setGeometry(contentGeometry);
}

int KNMusicAlbumDetail::sizeParameter() const
{
    return m_sizeParameter;
}

void KNMusicAlbumDetail::setSizeParameter(int sizeParameter)
{
    m_sizeParameter=sizeParameter;
    //Update geometries.
    updateWidgetGeometries();
}

