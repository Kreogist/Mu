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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QScrollBar>

#include "knthememanager.h"
#include "knscrollarea.h"

#include "knmusicstorepagehome.h"
#include "knmusicstorepagealbum.h"
#include "knmusicstorepagesinglesong.h"
#include "knmusicstoreheader.h"
#include "knmusicstoreutil.h"

#include "knmusicstorecontainer.h"

#define MaxContentWidth 1108

KNMusicStoreContainer::KNMusicStoreContainer(QWidget *parent) :
    QWidget(parent),
    m_pageContainer(new KNScrollArea(this)),
    m_headerContainer(new QWidget(this)),
    m_header(new KNMusicStoreHeader(this))
{
    setObjectName("MusicStoreContainer");
    //Set the properties.
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);
    //Configure the header container.
    m_headerContainer->setAutoFillBackground(true);
    m_headerContainer->setObjectName("MusicStoreHeaderContainer");
    knTheme->registerWidget(m_headerContainer);
    //Initial the header container layout.
    QBoxLayout *headerLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(0);
    m_headerContainer->setLayout(headerLayout);
    headerLayout->addStretch();
    headerLayout->addWidget(m_header);
    headerLayout->addStretch();
    //Configure the header.
    m_header->setChangeOpacity(true);
    m_header->setSenseRange(0x00, 0x15);
    m_header->updateObjectName("MusicStoreHeader");
    //Configure the page container.
    m_pageContainer->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_pageContainer->setVScrollBarTopMargin(KNMusicStoreUtil::headerHeight());
    //Configure the scroll bar.
    QScrollBar *containerScrollBar=m_pageContainer->vScrollBar();
    containerScrollBar->setObjectName("MusicStoreScrollBar");
    knTheme->registerWidget(containerScrollBar);
    containerScrollBar=m_pageContainer->hScrollBar();
    containerScrollBar->setObjectName("MusicStoreScrollBar");
    knTheme->registerWidget(containerScrollBar);
    //Update the page container object name.
    m_pageContainer->updateObjectName("MusicStorePageContainer");
    //Initial the pages.
    m_pages[PageSingleSong]=new KNMusicStorePageSingleSong(m_pageContainer);
    m_pages[PageAlbum]=new KNMusicStorePageAlbum(m_pageContainer);
    //Configure all widgets.
    configurePage(m_pages[PageSingleSong]);
    configurePage(m_pages[PageAlbum]);

    //Register the widget.
    knTheme->registerWidget(this);
}

KNMusicStorePage *KNMusicStoreContainer::page(int pageIndex)
{
    return m_pages[pageIndex];
}

void KNMusicStoreContainer::setNavigatorText(int itemIndex, const QString &text)
{
    //Update the header navigator text.
    m_header->setNavigatorText(itemIndex, text);
}

void KNMusicStoreContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Get current width.
    int contentWidth=width();
    //Calculate the content width.
    contentWidth=(contentWidth>MaxContentWidth)?MaxContentWidth:contentWidth;
    //Set the content width to widgets.
    m_header->setFixedWidth(contentWidth);
    //Update the header container width.
    m_headerContainer->resize(width(), KNMusicStoreUtil::headerHeight());
    //Update the page container width.
    m_pageContainer->resize(size());
    //Update the page widget size.
    updatePageWidth();
}

void KNMusicStoreContainer::onShowPage()
{
    //The check the sender, cast it as page widget.
    KNMusicStorePage *pageWidget=static_cast<KNMusicStorePage *>(sender());
    //Check the page container.
    if(m_pageContainer->widget())
    {
        //Clear the widget.
        KNMusicStorePage *originalPageWidget=
                static_cast<KNMusicStorePage *>(m_pageContainer->takeWidget());
        //Hide the page widget, set the page parent to this.
        originalPageWidget->setParent(this);
    }
    //Set the container to manage page widget.
    m_pageContainer->setWidget(pageWidget);
    //Update the page widget size.
    updatePageWidth();
}

void KNMusicStoreContainer::onShowAlbum(const QString &metadata)
{
    //The check the sender, cast it as page widget.
    KNMusicStorePage *pageWidget=static_cast<KNMusicStorePage *>(sender());
    //Resent the signal with name of backend.
    emit requireShowAlbum(pageWidget->backendName(),
                          metadata);
}

void KNMusicStoreContainer::onShowSingleSong(const QString &metadata)
{
    //The check the sender, cast it as page widget.
    KNMusicStorePage *pageWidget=static_cast<KNMusicStorePage *>(sender());
    //Resent the signal with name of backend.
    emit requireShowSingleSong(pageWidget->backendName(),
                               metadata);
}

inline void KNMusicStoreContainer::updatePageWidth()
{
    //Update the content widget.
    KNMusicStorePage *pageWidget=
            static_cast<KNMusicStorePage *>(m_pageContainer->widget());
    //Check the page widget.
    if(pageWidget==nullptr)
    {
        //Ignore the page widget.
        return;
    }
    //Resize the page widget.
    pageWidget->setFixedWidth(m_header->width());
    //Check the size hint.
    pageWidget->setFixedHeight(qMax(height(),
                                    pageWidget->sizeHint().height()));
}

inline void KNMusicStoreContainer::configurePage(KNMusicStorePage *pageWidget)
{
    //Link the page signals.
    connect(pageWidget, &KNMusicStorePage::requireShowPage,
            this, &KNMusicStoreContainer::onShowPage);
    connect(pageWidget, &KNMusicStorePage::requireShowAlbum,
            this, &KNMusicStoreContainer::onShowAlbum);
    connect(pageWidget, &KNMusicStorePage::requireShowSingleSong,
            this, &KNMusicStoreContainer::onShowSingleSong);
    //Hide the page.
    pageWidget->hide();
}
