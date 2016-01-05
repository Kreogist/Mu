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
#include <QBoxLayout>
#include <QMouseEvent>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "knmouseunclickablewidget.h"
#include "knsideshadowwidget.h"
#include "knscrolllabel.h"
#include "knthememanager.h"

#include "knmusiclibrarymodel.h"
#include "knmusicalbumlistview.h"
#include "knmusicalbumtitle.h"
#include "knmusicalbummodel.h"
#include "knmusiclibraryimagemanager.h"

#include "knmusicalbumdetail.h"

#define ShadowWidth 15

#include <QDebug>

KNMusicAlbumDetail::KNMusicAlbumDetail(QWidget *parent, KNMusicTab *tab) :
    QWidget(parent),
    m_currentIndex(QModelIndex()),
    m_inCurve(QEasingCurve(QEasingCurve::OutCubic)),
    m_animeStartRect(QRect()),
    m_albumContent(new KNMouseUnclickableWidget(this)),
    m_albumArt(new KNMusicAlbumTitle(this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_leftShadow(new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                        this)),
    m_albumTitle(new KNScrollLabel(this)),
    m_albumDetails(new KNScrollLabel(this)),
    m_albumListView(new KNMusicAlbumListView(this, tab)),
    m_libraryModel(nullptr),
    m_expandAnime(new QSequentialAnimationGroup(this)),
    m_expandStep1(new QParallelAnimationGroup(this)),
    m_expandStep2(new QParallelAnimationGroup(this)),
    m_foldAnime(new QParallelAnimationGroup(this)),
    m_showAlbumArt(new QParallelAnimationGroup(this)),
    m_hideAlbumArt(new QParallelAnimationGroup(this)),
    m_flyAwayAnime(new QParallelAnimationGroup(this)),
    m_albumArtIn1(generateAnime(m_albumArt)),
    m_albumContentIn1(generateAnime(m_albumContent)),
    m_albumArtIn2(generateAnime(m_albumArt)),
    m_albumContentIn2(generateAnime(m_albumContent)),
    m_albumArtOut(generateAnime(m_albumArt)),
    m_albumContentOut(generateAnime(m_albumContent)),
    m_albumArtFlyAway(generateAnime(m_albumArt)),
    m_albumContentFlyAway(generateAnime(m_albumContent)),
    m_showAlbumArtLabel(generateAnime(m_albumArt)),
    m_showAlbumContent(generateAnime(m_albumContent)),
    m_hideAlbumArtLabel(generateAnime(m_albumArt)),
    m_hideAlbumContent(generateAnime(m_albumContent)),
    m_imageManager(nullptr),
    m_iconSize(0),
    m_panelSize(0),
    m_backgroundAnime(true),
    m_pressed(false)
{
    //Set properties.
    setAutoFillBackground(true);
    setFocusPolicy(Qt::WheelFocus);
    //Set the background color.
    QPalette pal=palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    setPalette(pal);

    //Configure the album content.
    m_albumContent->setAutoFillBackground(true);
    m_albumContent->setFocusPolicy(Qt::StrongFocus);
    QPalette contentPalette=m_albumContent->palette();
    contentPalette.setColor(QPalette::Window, QColor(255,255,255,240));
    m_albumContent->setPalette(contentPalette);
    //Configure the album title label.
    m_albumTitle->setObjectName("MusicAlbumTitleLabel");
    m_albumTitle->setGlowRadius(4.0);
    knTheme->registerWidget(m_albumTitle);
    // Set the font.
    QFont captionFont=m_albumTitle->font();
    captionFont.setBold(true);
    captionFont.setPixelSize(21);
    m_albumTitle->setFont(captionFont);
    //Configure the album artist label.
    m_albumDetails->setObjectName("MusicAlbumDetailLabel");
    m_albumDetails->setGlowRadius(4.0);
    knTheme->registerWidget(m_albumDetails);
    //Link signals.
    connect(m_expandAnime, &QSequentialAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionExpandFinished);
    connect(m_albumContentIn1, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionExpandStep1);
    connect(m_expandStep1, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionExpandStep1InFinished);
    connect(m_albumContentIn2, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    connect(m_foldAnime, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionFoldFinished);
    connect(m_albumArtOut, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionFold);
    connect(m_flyAwayAnime, &QParallelAnimationGroup::finished,
            this, &KNMusicAlbumDetail::onActionFlyAwayFinished);
    connect(m_albumArtFlyAway, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionFlyAway);
    connect(m_showAlbumContent, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    connect(m_hideAlbumContent, &QPropertyAnimation::valueChanged,
            this, &KNMusicAlbumDetail::onActionContentMove);
    //Configure the animation groups.
    m_expandStep1->addAnimation(m_albumArtIn1);
    m_expandStep1->addAnimation(m_albumContentIn1);
    m_expandAnime->addAnimation(m_expandStep1);
    m_expandStep2->addAnimation(m_albumArtIn2);
    m_expandStep2->addAnimation(m_albumContentIn2);
    m_expandAnime->addAnimation(m_expandStep2);
    m_foldAnime->addAnimation(m_albumArtOut);
    m_foldAnime->addAnimation(m_albumContentOut);
    m_flyAwayAnime->addAnimation(m_albumArtFlyAway);
    m_flyAwayAnime->addAnimation(m_albumContentFlyAway);
    m_showAlbumArt->addAnimation(m_showAlbumArtLabel);
    m_showAlbumArt->addAnimation(m_showAlbumContent);
    m_hideAlbumArt->addAnimation(m_hideAlbumArtLabel);
    m_hideAlbumArt->addAnimation(m_hideAlbumContent);

    //Initial the content layout for album content.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             m_albumContent);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);
    m_albumContent->setLayout(contentLayout);
    //Initial the caption layout for the title and detail label.
    QBoxLayout *captionLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             contentLayout->widget());
    captionLayout->setContentsMargins(21,21,21,14);
    captionLayout->setSpacing(0);
    contentLayout->addLayout(captionLayout);
    //Add widget to caption layout.
    captionLayout->addWidget(m_albumTitle);
    captionLayout->addWidget(m_albumDetails);
    //Add the tree view to content layout.
    contentLayout->addWidget(m_albumListView, 1);
}

void KNMusicAlbumDetail::setLibraryModel(KNMusicLibraryModel *model)
{
    //Check the library model has been set before.
    if(m_libraryModel!=nullptr)
    {
        //We have set a library model before.
        return;
    }
    //Save the library model.
    m_libraryModel=model;
    //Check the library model after set.
    if(m_libraryModel==nullptr)
    {
        //The new model is nullptr, ignore it.
        return;
    }
    //Set the library model to treeview.
    m_albumListView->setMusicModel(m_libraryModel);
    //Set default sort state.
    m_albumListView->sortByColumn(TrackNumber, Qt::AscendingOrder);
}

void KNMusicAlbumDetail::setAnimeParameter(const QRect &albumRect,
                                           int iconSize)
{
    //Save the icon size and the album position.
    m_iconSize=iconSize;
    m_animeStartRect=albumRect;
}

void KNMusicAlbumDetail::updateFoldEndValue(const QRect &position,
                                            int iconSize)
{
    //Check the running state of the fold anime.
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

void KNMusicAlbumDetail::displayAlbumDetail(const QModelIndex &index)
{
    //Cut all the connections.
    m_detailHandler.disconnectAll();
    //Stop all the animations.
    stopAllAnimations();
    //Hide the contents.
    hideContentWidgets();
    //Update the panel size.
    updatePanelSize();
    //Save the index.
    m_currentIndex=index;
    //Update the artwork.
    updateAlbumArtwork();
    //Set category content.
    m_albumListView->setCategoryText(m_currentIndex.row()==0?
                                         QVariant(QString("")):
                                         m_currentIndex.data(Qt::DisplayRole));
    m_albumListView->scrollToTop();
    //Initial the opacity effect.
    QPalette contentPalette=m_albumContent->palette();
    contentPalette.setColor(QPalette::Window, QColor(255,255,255,240));
    m_albumContent->setPalette(contentPalette);
    //Set the position.
    QRect albumArtStartRect(m_animeStartRect.x(),
                            m_animeStartRect.y(),
                            m_iconSize,
                            m_iconSize);
    //Set the album art and album content to the initial geometry.
    m_albumArt->setGeometry(albumArtStartRect);
    m_albumContent->setGeometry(albumArtStartRect);
    //Update the parameter.
    updateExpandAlbumParameter();
    //Make album art up.
    m_albumArt->raise();
    //Tweak the shadows.
    m_rightShadow->hide();
    m_leftShadow->raise();
    m_leftShadow->show();
    //Initial the shadow position.
    updateShadowGeometries(albumArtStartRect);
    //Show the widget.
    show();
    //Set the focus.
    setFocus();
    //Start animation.
    m_expandAnime->start();
}

void KNMusicAlbumDetail::foldAlbumDetail()
{
    //Cut all the connections.
    m_detailHandler.disconnectAll();
    //If m_animeStartRect is empty, or the state is running means now is running
    //fold animation, we don't need to fold it more.
    if(!m_animeStartRect.isNull()
                && m_foldAnime->state()==QAbstractAnimation::Stopped)
    {
        //Stop all the animations.
        stopAllAnimations();
        //Update the fold animation.
        updateFoldAlbumParameter();
        //Hide all the shadows.
        m_rightShadow->hide();
        m_leftShadow->hide();
        //Enable background change.
        m_backgroundAnime=true;
        //Hide contents.
        hideContentWidgets();
        //Start animation.
        m_foldAnime->start();
    }
}

void KNMusicAlbumDetail::flyAwayAlbumDetail()
{
    //Cut all the connections.
    m_detailHandler.disconnectAll();
    //Check is fly out animation is running.
    if(m_flyAwayAnime->state()==QAbstractAnimation::Stopped)
    {
        //Stop all animations.
        stopAllAnimations();
        //Emit clear selection signal.
        emit requireClearSelection();
        //Generate the position.
        QRect albumArtEndRect(-m_iconSize,
                              (height()-m_iconSize)>>1,
                              m_iconSize,
                              m_iconSize);
        //Set the animation start and end value.
        m_albumArtFlyAway->setStartValue(m_albumArt->geometry());
        m_albumArtFlyAway->setEndValue(albumArtEndRect);
        m_albumContentFlyAway->setStartValue(m_albumContent->geometry());
        m_albumContentFlyAway->setEndValue(albumArtEndRect);
        //Hide all the shadows.
        m_rightShadow->hide();
        m_leftShadow->hide();
        //Enable background change.
        m_backgroundAnime=true;
        //Hide contents.
        hideContentWidgets();
        //Start animation.
        m_flyAwayAnime->start();
    }
}

void KNMusicAlbumDetail::scrollToSourceRow(int row)
{
    m_albumListView->scrollToSourceRow(row);
}

void KNMusicAlbumDetail::setImageManager(
        KNMusicLibraryImageManager *imageManager)
{
    //Save the pointer.
    m_imageManager=imageManager;
    //Link the image manager with the album detail.
    connect(m_imageManager, &KNMusicLibraryImageManager::imageInserted,
            this, &KNMusicAlbumDetail::onActionImageInserted,
            Qt::QueuedConnection);
}

void KNMusicAlbumDetail::onActionAlbumArtUpdate(const QModelIndex &updatedIndex)
{
    //Check out the updated index is current index.
    if(updatedIndex==m_currentIndex)
    {
        //Update the artwork.
        updateAlbumArtwork();
    }
}

void KNMusicAlbumDetail::onActionAlbumRemoved(const QModelIndex &removedIndex)
{
    //Check the index, if the delete one is current, fly me away.
    if(removedIndex==m_currentIndex)
    {
        //Fly the current album detail away.
        flyAwayAlbumDetail();
    }
}

void KNMusicAlbumDetail::resizeEvent(QResizeEvent *event)
{
    //Do original resize.
    QWidget::resizeEvent(event);
    //Update the panel size.
    updatePanelSize();
    //Check out the animation running state.
    //Check expand animation state.
    if(m_expandAnime->state()==QAbstractAnimation::Running)
    {
        //Update the parameter.
        updateExpandAlbumParameter();
        return;
    }
    //Check fly away animation state.
    //Update the child widget geometries.
    updateWidgetGeometries();
}

void KNMusicAlbumDetail::showEvent(QShowEvent *event)
{
    //Show the widget.
    QWidget::showEvent(event);
    //Update the panel size.
    updatePanelSize();
    //Check out the animation running state.
    //Check expand animation state.
    if(m_expandAnime->state()==QAbstractAnimation::Running)
    {
        //Update the parameter.
        updateExpandAlbumParameter();
    }
}

void KNMusicAlbumDetail::hideEvent(QHideEvent *event)
{
    //Hide the widget first.
    QWidget::hideEvent(event);
    //Clear the album art image, set the null pixmap to album art.
    m_albumArt->clearAlbumArt();
}

void KNMusicAlbumDetail::mousePressEvent(QMouseEvent *event)
{
    //Do original pressed event.
    QWidget::mousePressEvent(event);
    //Set the preesed flag to true.
    m_pressed=true;
}

void KNMusicAlbumDetail::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original release event.
    QWidget::mouseReleaseEvent(event);
    //Check the flag.
    if(m_pressed)
    {
        //Release the pressed flag to false.
        m_pressed=false;
        //Check the position of the mouse.
        if(rect().contains(event->pos()))
        {
            //Ask to show the other album.
            emit requireShowAlbum(event->pos());
        }
    }
}

void KNMusicAlbumDetail::onActionExpandFinished()
{
    //Add show and hide album art signal to detail handler.
    m_detailHandler.append(
                connect(m_albumArt, &KNMusicAlbumTitle::requireShowAlbumArt,
                        this, &KNMusicAlbumDetail::onActionShowAlbumArt));
    m_detailHandler.append(
                connect(m_albumArt, &KNMusicAlbumTitle::requireHideAlbumArt,
                        this, &KNMusicAlbumDetail::onActionHideAlbumArt));
}

void KNMusicAlbumDetail::onActionShowAlbumArt()
{
    //Stop all the animations.
    stopAllAnimations();
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
    //Stop all the animations.
    stopAllAnimations();
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

void KNMusicAlbumDetail::onActionExpandStep1(const QVariant &position)
{
    //Move the shadows.
    updateShadowGeometries(position.toRect());
    //Calculate the progress.
    qreal progress=m_inCurve.valueForProgress(
                (qreal)m_albumArtIn1->currentTime()/m_albumArtIn1->duration());
    if(m_backgroundAnime)
    {
        QPalette pal=palette();
        pal.setColor(QPalette::Window, QColor(0,0,0,progress*200));
        setPalette(pal);
    }
    //Set the opacity effect.
    QPalette contentPalette=m_albumContent->palette();
    contentPalette.setColor(QPalette::Window, QColor(255,
                                                     255,
                                                     255,
                                                     progress*240.0));
    m_albumContent->setPalette(contentPalette);
}

void KNMusicAlbumDetail::onActionExpandStep1InFinished()
{
    //Raise up the content.
    m_albumContent->raise();
    //Change the shadow of the content widget.
    m_rightShadow->raise();
    m_rightShadow->show();
    m_leftShadow->hide();
    //Disable the background anime.
    m_backgroundAnime=false;
    //Show the contents.
    showContentWidgets();
}

void KNMusicAlbumDetail::onActionContentMove(const QVariant &position)
{
    //Move the shadows.
    updateShadowGeometries(position.toRect());
}

void KNMusicAlbumDetail::onActionFoldFinished()
{
    //Emit clear selection signal.
    emit requireClearSelection();
    //Hide the detail widget.
    hide();
}

void KNMusicAlbumDetail::onActionFold(const QVariant &position)
{
    Q_UNUSED(position)
    //Clear the current index.
    m_currentIndex=QModelIndex();
    //Calculate the progress.
    qreal progress=1.0-(qreal)m_albumArtOut->currentTime()/
                                               (qreal)m_albumArtOut->duration();
    //Set the palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,progress*200));
    setPalette(pal);
    //Set the opacity effect.
    pal=m_albumContent->palette();
    pal.setColor(QPalette::Window, QColor(255,255,255,progress*240.0));
    m_albumContent->setPalette(pal);
}

void KNMusicAlbumDetail::onActionFlyAwayFinished()
{
    //Hide the detail widget.
    hide();
}

void KNMusicAlbumDetail::onActionFlyAway(const QVariant &position)
{
    Q_UNUSED(position)
    //Clear the current index.
    m_currentIndex=QModelIndex();
    //Calculate the progress.
    qreal progress=1.0-(qreal)m_albumArtFlyAway->currentTime()/
            (qreal)m_albumArtFlyAway->duration();
    //Set the palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,progress*200));
    setPalette(pal);
    //Set the opacity effect.
    pal=m_albumContent->palette();
    pal.setColor(QPalette::Window, QColor(255,255,255,progress*240.0));
    m_albumContent->setPalette(pal);
}

void KNMusicAlbumDetail::onActionImageInserted(const QString &hashKey)
{
    //When there's a new image insert, check out the hash key.
    //Check the index is valid.
    if(m_currentIndex.isValid() && m_imageManager)
    {
        //Get the artwork key.
        QString &&albumHashKey=
                m_currentIndex.data(
                    KNMusicAlbumModel::CategoryArtworkKeyRole).toString();
        //Check album hash key.
        if(albumHashKey==hashKey)
        {
            //Set the album art.
            m_albumArt->setAlbumArt(m_imageManager->artwork(albumHashKey));
        }
    }
}

inline void KNMusicAlbumDetail::showContentWidgets()
{
    //Set the data.
    updateAlbumCaptions();
    //Show the titles.
    m_albumTitle->show();
    m_albumDetails->show();
    //When show the treeview, resize the header first.
    m_albumListView->show();
}

inline void KNMusicAlbumDetail::hideContentWidgets()
{
    //Hide all the widgets in the content widget.
    m_albumTitle->hide();
    m_albumDetails->hide();
    m_albumListView->hide();
}

void KNMusicAlbumDetail::updateAlbumCaptions()
{
    //Check is current index vaild.
    if(!m_currentIndex.isValid())
    {
        return;
    }
    //Set the title.
    m_albumTitle->setText(m_currentIndex.data(Qt::DisplayRole).toString());
    //Set the artist and other data of the album.
    m_albumDetails->setText(
                m_currentIndex.data(
                    KNMusicAlbumModel::AlbumArtistRole).toString());
}

inline void KNMusicAlbumDetail::generateStep1FinalPosition(
        QRect &albumArtGeometry,
        QRect &contentGeometry)
{
    //Set the position of the
    albumArtGeometry=QRect((width()-(m_panelSize<<1))>>1,
                           (height()-m_panelSize)>>1,
                           m_panelSize,
                           m_panelSize);
    contentGeometry=QRect(width()>>1,
                          (height()-m_panelSize)>>1,
                          m_panelSize,
                          m_panelSize);
}

inline void KNMusicAlbumDetail::generateStep2FinalPosition(
        QRect &albumArtGeometry,
        QRect &contentGeometry)
{
    albumArtGeometry=QRect((width()-m_panelSize-
                            (m_panelSize>>1))>>1,
                           (height()-m_panelSize)>>1,
                           m_panelSize,
                           m_panelSize);
    contentGeometry=QRect(albumArtGeometry.x()+(m_panelSize>>1),
                          albumArtGeometry.y(),
                          m_panelSize,
                          m_panelSize);
}

inline void KNMusicAlbumDetail::stopAllAnimations()
{
    //Stop all the anime group.
    m_foldAnime->stop();
    m_expandAnime->stop();
    m_flyAwayAnime->stop();
    //Stop the hide artwork animations.
    m_showAlbumArt->stop();
    m_hideAlbumArt->stop();
}

void KNMusicAlbumDetail::updateAlbumArtwork()
{
    //Check the index is valid.
    if(m_currentIndex.isValid() && m_imageManager)
    {
        //Get the artwork key.
        QString &&albumHashKey=
                m_currentIndex.data(
                    KNMusicAlbumModel::CategoryArtworkKeyRole).toString();
        //Check album hash key.
        if(albumHashKey.isEmpty())
        {
            //Set the album art.
            m_albumArt->setAlbumArt(knMusicGlobal->noAlbumArt());
        }
        //Get the variant image.
        QPixmap &&albumArtImage=m_imageManager->artwork(albumHashKey);
        //Set the album art.
        m_albumArt->setAlbumArt(albumArtImage.isNull()?
                                    knMusicGlobal->noAlbumArt():
                                    albumArtImage);
    }
}

inline void KNMusicAlbumDetail::updatePanelSize()
{
    //Check the width and height of the widget.
    if(width()>height())
    {
        //Calculate the panel size.
        // First calculate the 3/4 of the height.
        int preferSize=(height()>>2)*3;
        //Check if the width can hold double of the prefer size.
        m_panelSize=((preferSize<<1)<=width())?
                    //Save it as the prefer size.
                    preferSize:
                    //Half of the height. It must can be hold.
                    (height()>>1);
        //Complete.
        return;
    }
    // 1/2 of the width.
    m_panelSize=(width()>>1);
}

inline void KNMusicAlbumDetail::updateWidgetGeometries()
{
    //Get the final position.
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
    }
}

inline void KNMusicAlbumDetail::updateShadowGeometries(
        const QRect &contentPosition)
{
    //Update the shadow widget.
    m_rightShadow->setGeometry(contentPosition.left()-ShadowWidth,
                               contentPosition.y(),
                               ShadowWidth,
                               contentPosition.height());
    m_leftShadow->setGeometry(m_albumArt->geometry().right(),
                              contentPosition.y(),
                              ShadowWidth,
                              contentPosition.height());
}

inline void KNMusicAlbumDetail::updateExpandAlbumParameter()
{
    //Set the position.
    QRect albumArtStartRect(m_animeStartRect.x(),
                            m_animeStartRect.y(),
                            m_iconSize,
                            m_iconSize);
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
}

inline void KNMusicAlbumDetail::updateFoldAlbumParameter()
{
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
}

inline QPropertyAnimation *KNMusicAlbumDetail::generateAnime(
        QObject *target,
        QEasingCurve::Type type)
{
    //Generate a property anime.
    QPropertyAnimation *anime=new QPropertyAnimation(target,
                                                     "geometry",
                                                     this);
    //Configure up the anime.
    anime->setEasingCurve(type);
    //Give back the anime.
    return anime;
}
