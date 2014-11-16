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

#include "knsideshadowwidget.h"
#include "knmousesensewidget.h"
#include "knmusiclibrarymodel.h"
#include "knmusiclibrarytreeview.h"

#include "knlocalemanager.h"

#include "knmusiccategorydisplay.h"

KNMusicCategoryDisplay::KNMusicCategoryDisplay(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

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
    titleFont.setPixelSize(17);
    titleFont.setBold(true);
    m_categoryTitle->setFont(titleFont);
    infoLayout->addWidget(m_categoryTitle);
    m_categoryInfo=new QLabel(this);
    m_categoryInfo->setPalette(captionPal);
    infoLayout->addWidget(m_categoryInfo);

    //Initial the tree view.
    m_categoryTreeView=new KNMusicLibraryTreeView(this);
    displayLayout->addWidget(m_categoryTreeView, 1);

    //Initial the shadow.
    m_leftShadow=new KNSideShadowWidget(LeftShadow, this);

    //Connect retranslate signal.
    connect(KNLocaleManager::instance(), &KNLocaleManager::requireRetranslate,
            this, &KNMusicCategoryDisplay::retranslate);
    //Do retranslate.
    retranslate();
}

void KNMusicCategoryDisplay::retranslate()
{
    ;
}

void KNMusicCategoryDisplay::updateDetailInfo()
{
    ;
}

void KNMusicCategoryDisplay::setLibraryModel(KNMusicLibraryModel *model)
{
    m_categoryTreeView->setMusicModel(model);
}

void KNMusicCategoryDisplay::setCategoryText(const QString &text)
{
    //Update the title.
    m_categoryTitle->setText(text);
    //Set the category text to treeview.
    m_categoryTreeView->setCategoryText(text);
}

void KNMusicCategoryDisplay::setCategoryColumn(const int &column)
{
    m_categoryTreeView->setCategoryColumn(column);
}

void KNMusicCategoryDisplay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //Move the shadow.
    m_leftShadow->setGeometry(0,
                              0,
                              15,
                              height());
}
