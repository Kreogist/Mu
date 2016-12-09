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

#include "knanimelabelbutton.h"

KNAnimeLabelButton::KNAnimeLabelButton(QWidget *parent) :
    KNLabelButton(parent),
    m_timeLine(new QTimeLine(200, this)),
    m_minBrightness(0x9D),
    m_maxBrightness(0xFF)
{
    //Configure the time line.
    m_timeLine->setUpdateInterval(33);
    m_timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_timeLine, &QTimeLine::frameChanged,
            this, &KNAnimeLabelButton::onUpdatePalette);
}

void KNAnimeLabelButton::updateObjectName(const QString &name)
{
    //Set the object name.
    setObjectName(name);
    //Update the palette.
    setPalette(knTheme->getPalette(name));
    //Update the brightness.
    onUpdatePalette(m_minBrightness);
}

void KNAnimeLabelButton::setRange(int min, int max)
{
    //Save the min and max brightness.
    m_minBrightness=min;
    m_maxBrightness=max;
    //update the brightness.
    onUpdatePalette(m_minBrightness);
}

void KNAnimeLabelButton::enterEvent(QEvent *event)
{
    //Do the original enter event.
    KNLabelButton::enterEvent(event);
    //Start the anime.
    startAnime(m_maxBrightness);
}

void KNAnimeLabelButton::leaveEvent(QEvent *event)
{
    //Do the original leave event.
    KNLabelButton::leaveEvent(event);
    //Start the anime.
    startAnime(m_minBrightness);
}

void KNAnimeLabelButton::onUpdatePalette(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Get the window text frame.
    QColor textColor=pal.color(QPalette::WindowText);
    //Update the brightness.
    textColor.setHsv(textColor.hue(),
                     textColor.saturation(),
                     frame);
    //Set the window text color.
    pal.setColor(QPalette::WindowText, textColor);
    //Set the palette.
    setPalette(pal);
}

inline void KNAnimeLabelButton::startAnime(int endFrame)
{
    //Stop the time line.
    m_timeLine->stop();
    //Update the range.
    m_timeLine->setFrameRange(palette().color(QPalette::WindowText).value(),
                              endFrame);
    //Start the time line.
    m_timeLine->start();
}
