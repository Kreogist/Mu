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
    m_mouseAnime(new QTimeLine(200, this)),
    m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight, this))
{
    setObjectName("CategoryTabBar");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(KNCategoryTab::tabHeight());
    //Configure the animation.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(16);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNCategoryTabBar::onMouseInOut);
    //Set layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    //Update the palette using the frame function.
    onMouseInOut(OutBrightness);
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
    startAnime(InBrightness);
    //Do original enter event.
    KNAbstractTabGroup::enterEvent(event);
}

void KNCategoryTabBar::leaveEvent(QEvent *event)
{
    //Start mouse out anime.
    startAnime(OutBrightness);
    //Do original leave event.
    KNAbstractTabGroup::enterEvent(event);
}

void KNCategoryTabBar::onMouseInOut(int frame)
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

inline void KNCategoryTabBar::startAnime(int endFrame)
{
    //Stop all animation.
    m_mouseAnime->stop();
    //Configrue the start and end frame of the time line.
    m_mouseAnime->setFrameRange(palette().color(QPalette::Window).value(),
                                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
