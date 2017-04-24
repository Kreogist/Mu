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
#include <QTimeLine>

#include "knthememanager.h"
#include "kncategorytab.h"

#include "kncategorytabbar.h"

#define InBrightness 0x40
#define OutBrightness 0x20

KNCategoryTabBar::KNCategoryTabBar(QWidget *parent) :
    KNAbstractTabGroup(parent),
    m_mouseIn(generateTimeLine(InBrightness)),
    m_mouseOut(generateTimeLine(OutBrightness)),
    m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight, this))
{
    setObjectName("CategoryTabBar");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(KNCategoryTab::tabHeight());

    //Set layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    //Update the palette using the frame function.
    onActionMouseInOut(OutBrightness);
}

bool KNCategoryTabBar::isEmpty()
{
    //The group empty state is the same as the layout state.
    return m_mainLayout->isEmpty();
}

int KNCategoryTabBar::addTabToWidget(QAbstractButton *tab)
{
    //Add tab item to the layout.
    m_mainLayout->addWidget(tab);
    //The maximum index of the main layout will be the tab's index.
    return m_mainLayout->count()-1;
}

QAbstractButton *KNCategoryTabBar::tabAt(int index)
{
    Q_ASSERT(index>-1 && index<m_mainLayout->count());
    //Get the index from the main layout.
    return static_cast<QAbstractButton *>
            (m_mainLayout->itemAt(index)->widget());
}

void KNCategoryTabBar::enterEvent(QEvent *event)
{
    //Start mouse in anime.
    startAnime(m_mouseIn);
    //Do original enter event.
    KNAbstractTabGroup::enterEvent(event);
}

void KNCategoryTabBar::leaveEvent(QEvent *event)
{
    //Start mouse out anime.
    startAnime(m_mouseOut);
    //Do original leave event.
    KNAbstractTabGroup::enterEvent(event);
}

void KNCategoryTabBar::onActionMouseInOut(const int &frame)
{
    //Get the palette and background color.
    QPalette pal=palette();
    QColor backgroundColor=pal.color(QPalette::Window);
    //Change the brightness of the window widget, but can only set hsv in this
    //mode.
    backgroundColor.setHsv(backgroundColor.hue(),
                           backgroundColor.saturation(),
                           frame);
    //Update the palette.
    pal.setColor(QPalette::Window, backgroundColor);
    //Set the palette.
    setPalette(pal);
}

inline QTimeLine *KNCategoryTabBar::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeLine=new QTimeLine(200, this);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setEndFrame(endFrame);
    timeLine->setUpdateInterval(10);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNCategoryTabBar::onActionMouseInOut);
    return timeLine;
}

inline void KNCategoryTabBar::startAnime(QTimeLine *timeLine)
{
    //Stop all animation.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Configrue the start frame of the time line.
    timeLine->setStartFrame(palette().color(QPalette::Window).value());
    //Start the time line.
    timeLine->start();
}
