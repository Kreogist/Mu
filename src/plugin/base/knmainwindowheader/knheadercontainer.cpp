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

#include "knheadercontainer.h"

KNHeaderContainer::KNHeaderContainer(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Configure the palette.
    configurePalette();

    //Initial the animation timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(maximumBrightness);
    configureMouseTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(minimumBrightness);
    configureMouseTimeLine(m_mouseOut);
}

void KNHeaderContainer::setHeaderSwitcher(QWidget *switcher)
{
    Q_ASSERT(switcher!=NULL);
    m_switcher=switcher;
    m_switcher->setParent(this);
    //Let the container fit the switcher.
    setFixedHeight(m_switcher->height());
}

void KNHeaderContainer::setHeaderButton(QWidget *button)
{
    Q_ASSERT(button!=NULL);
    //Change the parent relationship.
    button->setParent(this);
    //Reset the position and properties.
    button->move(0,0);
    button->setFixedHeight(height());
    button->lower();
}

void KNHeaderContainer::setLeftSpacing(const int &spacing)
{
    //This property is used to show the category button.
    m_leftSpacing=spacing;
    updateSwitcherPosition();
}

void KNHeaderContainer::enterEvent(QEvent *event)
{
    //Stop all the animation.
    m_mouseOut->stop();
    m_mouseIn->stop();
    //Set mouse in properties and run animation.
    m_mouseIn->setStartFrame(m_backgroundColor.value());
    m_mouseIn->start();
    QWidget::enterEvent(event);
}

void KNHeaderContainer::leaveEvent(QEvent *event)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.value());
    m_mouseOut->start();
    QWidget::leaveEvent(event);
}

void KNHeaderContainer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //When size changed, should update the switcher postion as well.
    updateSwitcherPosition();
}

void KNHeaderContainer::changeBackgroundColor(const int &frameData)
{
    //Update the background color.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             frameData);
    //Set the color to the palette.
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    setPalette(m_palette);
}

void KNHeaderContainer::configurePalette()
{
    //Set the background color to the minimum brightness.
    m_backgroundColor.setHsv(m_backgroundColor.hue(),
                             m_backgroundColor.saturation(),
                             minimumBrightness);
    //Set the palette.
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::Window, m_backgroundColor);
    m_palette.setColor(QPalette::Text, QColor(0x9f, 0x9f, 0x9f));
    setPalette(m_palette);
}

void KNHeaderContainer::configureMouseTimeLine(QTimeLine *timeline)
{
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(5);
    //This animation is going to change to background color, so the each frame
    //will be the parameter of a color. When frame changed, change the color.
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNHeaderContainer::changeBackgroundColor);
}

void KNHeaderContainer::updateSwitcherPosition()
{
    if(m_switcher!=nullptr)
    {
        m_switcher->setGeometry(m_leftSpacing,
                                0,
                                width()-m_leftSpacing,
                                m_switcher->height());
    }
}
