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
#include <QTimeLine>

#include "knsaobutton.h"

#define HoverLight 800
#define BaseLight 0

KNSaoButton::KNSaoButton(QWidget *parent) :
    QAbstractButton(parent),
    m_brightness(0.0),
    m_mouseAnime(new QTimeLine(200, this))
{
    //Configure the time line.
    m_mouseAnime->setUpdateInterval(16);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNSaoButton::onActionMouseAnime);

    //Get the palette.
    QPalette pal=palette();
    //Configure the palette.
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    //Set the palette.
    setPalette(pal);

    //Change the default font size.
    QFont buttonFont=font();
    //Set the new font.
    buttonFont.setPixelSize(16);
    //Set the font.
    setFont(buttonFont);
}

QSize KNSaoButton::sizeHint() const
{
    return QSize(fontMetrics().width(text()), 32);
}

void KNSaoButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the background.
    painter.fillRect(rect(), QColor(0,0,0,100));
    painter.setOpacity(m_brightness);
    painter.fillRect(rect(), palette().color(QPalette::Highlight));
    //Calculate the content color parameter.
    int parameter=255.0*m_brightness;
    //Draw the content.
    painter.setOpacity(1.0);
    painter.setPen(QColor(parameter, parameter, parameter));
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void KNSaoButton::enterEvent(QEvent *event)
{
    //Start the original enter event.
    QAbstractButton::enterEvent(event);
    //Start the mouse in animation.
    startAnime(1000);
}

void KNSaoButton::leaveEvent(QEvent *event)
{
    //Start the original leave event.
    QAbstractButton::leaveEvent(event);
    //Start the mouse out animation.
    startAnime(0);
}

void KNSaoButton::onActionMouseAnime(const int &frame)
{
    //Update the brightness.
    m_brightness=frame/1000.0;
    //Update the UI.
    update();
}

void KNSaoButton::startAnime(const int &endFrame)
{
    //Stop the anime time line.
    m_mouseAnime->stop();
    //Reset the range.
    m_mouseAnime->setFrameRange(m_brightness*1000.0, endFrame);
    //Start the mouse anime.
    m_mouseAnime->start();
}

