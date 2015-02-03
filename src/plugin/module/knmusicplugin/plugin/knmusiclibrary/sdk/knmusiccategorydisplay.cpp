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
#include <QLabel>
#include <QGraphicsOpacityEffect>

#include "knsideshadowwidget.h"
#include "knglobal.h"

#include "knmousesensewidget.h"
#include "knmusicproxymodel.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibrarytreeview.h"

#include "knmusiccategorydisplay.h"

#include <QDebug>

KNMusicCategoryDisplay::KNMusicCategoryDisplay(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);

    //Initial icon background.
    m_largeIcon=new QLabel(this);
    m_largeIcon->installEventFilter(this);
    m_largeIcon->setScaledContents(true);

    m_iconEffect=new QGraphicsOpacityEffect(m_largeIcon);
    m_iconEffect->setOpacity(0.9);
    m_iconGradient.setColorAt(0.0, Qt::black);
    m_iconGradient.setColorAt(1.0, Qt::transparent);
    m_largeIcon->setGraphicsEffect(m_iconEffect);

    //Add layout.
    QBoxLayout *displayLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             this);
    displayLayout->setContentsMargins(0,0,0,0);
    displayLayout->setSpacing(0);
    setLayout(displayLayout);

    //Initial information container.
    KNMouseSenseWidget *infoContainer=new KNMouseSenseWidget(this);
    infoContainer->setContentsMargins(20,12,0,8);

    QBoxLayout *infoLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          infoContainer);
    infoLayout->setContentsMargins(0,0,0,0);
    infoLayout->setSpacing(0);
    infoContainer->setLayout(infoLayout);

    displayLayout->addWidget(infoContainer);

    //Initial the information label.
    m_categoryTitle=new QLabel(this);
    QPalette captionPal=m_categoryTitle->palette();
    captionPal.setColor(QPalette::WindowText, QColor(255,255,255));
    m_categoryTitle->setPalette(captionPal);
    QFont titleFont=m_categoryTitle->font();
    titleFont.setPixelSize(21);
    titleFont.setBold(true);
    m_categoryTitle->setFont(titleFont);
    infoLayout->addWidget(m_categoryTitle);
    m_categoryInfo=new QLabel(this);
    m_categoryInfo->setPalette(captionPal);
    QFont infoFont=m_categoryInfo->font();
    infoFont.setPixelSize(14);
    m_categoryInfo->setFont(infoFont);
    infoLayout->addWidget(m_categoryInfo);

    //Initial the tree view.
    m_categoryTreeView=new KNMusicLibraryTreeView(this);
    //Set the focus proxy to the treeview.
    setFocusProxy(m_categoryTreeView);
    //Disable the search shortcut, using the category search shortcut.
    m_categoryTreeView->disableSearchShortcut();
    QPalette pal=m_categoryTreeView->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    m_categoryTreeView->setPalette(pal);
    displayLayout->addWidget(m_categoryTreeView, 1);

    //Initial the shadow.
    m_leftShadow=new KNSideShadowWidget(LeftShadow, this);

    //Update the background icon.
    updateBackgroundIcon();

    //Connect retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicCategoryDisplay::retranslate);
    //Do retranslate.
    retranslate();
}

void KNMusicCategoryDisplay::setMusicTab(KNMusicTab *tab)
{
    m_categoryTreeView->setMusicTab(tab);
}

void KNMusicCategoryDisplay::retranslate()
{
    //Update the text resourse.
    m_songCount[0]=tr("No song.");
    m_songCount[1]=tr("1 song.");
    m_songCount[2]=tr("%1 songs.");
    //Update the information.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::updateDetailInfo()
{
    KNMusicProxyModel *model=m_categoryTreeView->proxyModel();
    //Check the model row count, this will be used as song count.
    switch(model->rowCount())
    {
    case 0:
        m_categoryInfo->setText(m_songCount[0]);
        break;
    case 1:
        m_categoryInfo->setText(m_songCount[1]);
        break;
    default:
        m_categoryInfo->setText(m_songCount[2].arg(QString::number(model->rowCount())));
        break;
    }
}

void KNMusicCategoryDisplay::setLibraryModel(KNMusicLibraryModel *model)
{
    //Set the model.
    m_categoryTreeView->setMusicModel(model);
    //Set default sort state.
    m_categoryTreeView->sortByColumn(Name, Qt::AscendingOrder);
}

void KNMusicCategoryDisplay::showNoCategoryItem(const QString &title)
{
    //Update the title.
    m_categoryTitle->setText(title);
    //Set the category text to treeview.
    m_categoryTreeView->setCategoryText("");
    //Update the background to no album art.
    setCategoryIcon(QPixmap());
    //Update the detail info.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::setCategoryText(const QString &text)
{
    //Update the title.
    m_categoryTitle->setText(text);
    //Set the category text to treeview.
    m_categoryTreeView->setCategoryText(text);
    //Update the detail info.
    updateDetailInfo();
}

void KNMusicCategoryDisplay::setCategoryIcon(const QIcon &icon)
{
    m_largeIcon->setPixmap(icon.isNull()?
                               KNMusicGlobal::instance()->noAlbumArt():
                               icon.pixmap(m_largeIcon->size()));
}

void KNMusicCategoryDisplay::setCategoryColumn(const int &column)
{
    m_categoryTreeView->setCategoryColumn(column);
}

void KNMusicCategoryDisplay::scrollToRow(const int &row)
{
    m_categoryTreeView->scrollToSongRow(row);
}

void KNMusicCategoryDisplay::scrollToSourceRow(const int &row)
{
    m_categoryTreeView->scrollToSourceSongRow(row);
}

void KNMusicCategoryDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Move the shadow.
    m_leftShadow->setGeometry(0,
                              0,
                              15,
                              height());
    //Update the background icon.
    updateBackgroundIcon();
}

void KNMusicCategoryDisplay::updateBackgroundIcon()
{
    //Get the tree view height.
    int viewHeight=m_categoryTreeView->height();
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
}
