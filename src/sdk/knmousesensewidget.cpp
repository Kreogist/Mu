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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTimeLine>

#include "knthememanager.h"

#include "knmousesensewidget.h"

KNMouseSenseWidget::KNMouseSenseWidget(QWidget *parent) :
    QWidget(parent),
    m_mouseInOut(generateTimeline()),
    m_rangeStart(0),
    m_rangeEnd(0)
{
    //Set properties.
    setAutoFillBackground(true);
    //Add header to theme list.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMouseSenseWidget::onPaletteChanged);
}

void KNMouseSenseWidget::updateObjectName(const QString &name)
{
    //Set the object name.
    setObjectName(name);
    //Update the palette.
    onPaletteChanged();
}

void KNMouseSenseWidget::enterEvent(QEvent *event)
{
    //Start the animation to range end.
    startAnime(m_rangeEnd);
    //Do the original enter event.
    QWidget::enterEvent(event);
}

void KNMouseSenseWidget::leaveEvent(QEvent *event)
{
    //Start the animation to range start.
    startAnime(m_rangeStart);
    //Do the original leave event.
    QWidget::leaveEvent(event);
}

void KNMouseSenseWidget::changeBackgroundColor(int frame)
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

void KNMouseSenseWidget::onPaletteChanged()
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

inline QTimeLine *KNMouseSenseWidget::generateTimeline()
{
    //Generate the time line.
    QTimeLine *timeline=new QTimeLine(200, this);
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    timeline->setUpdateInterval(10);
    //This animation is going to change to background color, so the each frame
    //will be the parameter of a color. When frame changed, change the color.
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNMouseSenseWidget::changeBackgroundColor);
    //Return the time line.
    return timeline;
}

inline void KNMouseSenseWidget::startAnime(int endFrame)
{
    //Stop the time line.
    m_mouseInOut->stop();
    //Set the range of the color from current to range start.
    m_mouseInOut->setFrameRange(m_mouseInOut->currentFrame(),
                                endFrame);
    //Start the time line.
    m_mouseInOut->start();
}

