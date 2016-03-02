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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPainter>

#include "knmousesensewidget.h"
#include "knthememanager.h"
#include "knsideshadowwidget.h"
#include "knlocalemanager.h"
#include "knscrolllabel.h"

#include "knmusicproxymodel.h"
#include "knmusicsearchbase.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibrarytreeview.h"

#include "knmusiccategorydisplay.h"

KNMusicCategoryDisplay::KNMusicCategoryDisplay(QWidget *parent,
                                               KNMusicTab *tab) :
    QWidget(parent),
    m_iconGradient(QRadialGradient()),
    m_categoryIcon(QPixmap()),
    m_searchResultIn(QString()),
    m_titleText(QString()),
    m_categoryTitle(new KNScrollLabel(this)),
    m_categoryInfo(new QLabel(this)),
    m_categoryTreeView(new KNMusicLibraryTreeView(this, tab)),
    m_leftShadow(new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                        this)),
    m_backgroundSize(0)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);

    //Configure the icon gradient.
    m_iconGradient.setColorAt(0.0, Qt::black);
    m_iconGradient.setColorAt(1.0, Qt::transparent);
    //Configure the title label.
    m_categoryTitle->setObjectName("CategoryViewerLabel");
    knTheme->registerWidget(m_categoryTitle);
    QFont titleFont=m_categoryTitle->font();
    titleFont.setPixelSize(21);
    titleFont.setBold(true);
    m_categoryTitle->setFont(titleFont);
    //Configure the information label.
    m_categoryInfo->setObjectName("CategoryDetailLabel");
    knTheme->registerWidget(m_categoryInfo);
    QFont infoFont=m_categoryInfo->font();
    infoFont.setPixelSize(14);
    m_categoryInfo->setFont(infoFont);
    m_categoryInfo->setContentsMargins(4,0,0,0);
    //Configure the tree view.
    setFocusProxy(m_categoryTreeView);
    m_categoryTreeView->updateObjectName("CategoryDisplayTreeView");
    connect(m_categoryTreeView, &KNMusicLibraryTreeView::startDraggingSong,
            this, &KNMusicCategoryDisplay::requireShowPlaylistList);
    connect(m_categoryTreeView, &KNMusicLibraryTreeView::endDraggingSong,
            this, &KNMusicCategoryDisplay::requireHidePlaylistList);

    //Add main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Initial information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->updateObjectName("CategoryInformationContainer");
    infoContainer->setContentsMargins(15,12,15,8);
    //Add container to display layout.
    mainLayout->addWidget(infoContainer);
    //Initial the information container layout.
    QBoxLayout *infoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          infoContainer);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->setSpacing(0);
    //Set information container.
    infoContainer->setLayout(infoLayout);
    //Add widgets to information layout.
    infoLayout->addWidget(m_categoryTitle);
    infoLayout->addWidget(m_categoryInfo);
    //Add tree view to main layout.
    mainLayout->addWidget(m_categoryTreeView, 1);

    //Update the background icon.
    updateBackgroundIcon();

    //Link the search.
    connect(knMusicGlobal->search(), &KNMusicSearchBase::requireSearch,
            this, &KNMusicCategoryDisplay::onActionSearch);

    //Connect retranslate signal.
    knI18n->link(this, &KNMusicCategoryDisplay::retranslate);
    retranslate();
}

void KNMusicCategoryDisplay::setLibraryModel(KNMusicLibraryModel *model)
{
    //Set the library model to the tree view.
    m_categoryTreeView->setMusicModel(model);
    //Set the default sort state.
    m_categoryTreeView->sortByColumn(Name, Qt::AscendingOrder);
}

void KNMusicCategoryDisplay::scrollToSourceRow(int row)
{
    //Scroll to the source row.
    m_categoryTreeView->scrollToSourceRow(row);
}

void KNMusicCategoryDisplay::showNoCategoryItem(const QString &title)
{
    //Check the visible.
    if(!m_categoryTreeView->isVisible())
    {
        //Show up all the staffs.
        showAllStaffs();
    }
    //Save the title text.
    m_titleText=title;
    //Update the title.
    updateTitle();
    //Set the category text to empty
    m_categoryTreeView->setCategoryText(QVariant(QString("")));
    //Update the background to no album art.
    setCategoryIcon(QPixmap());
    //Update the detail info.
    updateDetailInfo();
    //Move the tree view to the top.
    m_categoryTreeView->scrollToTop();
}

void KNMusicCategoryDisplay::setCategoryColumn(const int &column)
{
    //Set the category column to the tree view.
    m_categoryTreeView->setCategoryColumn(column);
}

void KNMusicCategoryDisplay::setCategoryText(const QString &text)
{
    //Check the visible of widgets.
    if(!m_categoryTreeView->isVisible())
    {
        //Show up all the staffs.
        showAllStaffs();
    }
    //Save the text.
    m_titleText=text;
    //Update the title.
    updateTitle();
    //Set the category to treeview.
    m_categoryTreeView->setCategoryText(QVariant(text));
    //Update the detail info.
    updateDetailInfo();
    //Move the tree view to the top.
    m_categoryTreeView->scrollToTop();
}

