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
#include <QBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QTimeLine>

#include "kndpimanager.h"
#include "kneditablelabel.h"
#include "knopacityanimebutton.h"

#include "knmusicmainplayercontrol.h"

#define ButtonSpace 21
#define ButtonSize 32

KNMusicMainPlayerControl::KNMusicMainPlayerControl(QWidget *parent) :
    QWidget(parent),
    m_playIcon(QPixmap(":/plugin/music/player/play_light.png")),
    m_pauseIcon(QPixmap(":/plugin/music/player/pause_light.png")),
    m_changeOpacity(new QTimeLine(200, this)),
    m_opacity(new QGraphicsOpacityEffect(this)),
    m_position(new QLabel(this)),
    m_previous(generateButton(":/plugin/music/player/previous_light.png")),
    m_playNPause(generateButton()),
    m_next(generateButton(":/plugin/music/player/next_light.png")),
    m_currentOpacity(0)
{
    //Set properties.
    setFixedHeight(knDpi->height(93));
    setGraphicsEffect(m_opacity);
    //Configure buttons.
    m_playNPause->setIcon(m_playIcon);
    //Configure the time line.
    m_opacity->setOpacity(0.0);
    m_changeOpacity->setUpdateInterval(16);
    m_changeOpacity->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_changeOpacity, &QTimeLine::frameChanged,
            [=](int frameValue)
            {
                //Save the value.
                m_currentOpacity=frameValue;
                //Update the opacity.
                m_opacity->setOpacity((qreal)frameValue/255.0);
            });

    //Set the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add the button layout.
    int buttonSpace=knDpi->width(ButtonSpace);
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);
    mainLayout->addLayout(controlLayout, 1);
    //Add button to the layout.
    controlLayout->addStretch();
    controlLayout->addWidget(m_previous);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_playNPause);
    controlLayout->addSpacing(buttonSpace);
    controlLayout->addWidget(m_next);
    controlLayout->addStretch();
}

void KNMusicMainPlayerControl::enterEvent(QEvent *event)
{
    //Do the original enter event.
    QWidget::enterEvent(event);
    //Display all the widget.
    changeOpacity(255);
}

void KNMusicMainPlayerControl::leaveEvent(QEvent *event)
{
    //Do the original leave event.
    QWidget::leaveEvent(event);
    //Hide all the widget.
    changeOpacity(0);
}

inline KNOpacityAnimeButton *KNMusicMainPlayerControl::generateButton(
        const QString &imagePath)
{
    //Create the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    if(!imagePath.isEmpty())
    {
        //Check the icon image path.
        button->setIcon(QIcon(imagePath));
    }
    button->setFixedSize(knDpi->size(ButtonSize, ButtonSize));
    return button;
}

inline void KNMusicMainPlayerControl::changeOpacity(int endFrame)
{
    //Stop the time line.
    m_changeOpacity->stop();
    //Update the time line value.
    m_changeOpacity->setFrameRange(m_currentOpacity, endFrame);
    //Start the time line.
    m_changeOpacity->start();
}
