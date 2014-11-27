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
#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QPolygonF>
#include <QMouseEvent>
#include <QTransform>

#include "knsideshadowwidget.h"
#include "knconnectionhandler.h"
#include "knmusicalbumtitle.h"
#include "knmusicalbummodel.h"
#include "knmusiclibrarymodel.h"
#include "knmusicalbumtreeview.h"

#include "knmusicalbumdetail.h"

#include <QDebug>

KNMusicAlbumDetail::KNMusicAlbumDetail(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    //Set the background color.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    setPalette(pal);

    //Initial the album content.
    initialInfrastructure();
    initialAlbumShadow();
    //Initial the album content widgets.
    initialAlbumContentWidgets();
    //Initial the animations.
    initialAnimations();
    //Initial the shortcut.
    initialShortCuts();
}

void KNMusicAlbumDetail::setAlbumModel(KNMusicCategoryModel *model)
{
    //Translate the model to album model.
    m_albumModel=static_cast<KNMusicAlbumModel *>(model);
}

void KNMusicAlbumDetail::setLibraryModel(KNMusicLibraryModel *model)
{
    //Save the library model.
    m_libraryModel=model;
    //Set the library model to treeview.
    m_albumTreeView->setMusicModel(m_libraryModel);
    //Set default sort state.
    m_albumTreeView->sortByColumn(TrackNumber, Qt::AscendingOrder);
}

void KNMusicAlbumDetail::setAnimeParameter(const QRect &albumRect,
                                           const int &iconSize)
{
    m_iconSize=iconSize;
    m_animeStartRect=albumRect;
}

void KNMusicAlbumDetail::displayAlbumIndex(const QModelIndex &index)
{
    //Stop all the animation.
    m_foldAnime->stop();
    m_expandAnime->stop();
    //Hide the contents.
    hideContentWidgets();
    //Save the index.
    m_currentIndex=index;
    //Initial the pixmap.
    QPixmap currentPixmap=m_libraryModel->artwork(m_albumModel->data(m_currentIndex, CategoryArtworkKeyRole).toString());
    //Set the pixmap.
    m_albumArt->setPixmap(currentPixmap.isNull()?
                              KNMusicGlobal::instance()->noAlbumArt():currentPixmap);
    //Set category content.
    m_albumTreeView->setCategoryText(m_albumModel->data(m_currentIndex, Qt::DisplayRole).toString());
    //Initial the opacity effect.
    m_opacityEffect->setOpacity(1.0);
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
    QRect albumArtStepFinal, albumContentStepFinal;
    generateStep1FinalPosition(albumArtStepFinal,
                               albumContentStepFinal);
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
    m_rightShadow->hide();
    m_leftShadow->raise();
    m_leftShadow->show();
    //Show the widget.
    show();
    //Set the focus.
    setFocus();
    //Start animation.
    m_expandAnime->start();
}

void KNMusicAlbumDetail::updateFoldEndValue(const QRect &position,
                                            const int &iconSize)
{
    if(m_foldAnime->state()==QAbstractAnimation::Running)
    {
        //Update the icon size.
        m_iconSize=iconSize;
        //Generate the final value.
        QRect endPosition=QRect(position.x(),
                                position.y(),
                                m_iconSize,
                                m_iconSize);
        //Update the final geometry.
        m_albumArtOut->setEndValue(endPosition);
        m_albumContentOut->setEndValue(endPosition);
    }
}

void KNMusicAlbumDetail::foldDetail()
{
    //Cut all the connections.
    m_detailHandler->disConnectAll();
    //If m_animeStartRect is empty, or the state is running means now is running
    //fold animation, we don't need to fold it more.
    if(!m_animeStartRect.isNull()
            && m_foldAnime->state()==QAbstractAnimation::Stopped)
    {
        //Stop all the anime.
        m_expandAnime->stop();
        m_foldAnime->stop();
        //Set the position.
        QRect albumArtEndRect(m_animeStartRect.x(),
                              m_animeStartRect.y(),
                              m_iconSize,
                              m_iconSize);
        //Set the animation start and end value.
        m_albumArtOut->setStartValue(m_albumArt->geometry());
        m_albumArtOut->setEndValue(albumArtEndRect);
        m_albumContentOut->setStartValue(m_albumContent->geometry());
        m_albumContentOut->setEndValue(albumArtEndRect);
        //Hide all the shadows.
        m_rightShadow->hide();
        m_leftShadow->hide();
        //Enable background change.
        m_backgroundAnime=true;
        //Start animation.
        m_foldAnime->start();
    }
}

void KNMusicAlbumDetail::scrollToSourceRow(const int &row)
{
    m_albumTreeView->scrollToSourceSongRow(row);
}

