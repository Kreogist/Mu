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

#include "knthememanager.h"
#include "knscrollarea.h"

#include "knmusicstoreheader.h"
#include "knmusicstoreutil.h"

#include "knmusicstorecontainer.h"

#define MaxContentWidth 1008

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
    m_pageContainer->updateObjectName("MusicStorePageContainer");

    //Register the widget.
    knTheme->registerWidget(this);
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
}
