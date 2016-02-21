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
#include <QSpacerItem>

#include "knmainwindowiconbutton.h"
#include "kncategoryplugin.h"
#include "knlocalemanager.h"

#include "knmainwindowheader.h"

#include <QDebug>

KNMainWindowHeader::KNMainWindowHeader(QWidget *parent) :
    KNMainWindowHeaderBase(parent),
    m_widgetContainer(new QWidget(this)),
    m_notificationLayout(new QBoxLayout(QBoxLayout::LeftToRight,
                                        m_widgetContainer)),
    m_widgetLayout(new QBoxLayout(QBoxLayout::LeftToRight,
                                  m_notificationLayout->widget())),
    m_categoryPlugin(nullptr),
    m_iconButton(new KNMainWindowIconButton(this))
{
    updateObjectName("MainWindowHeader");
    //Set properties.
    setFixedHeight(70);
    //Initial the button position.
    m_iconButton->move(0, 0);

    //Configure the notification layout.
    m_notificationLayout->setContentsMargins(0,0,0,0);
    m_notificationLayout->setSpacing(0);
    //Add the notification layout to container.
    m_widgetContainer->setLayout(m_notificationLayout);
    //Configure the widget layout.
    m_widgetLayout->setContentsMargins(0,0,0,0);
    m_widgetLayout->setSpacing(0);
    //Add widget layout to notification layout.
    m_notificationLayout->addLayout(m_widgetLayout, 1);

    //Link the icon button.
    connect(m_iconButton, &KNMainWindowIconButton::clicked,
            this, &KNMainWindowHeader::requireShowPreference);
    connect(m_iconButton, &KNMainWindowIconButton::buttonSizeChange,
            this, &KNMainWindowHeader::updateContainerSize);
    //Update the container size right now.
    updateContainerSize();

    //Link retranlsate.
    knI18n->link(this, &KNMainWindowHeader::retranslate);
    retranslate();
}

void KNMainWindowHeader::addHeaderWidget(QWidget *widget,
                                         int stretch,
                                         Qt::Alignment alignment)
{
    //Add the widget to widget layout.
    m_widgetLayout->addWidget(widget, stretch, alignment);
}

void KNMainWindowHeader::addNotificationButton(QWidget *widget)
{
    //Then add spacing to layout.
    m_notificationLayout->addSpacing(4);
    //Add widget to notification layout.
    m_notificationLayout->addWidget(widget, 0, Qt::AlignCenter);
    //Then add spacing to layout.
    m_notificationLayout->addSpacing(14);
}

void KNMainWindowHeader::retranslate()
{
    //Check the category plugin first.
    if(m_categoryPlugin!=nullptr)
    {
        //Update the button text.
        m_iconButton->setButtonText(m_categoryPlugin->title());
    }
}

void KNMainWindowHeader::resizeEvent(QResizeEvent *event)
{
    //Do the original resize event.
    KNMainWindowHeaderBase::resizeEvent(event);
    //Change the size of widget container.
    updateContainerSize();
}

void KNMainWindowHeader::updateContainerSize()
{
    //Calculate the prefer widget size.
    int containerWidth=width()-m_iconButton->width();
    //Check the validation of the widget.
    containerWidth=(containerWidth<0)?0:containerWidth;
    //Update the widget size.
    m_widgetContainer->setGeometry(width()-containerWidth,
                                   0,
                                   containerWidth,
                                   height());
}

void KNMainWindowHeader::setCategoryPlugin(KNCategoryPlugin *categoryPlugin)
{
    //Save the category plugin.
    m_categoryPlugin=categoryPlugin;
    //Ensure this is not a null widget.
    if(m_categoryPlugin==nullptr)
    {
        return;
    }
    //Add the header widget.
    addHeaderWidget(m_categoryPlugin->headerWidget());
    //Set the icon of the plugin.
    m_iconButton->setButtonIcon(m_categoryPlugin->icon());
    //Set the text of the plugin.
    m_iconButton->setButtonText(m_categoryPlugin->title());
}