void KNMusicAlbumDetail::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Update the child widget geometries.
    updateWidgetGeometries();
}

void KNMusicAlbumDetail::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    //Set the preesed flag.
    m_pressed=true;
}

void KNMusicAlbumDetail::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    //Check the flag.
    if(m_pressed)
    {
        m_pressed=false;
        if(rect().contains(event->pos()))
        {
            emit requireShowAlbum(event->pos());
        }
    }
}

void KNMusicAlbumDetail::onActionExpandStep1(const QVariant &position)
{
    //Move the shadows.
    updateShadowGeometries(position.toRect());
    //Calculate the progress.
    qreal progress=m_inCurve.valueForProgress((qreal)m_albumArtIn1->currentTime()/m_albumArtIn1->duration());
    if(m_backgroundAnime)
    {
        QPalette pal=palette();
        pal.setColor(QPalette::Window, QColor(0,0,0,progress*200));
        setPalette(pal);
    }
    //Set the opacity effect.
    m_opacityEffect->setOpacity(progress);
}

void KNMusicAlbumDetail::onActionFold(const QVariant &position)
{
    Q_UNUSED(position)
    //Clear the current index.
    m_currentIndex=QModelIndex();
    //Hide contents.
    hideContentWidgets();
    //Calculate the progress.
    qreal progress=1.0-(qreal)m_albumArtOut->currentTime()/(qreal)m_albumArtOut->duration();
    //Set the palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,progress*200));
    setPalette(pal);
    //Set the opacity effect.
    m_opacityEffect->setOpacity(progress);
}

void KNMusicAlbumDetail::onActionFoldFinished()
{
    //Emit finished signal.
    emit foldComplete();
    //Hide the detail widget.
    hide();
}

void KNMusicAlbumDetail::onActionAskToFold()
{
    //Emit a album which must be unavailable to fold the detail.
    emit requireShowAlbum(QPoint(-1,-1));
}

void KNMusicAlbumDetail::onActionContentMove(const QVariant &position)
{
    //Move the shadows.
    updateShadowGeometries(position.toRect());
}

void KNMusicAlbumDetail::onActionExpandStep1InFinished()
{
    //Make it up.
    m_albumContent->raise();
    //Show the shadow.
    m_rightShadow->raise();
    m_rightShadow->show();
    m_leftShadow->hide();
    //Disable the background anime.
    m_backgroundAnime=false;
    //Show the contents.
    showContentWidgets();
}

void KNMusicAlbumDetail::onActionExpandFinished()
{
    m_detailHandler->addConnectionHandle(
                connect(m_albumArt, &KNMusicAlbumTitle::requireShowAlbumArt,
                        this, &KNMusicAlbumDetail::onActionShowAlbumArt));
    m_detailHandler->addConnectionHandle(
                connect(m_albumArt, &KNMusicAlbumTitle::requireHideAlbumArt,
                        this, &KNMusicAlbumDetail::onActionHideAlbumArt));
}

void KNMusicAlbumDetail::showContentWidgets()
{
    //Set the data.
    updateAlbumCaptions();
    //Show the titles.
    m_albumTitle->show();
    m_albumDetails->show();
    //When show the treeview, resize the header first.
    m_albumTreeView->resizeHeader((m_sizeParameter>>2)*3);
    m_albumTreeView->show();
}

void KNMusicAlbumDetail::hideContentWidgets()
{
    m_albumTitle->hide();
    m_albumDetails->hide();
    m_albumTreeView->hide();
}

void KNMusicAlbumDetail::onActionShowAlbumArt()
{
    //Stop all shown animation.
    m_showAlbumArt->stop();
    m_hideAlbumArt->stop();
    //Get the position.
    QRect albumArtStepFinal, albumContentStepFinal;
    generateStep1FinalPosition(albumArtStepFinal,
                               albumContentStepFinal);
    //Set the value.
    m_showAlbumArtLabel->setStartValue(m_albumArt->geometry());
    m_showAlbumContent->setStartValue(m_albumContent->geometry());
    m_showAlbumArtLabel->setEndValue(albumArtStepFinal);
    m_showAlbumContent->setEndValue(albumContentStepFinal);
    //Start hide album.
    m_showAlbumArt->start();
}

