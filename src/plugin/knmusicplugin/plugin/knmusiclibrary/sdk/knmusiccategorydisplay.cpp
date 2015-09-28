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
    m_largeIcon(new QLabel(this)),
    m_iconEffect(new QGraphicsOpacityEffect(m_largeIcon)),
    m_categoryTitle(new KNScrollLabel(this)),
    m_categoryInfo(new QLabel(this)),
    m_categoryTreeView(new KNMusicLibraryTreeView(this, tab)),
    m_leftShadow(new KNSideShadowWidget(KNSideShadowWidget::LeftShadow,
                                        this))
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);

    //Configure the large icon.
    m_largeIcon->installEventFilter(this);
    m_largeIcon->setScaledContents(true);
    //Configure the icon effect.
    m_iconEffect->setOpacity(0.9);
    m_largeIcon->setGraphicsEffect(m_iconEffect);
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
    m_categoryTreeView->setObjectName("CategoryDisplayTreeView");
    knTheme->registerWidget(m_categoryTreeView);

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

void KNMusicCategoryDisplay::scrollToSourceRow(const int &row)
{
    //Scroll to the source row.
    m_categoryTreeView->scrollToSourceRow(row);
}

void KNMusicCategoryDisplay::showNoCategoryItem(const QString &title)
{
    //Update the title.
    m_categoryTitle->setText(title);
    //Set the category text to empty
    m_categoryTreeView->setCategoryText(QVariant(QString()));
    //Update the background to no album art.
    setCategoryIcon(QPixmap());
    //Update the detail info.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::setCategoryColumn(const int &column)
{
    //Set the category column to the tree view.
    m_categoryTreeView->setCategoryColumn(column);
}

void KNMusicCategoryDisplay::setCategoryText(const QString &text)
{
    //Update the title.
    m_categoryTitle->setText(text);
    //Set the category to treeview.
    m_categoryTreeView->setCategoryText(QVariant(text));
    //Update the detail info.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::setCategoryIcon(const QPixmap &pixmap)
{
    //Save the pixmap.
    m_categoryIcon=pixmap;
    //Update the icon content.
    updateBackgroundIconContent();
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

void KNMusicCategoryDisplay::retranslate()
{
    //Update the text resourse.
    m_songCount[0]=tr("No song.");
    m_songCount[1]=tr("1 song.");
    m_songCount[2]=tr("%1 songs.");
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
}

inline void KNMusicCategoryDisplay::updateDetailInfo()
{
    //Get the proxy model.
    KNMusicProxyModel *model=m_categoryTreeView->proxyModel();
    //The model cannot be null, we don't need to check it.
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
    int viewHeight=qMin(m_categoryTreeView->width(),
                        m_categoryTreeView->height());
    //Update the geometry.
    m_largeIcon->setGeometry(width()-viewHeight,
                             height()-viewHeight,
                             viewHeight,
                             viewHeight);
    //Update the gradient.
    m_iconGradient.setCenter(QPointF(viewHeight, viewHeight));
    m_iconGradient.setFocalPoint(QPointF(viewHeight, viewHeight));
    m_iconGradient.setRadius(viewHeight);
    //Set the gradient as the mask of the opacity effect.
    m_iconEffect->setOpacityMask(m_iconGradient);
    //Update the icon content.
    updateBackgroundIconContent();
}

inline void KNMusicCategoryDisplay::updateBackgroundIconContent()
{
    //Set the pixmap to the label.
    m_largeIcon->setPixmap(m_categoryIcon.isNull()?
                               knMusicGlobal->noAlbumArt():
                               m_categoryIcon.scaled(m_largeIcon->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}