void KNMusicCategoryDisplay::setCategoryIcon(const QPixmap &pixmap)
{
    //Save the pixmap.
    m_categoryIcon=pixmap;
    //Update the icon content.
    updateBackgroundIconContent();
}

void KNMusicCategoryDisplay::hideAllStaffs()
{
    //Hide all the data display widgets.
    //Update the background image to a null one.
    m_backgroundImage=QPixmap();
    //Hide labels.
    m_categoryTitle->hide();
    m_categoryInfo->hide();
    //Hide treeview.
    m_categoryTreeView->hide();
}

void KNMusicCategoryDisplay::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Resize the shadow.
    m_leftShadow->resize(15, height());
    //Update the background icon.
    updateBackgroundIcon();
}

void KNMusicCategoryDisplay::paintEvent(QPaintEvent *event)
{
    //Do the original paint event.
    QWidget::paintEvent(event);
    //Check background image is null or not.
    if(!m_backgroundImage.isNull())
    {
        //Initial a antialiasing painter.
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing |
                               QPainter::SmoothPixmapTransform, true);
        //Initial the opacity of the painter.
        painter.setOpacity(0.9);
        //Draw the scaled background image.
        painter.drawPixmap(width()-m_backgroundSize,
                           height()-m_backgroundSize,
                           m_backgroundImage);
    }
}

void KNMusicCategoryDisplay::retranslate()
{
    //Update the text resourse.
    m_songCount[0]=tr("No song.");
    m_songCount[1]=tr("1 song.");
    m_songCount[2]=tr("%1 songs.");
    //Update the search result.
    m_searchCount[0]=tr("No result.");
    m_searchCount[1]=tr("1 result.");
    m_searchCount[2]=tr("%1 results.");
    //Update the search result title.
    m_searchResultIn=tr("Search result in %1");
    //Update the detail info.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::onActionSearch()
{
    //Check whether the proxy model is nullptr.
    if(m_categoryTreeView->proxyModel()==nullptr)
    {
        //Ignore the search request.
        return;
    }
    //Set the search rules to the proxy model.
    m_categoryTreeView->proxyModel()->setSearchBlocks(
                knMusicGlobal->search()->rules());
    //Update the title and detail info.
    updateTitle();
    updateDetailInfo();
}

inline void KNMusicCategoryDisplay::showAllStaffs()
{
    //Show labels.
    m_categoryTitle->show();
    m_categoryInfo->show();
    //Show treeview.
    m_categoryTreeView->show();
}

inline void KNMusicCategoryDisplay::updateTitle()
{
    //Check whether the proxy model is in search mode.
    if(m_categoryTreeView->proxyModel()->isSearchMode())
    {
        //Update the title caption with result text.
        m_categoryTitle->setText(m_searchResultIn.arg(m_titleText));
        //Mission complete.
        return;
    }
    //Or else we have to simply set the title text.
    m_categoryTitle->setText(m_titleText);
}

inline void KNMusicCategoryDisplay::updateDetailInfo()
{
    //Get the proxy model.
    KNMusicProxyModel *model=m_categoryTreeView->proxyModel();
    //The model cannot be null, we don't need to check it.
    //Check whether the model is in search mode.
    if(model->isSearchMode())
    {
        //Set the category info data to search result.
        int searchResult=model->rowCount();
        //Set the detail text.
        m_categoryInfo->setText(searchResult<2?
                                    m_searchCount[searchResult]:
                                    m_searchCount[2].arg(
                                        QString::number(searchResult)));
        return;
    }
    //Get the row count.
    int songCount=model->rowCount();
    //Check out the song count.
    m_categoryInfo->setText(songCount<2?
                                //For 0 and 1, we will simply use the original
                                //data.
                                m_songCount[songCount]:
                                //For 2 and above, use arg().
                                m_songCount[2].arg(
                                    QString::number(model->rowCount())));
}

inline void KNMusicCategoryDisplay::updateBackgroundIcon()
{
    //Get the tree view height.
    m_backgroundSize=qMin(m_categoryTreeView->width(),
                          m_categoryTreeView->height());
    //Update the gradient.
    m_iconGradient.setCenter(QPointF(m_backgroundSize, m_backgroundSize));
    m_iconGradient.setFocalPoint(QPointF(m_backgroundSize, m_backgroundSize));
    m_iconGradient.setRadius(m_backgroundSize);
    //Update the icon content.
    updateBackgroundIconContent();
}

inline void KNMusicCategoryDisplay::updateBackgroundIconContent()
{
    //Set the pixmap to the label.
    m_backgroundImage=m_categoryIcon.isNull()?
                knMusicGlobal->noAlbumArt().scaled(
                    QSize(m_backgroundSize, m_backgroundSize),
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation):
                m_categoryIcon.scaled(
                    QSize(m_backgroundSize, m_backgroundSize),
                    Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation);
    //Start the draw the mask and blur on the background image.
    QPainter contentPainter(&m_backgroundImage);
    //Configure up the painter.
    contentPainter.setRenderHints(QPainter::Antialiasing |
                                  QPainter::SmoothPixmapTransform);
    //Draw the mask on the background image.
    contentPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //Draw the mask.
    contentPainter.fillRect(QRect(0, 0, m_backgroundSize, m_backgroundSize),
                            m_iconGradient);
}