void KNMusicAlbumDetail::onActionHideAlbumArt()
{
    //Stop all shown animation.
    m_showAlbumArt->stop();
    m_hideAlbumArt->stop();
    //Get the position.
    QRect albumArtStepFinal, albumContentStepFinal;
    generateStep2FinalPosition(albumArtStepFinal,
                               albumContentStepFinal);
    //Set the value.
    m_hideAlbumArtLabel->setStartValue(m_albumArt->geometry());
    m_hideAlbumContent->setStartValue(m_albumContent->geometry());
    m_hideAlbumArtLabel->setEndValue(albumArtStepFinal);
    m_hideAlbumContent->setEndValue(albumContentStepFinal);
    //Start hide album.
    m_hideAlbumArt->start();
}

void KNMusicAlbumDetail::initialInfrastructure()
{
    //Initial this first, and we don't need to raise it after.
    m_albumContent=new QWidget(this);
    m_albumContent->setAutoFillBackground(true);
    QPalette pal=m_albumContent->palette();
    pal.setColor(QPalette::Window, QColor(255,255,255,240));
    m_albumContent->setPalette(pal);
    //Initial the opacity effects.
    m_opacityEffect=new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1.0);
    m_albumContent->setGraphicsEffect(m_opacityEffect);
    //Initial the album art.
    m_albumArt=new KNMusicAlbumTitle(this);
    m_albumArt->setScaledContents(true);
    //Initial the show and hide artwork connection.
    m_detailHandler=new KNConnectionHandler(this);
}

void KNMusicAlbumDetail::initialAlbumShadow()
{
    //Initial the side shadows.
    m_rightShadow=new KNSideShadowWidget(this);
    m_rightShadow->setDirection(KNSideShadow::RightShadow);
    m_leftShadow=new KNSideShadowWidget(this);
    m_leftShadow->setDirection(KNSideShadow::LeftShadow);
}

void KNMusicAlbumDetail::initialAlbumContentWidgets()
{
    //Initial the content layout for album content.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             m_albumContent);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);
    m_albumContent->setLayout(contentLayout);

    //Initial the caption layout for the title and detail label.
    QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             contentLayout->widget());
    captionLayout->setContentsMargins(25,25,8,14);
    captionLayout->setSpacing(0);
    contentLayout->addLayout(captionLayout);

    //Initial title label.
    m_albumTitle=new QLabel(this);
    QFont captionFont=m_albumTitle->font();
    captionFont.setBold(true);
    captionFont.setPixelSize(21);
    m_albumTitle->setFont(captionFont);
    QPalette captionPalette=m_albumTitle->palette();
    captionPalette.setColor(QPalette::WindowText, QColor(0,0,0));
    m_albumTitle->setPalette(captionPalette);
    captionLayout->addWidget(m_albumTitle);

    //Initial the details label.
    m_albumDetails=new QLabel(this);
    m_albumDetails->setPalette(captionPalette);
    captionLayout->addWidget(m_albumDetails);

    //Initial album treeview.
    m_albumTreeView=new KNMusicAlbumTreeView(this);
    contentLayout->addWidget(m_albumTreeView, 1);
}

