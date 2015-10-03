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
#include <QPainter>

#include "knopacityanimebutton.h"

#include "knmusicmainplayercontentswitcher.h"

KNMusicMainPlayerContentSwitcher::KNMusicMainPlayerContentSwitcher(
        QWidget *parent) :
    QWidget(parent),
    m_previous(generateButton(":/plugin/music/player/previous_tab.png")),
    m_next(generateButton(":/plugin/music/player/next_tab.png")),
    m_currentIndex(0)
{
    //Link the button.
    connect(m_previous, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContentSwitcher::requirePrevious);
    connect(m_next, &KNOpacityAnimeButton::clicked,
            this, &KNMusicMainPlayerContentSwitcher::requireNext);
}

void KNMusicMainPlayerContentSwitcher::setCurrentIndex(const int &index)
{
    //Save the current index.
    m_currentIndex=index;
    //Update the widget.
    update();
}

void KNMusicMainPlayerContentSwitcher::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Change the position of the button.
    m_previous->move((width()>>1)-54, 0);
    m_next->move((width()>>1)+38, 0);
}

void KNMusicMainPlayerContentSwitcher::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Configure the render hints.
    painter.setRenderHints(QPainter::Antialiasing, true);
    //Draw the page point.
    int pointY=(height()>>1)-3, pointX=(width()>>1)-3;
    //Paint the center circle.
    painter.setPen(QColor(0,0,0,0));
    painter.setBrush(QColor(255,255,255,0x4e));
    painter.drawEllipse(QRect(pointX, pointY, 6, 6));
    //Paint the right circle.
    painter.drawEllipse(QRect(pointX+16, pointY, 6, 6));
    //Paint the left circle.
    painter.drawEllipse(QRect(pointX-16, pointY, 6, 6));
    //Paint the selected circle.
    painter.setBrush(QColor(255,255,255));
    painter.drawEllipse(pointX+16*(m_currentIndex-1), pointY, 6, 6);
}

inline KNOpacityAnimeButton *KNMusicMainPlayerContentSwitcher::generateButton(
        const QString &imagePath)
{
    //Generate the button.
    KNOpacityAnimeButton *button=new KNOpacityAnimeButton(this);
    //Configure the button.
    button->setFixedSize(16, 16);
    button->setIcon(QIcon(imagePath));
    //Give back the button.
    return button;
}

