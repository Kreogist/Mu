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
#include <QTimeLine>

#include "knthememanager.h"

#include "knmusicstoreerrordimmer.h"

#define MinBackgroundAlpha 0
#define ErrorBackgroundAlpha 100
#define MaxBackgroundAlpha 255

KNMusicStoreErrorDimmer::KNMusicStoreErrorDimmer(QWidget *parent) :
    QWidget(parent),
    m_background(new QTimeLine(200, this)),
    m_showLabel(false)
{
    setObjectName("MusicStoreErrorDimmer");
    //Set properties.
    setAutoFillBackground(true);
    //Configure the time line.
    m_background->setStartFrame(0);
    m_background->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_background, &QTimeLine::frameChanged,
            this, &KNMusicStoreErrorDimmer::onBackgroundAlphaChange);
    connect(m_background, &QTimeLine::finished,
            this, &KNMusicStoreErrorDimmer::onTimeLineFinish);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMusicStoreErrorDimmer::onThemeChanged);
    onThemeChanged();
}

void KNMusicStoreErrorDimmer::showDimmer()
{
    //Show the widget.
    show();
    //Set the show label text to false.
    m_showLabel=false;
    //Update the show dimmer.
    startAnime(MaxBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::showErrorDimmer()
{
    //Show the widget.
    show();
    //Set the show label text to false.
    m_showLabel=true;
    //Update the show dimmer.
    startAnime(ErrorBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::hideDimmer()
{
    //Start to hide the dimmer.
    startAnime(MinBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::onThemeChanged()
{
    //Update the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Update the background by calling the frame change slot.
    onBackgroundAlphaChange(MinBackgroundAlpha);
}

void KNMusicStoreErrorDimmer::onTimeLineFinish()
{
    //Check show label flag for show the dimmer.
    if((m_background->endFrame()==MaxBackgroundAlpha))
    {
        //Check the label.
        if(m_showLabel)
        {
            //Show the label.
            //! FIXME: Add show label codes here.
        }
    }
    else
    {
        //Hide the widget.
        hide();
    }
}

void KNMusicStoreErrorDimmer::onBackgroundAlphaChange(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Get the background color.
    QColor backgroundColor=palette().color(QPalette::Window);
    //Update the alpha.
    backgroundColor.setAlpha(frame);
    //Update the background palette.
    pal.setColor(QPalette::Window, backgroundColor);
    //Set the palette.
    setPalette(pal);
}

inline void KNMusicStoreErrorDimmer::startAnime(int endFrame)
{
    //Stop the background.
    m_background->stop();
    //Update stop the current time line.
    m_background->setEndFrame(endFrame);
    //Start the background.
    m_background->start();
}
