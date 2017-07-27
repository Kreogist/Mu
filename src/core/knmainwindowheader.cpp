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

#include "knmainwindowstatusbar.h"
#include "knmainwindowiconbutton.h"
#include "kncategoryplugin.h"
#include "knlocalemanager.h"
#include "knconfigure.h"
#include "kndpimanager.h"

#include "knmainwindowheader.h"

#include <QDebug>

KNMainWindowHeader::KNMainWindowHeader(QWidget *parent) :
    KNMainWindowHeaderBase(parent),
    m_widgetContainer(new QWidget(this)),
    m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight,
                                  m_widgetContainer)),
    m_notificationLayout(new QBoxLayout(QBoxLayout::LeftToRight,
                                        m_mainLayout->widget())),
    m_categoryPlugin(nullptr),
    m_appearanceConfigure(nullptr),
    m_iconButton(new KNMainWindowIconButton(this)),
    m_statusBar(new KNMainWindowStatusBar(this))
{
    updateObjectName("MainWindowHeader");
    //Set properties.
    setFixedHeight(knDpi->height(70));
    //Initial the button position.
    m_iconButton->move(0, 0);

    //Configure the main layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    //Set main layout to the widget container.
    m_widgetContainer->setLayout(m_mainLayout);
    //Add the status layout.
    QBoxLayout *statusLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            m_mainLayout->widget());
    statusLayout->setContentsMargins(0, 0, 0, 0);
    statusLayout->setSpacing(0);
    m_mainLayout->addLayout(statusLayout);
    //Add status bar to layout.
    statusLayout->addWidget(m_statusBar, 0, Qt::AlignRight);
    //Configure the notification layout.
    m_notificationLayout->setContentsMargins(0, 0, 0, 0);
    m_notificationLayout->setSpacing(0);
    //Add widget layout to notification layout.
    statusLayout->addLayout(m_notificationLayout, 1);

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
    m_mainLayout->insertWidget(m_mainLayout->count()-1,
                               widget, stretch, alignment);
}

void KNMainWindowHeader::addNotificationWidget(QWidget *widget)
{
    //Check the layout count first.
    if(m_notificationLayout->count()==0)
    {
        //Add spacing.
        m_notificationLayout->addSpacing(knDpi->width(NotificationHeadSpacing));
        //Add widget to notification layout.
        m_notificationLayout->addWidget(widget, 0, Qt::AlignCenter);
        //Insert spacing to layout.
        m_notificationLayout->addSpacing(knDpi->width(NotificationTailSpacing));
    }
    else
    {
        //Insert spacing to layout.
        m_notificationLayout->insertSpacing(
                    0, knDpi->width(NotificationTailSpacing));
        //Insert to the one widget before.
        m_notificationLayout->insertWidget(0, widget, 0, Qt::AlignCenter);
        //Insert head spacing.
        m_notificationLayout->insertSpacing(
                    0, knDpi->width(NotificationHeadSpacing));
    }
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

void KNMainWindowHeader::onConfigureChanged()
{
    //Check the configure pointer.
    if(!m_appearanceConfigure)
    {
        //Ignore the invalid call.
        return;
    }
    //Update the status bar visibility.
    m_statusBar->setVisible(
                m_appearanceConfigure->data("ShowStatusBar", true).toBool());
}

void KNMainWindowHeader::setAppearanceConfigure(KNConfigure *configure)
{
    //Save the configure object pointer.
    m_appearanceConfigure = configure;
    //Change the configure object.
    connect(m_appearanceConfigure, &KNConfigure::valueChanged,
            this, &KNMainWindowHeader::onConfigureChanged);
    //Apply the configure to the header.
    onConfigureChanged();
}

int KNMainWindowHeader::minimumWidthHint()
{
    //This will simply return the widget of the header.
    return m_iconButton->width();
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

void KNMainWindowHeader::addStatusWidget(QWidget *widget)
{
    //Add the widget to the status bar.
    m_statusBar->addWidget(widget);
}
