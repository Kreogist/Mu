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
#include <QTimeLine>
#include <QBoxLayout>

#include "knthememanager.h"
#include "knmainwindowiconbutton.h"
#include "kncategoryplugin.h"
#include "knlocalemanager.h"

#include "knmainwindowheader.h"

#include <QDebug>

KNMainWindowHeader::KNMainWindowHeader(QWidget *parent) :
    KNMainWindowHeaderBase(parent),
    m_mouseInOut(generateTimeline()),
    m_widgetLayout(new QBoxLayout(QBoxLayout::LeftToRight)),
    m_categoryPlugin(nullptr),
    m_iconButton(new KNMainWindowIconButton(this)),
    m_rangeStart(0),
    m_rangeEnd(0)
{
    setObjectName("MainWindowHeader");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(70);
    //Add header to theme list.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMainWindowHeader::onActionPaletteChanged);
    onActionPaletteChanged();

    //Initial the layout of the header.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add button to main layout.
    mainLayout->addWidget(m_iconButton);
    //Configure the widget layout.
    m_widgetLayout->setContentsMargins(0,0,0,0);
    m_widgetLayout->setSpacing(0);
    //Add the widget layout to header.
    mainLayout->addLayout(m_widgetLayout, 1);

    //Link the icon button.
    connect(m_iconButton, SIGNAL(clicked(bool)),
            this, SIGNAL(requireShowPreference()));

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

void KNMainWindowHeader::enterEvent(QEvent *event)
{
    //Stop the time line.
    m_mouseInOut->stop();
    //Set the range of the color from current to range end.
    m_mouseInOut->setFrameRange(m_mouseInOut->currentFrame(),
                                m_rangeEnd);
    //Start the time line.
    m_mouseInOut->start();
    //Do the original enter event.
    KNMainWindowHeaderBase::enterEvent(event);
}

void KNMainWindowHeader::leaveEvent(QEvent *event)
{
    //Stop the time line.
    m_mouseInOut->stop();
    //Set the range of the color from current to range start.
    m_mouseInOut->setFrameRange(m_mouseInOut->currentFrame(),
                                m_rangeStart);
    //Start the time line.
    m_mouseInOut->start();
    //Do the original leave event.
    KNMainWindowHeaderBase::leaveEvent(event);
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

void KNMainWindowHeader::changeBackgroundColor(const int &frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Get the background ground color, change the background color.
    QColor backgroundColor=pal.color(QPalette::Window);
    backgroundColor.setHsv(backgroundColor.hue(),
                           backgroundColor.saturation(),
                           frame);
    pal.setColor(QPalette::Window, backgroundColor);
    //Set the palette.
    setPalette(pal);
}

void KNMainWindowHeader::onActionPaletteChanged()
{
    //Set the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Update the palette information.
    QColor backgroundColor=palette().color(QPalette::Window).toHsv();
    //The start brightness is the current brightness.
    m_rangeStart=backgroundColor.value();
    //If the color is a light color, the end of the range will be darker.
    m_rangeEnd=m_rangeStart+((backgroundColor.value()>0xBE)?-0x40:0x40);
}

inline QTimeLine *KNMainWindowHeader::generateTimeline()
{
    //Generate the time line.
    QTimeLine *timeline=new QTimeLine(200, this);
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(10);
    //This animation is going to change to background color, so the each frame
    //will be the parameter of a color. When frame changed, change the color.
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNMainWindowHeader::changeBackgroundColor);
    //Return the time line.
    return timeline;
}

void KNMainWindowHeader::setCategoryPlugin(KNCategoryPlugin *categoryPlugin)
{
    //Save the category plugin.
    m_categoryPlugin = categoryPlugin;
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