void KNMusicAlbumDetail::initialAnimations()
{
    //Initial expand animation.
    m_expandAnime=new QSequentialAnimationGroup(this);
    connect(m_expandAnime, &QSequentialAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionExpandFinished);
    //Initial expand step 1.
    m_expandStep1=new QParallelAnimationGroup(this);
    //Initial artwork-in step 1.
    m_inCurve=QEasingCurve(QEasingCurve::OutCubic);
    m_albumArtIn1=new QPropertyAnimation(this);
    generateAlbumArtAnimation(m_albumArtIn1);
    m_expandStep1->addAnimation(m_albumArtIn1);
    //Initial content-in step 1.
    m_albumContentIn1=new QPropertyAnimation(this);
    generateContentAnimation(m_albumContentIn1);
    connect(m_albumContentIn1, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionExpandStep1);
    m_expandStep1->addAnimation(m_albumContentIn1);
    connect(m_expandStep1, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionExpandStep1InFinished);
    m_expandAnime->addAnimation(m_expandStep1);

    //Initial expand step 2.
    m_expandStep2=new QParallelAnimationGroup(this);
    //Initial artwork-in step 1.
    m_albumArtIn2=new QPropertyAnimation(this);
    generateAlbumArtAnimation(m_albumArtIn2);
    m_expandStep2->addAnimation(m_albumArtIn2);
    //Initial content-in step 1.
    m_albumContentIn2=new QPropertyAnimation(this);
    generateContentAnimation(m_albumContentIn2);
    connect(m_albumContentIn2, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    m_expandStep2->addAnimation(m_albumContentIn2);
    m_expandAnime->addAnimation(m_expandStep2);

    //Initial fold animation.
    m_foldAnime=new QParallelAnimationGroup(this);
    connect(m_foldAnime, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionFoldFinished);
    //Initial artwork fold animation.
    m_albumArtOut=new QPropertyAnimation(this);
    generateAlbumArtAnimation(m_albumArtOut);
    connect(m_albumArtOut, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionFold);
    m_foldAnime->addAnimation(m_albumArtOut);
    m_albumContentOut=new QPropertyAnimation(this);
    generateContentAnimation(m_albumContentOut);
    m_foldAnime->addAnimation(m_albumContentOut);

    //Initial show album animation.
    m_showAlbumArt=new QParallelAnimationGroup(this);
    m_showAlbumArtLabel=new QPropertyAnimation(this);
    generateAlbumArtAnimation(m_showAlbumArtLabel);
    m_showAlbumArt->addAnimation(m_showAlbumArtLabel);
    m_showAlbumContent=new QPropertyAnimation(this);
    generateContentAnimation(m_showAlbumContent);
    connect(m_showAlbumContent, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    m_showAlbumArt->addAnimation(m_showAlbumContent);
    //Initial hide album animation.
    m_hideAlbumArt=new QParallelAnimationGroup(this);
    m_hideAlbumArtLabel=new QPropertyAnimation(this);
    generateAlbumArtAnimation(m_hideAlbumArtLabel);
    m_hideAlbumArt->addAnimation(m_hideAlbumArtLabel);
    m_hideAlbumContent=new QPropertyAnimation(this);
    connect(m_hideAlbumContent, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    generateContentAnimation(m_hideAlbumContent);
    m_hideAlbumArt->addAnimation(m_hideAlbumContent);
}

void KNMusicAlbumDetail::initialShortCuts()
{
    QAction *foldShortCut=new QAction(this);
    foldShortCut->setShortcut(QKeySequence(Qt::Key_Escape));
    foldShortCut->setShortcutContext(Qt::WidgetShortcut);
    connect(foldShortCut, SIGNAL(triggered()), this, SLOT(onActionAskToFold()));
    addAction(foldShortCut);
}

void KNMusicAlbumDetail::generateAlbumArtAnimation(QPropertyAnimation *anime)
{
    anime->setTargetObject(m_albumArt);
    anime->setPropertyName("geometry");
    anime->setEasingCurve(m_inCurve);
}

void KNMusicAlbumDetail::generateContentAnimation(QPropertyAnimation *anime)
{
    anime->setTargetObject(m_albumContent);
    anime->setPropertyName("geometry");
    anime->setEasingCurve(QEasingCurve::OutCubic);
}

void KNMusicAlbumDetail::generateStep1FinalPosition(QRect &albumArtGeometry,
                                                    QRect &contentGeometry)
{
    int finalSizeParameter=(m_sizeParameter>>2)*3;
    albumArtGeometry=QRect((width()-(finalSizeParameter<<1))>>1,
                           (height()-finalSizeParameter)>>1,
                           finalSizeParameter,
                           finalSizeParameter);
    contentGeometry=QRect(width()>>1,
                          (height()-finalSizeParameter)>>1,
                          finalSizeParameter,
                          finalSizeParameter);
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
    //Update the shadow.
    updateShadowGeometries(contentGeometry);
    //Update the data.
    if(m_currentIndex.isValid())
    {
        updateAlbumCaptions();
        m_albumTreeView->resizeHeader();
    }
}

void KNMusicAlbumDetail::updateShadowGeometries(const QRect &contentPosition)
{
    m_rightShadow->setGeometry(contentPosition.left()-m_shadowWidth,
                               contentPosition.y(),
                               m_shadowWidth,
                               contentPosition.height());
    m_leftShadow->setGeometry(m_albumArt->geometry().right(),
                              contentPosition.y(),
                              m_shadowWidth,
                              contentPosition.height());
}

void KNMusicAlbumDetail::updateAlbumCaptions()
{
    //Set the title.
    m_albumTitle->setText(m_albumTitle->fontMetrics().elidedText(
                              m_albumModel->data(m_currentIndex, Qt::DisplayRole).toString(),
                              Qt::ElideRight,
                              m_albumTitle->width()-16));
    //Set the artist label.
    //Get the album list.
    QHash<QString, QVariant> artistList=m_albumModel->data(m_currentIndex, CategoryArtistList).toHash();
    //Judge the artist by list.
    QString albumArtist;
    if(artistList.isEmpty())
    {
        albumArtist=tr("Unknown Artist");
    }
    else
    {
        albumArtist=artistList.size()==1?
                        artistList.keys().first():
                        tr("Various Artists");
    }
    m_albumDetails->setText(m_albumDetails->fontMetrics().elidedText(
                                albumArtist,
                                Qt::ElideMiddle,
                                m_albumDetails->width()-16));
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

