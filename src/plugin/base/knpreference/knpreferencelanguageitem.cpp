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
#include <QPaintEvent>
#include <QPainter>
#include <QTimeLine>

#include "knpreferencelanguageitem.h"

#include <QDebug>

#define shadowOpacity 95

KNPreferenceLanguageItem::KNPreferenceLanguageItem(QWidget *parent) :
    KNAnimeCheckedButton(parent)
{
    //Set properties.
    setFixedHeight(m_itemHeight);

    //Set the font.
    QFont itemFont=font();
    itemFont.setPixelSize(15);
    setFont(itemFont);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);

    //Set shadow gradient.
    m_shadowGradient.setStart(0,0);
    m_shadowGradient.setFinalStop(0,m_shadowHeight);

    //Set the animation timeline.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEndFrame(100);
    configureTimeLine(m_mouseIn);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEndFrame(0);
    configureTimeLine(m_mouseOut);
}

QPixmap KNPreferenceLanguageItem::icon() const
{
    return m_icon;
}

void KNPreferenceLanguageItem::setIcon(const QPixmap &icon)
{
    //Scale the icon.
    m_icon=icon.scaled(m_iconSize, m_iconSize,
                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //Redraw the icon.
    update();
}

QString KNPreferenceLanguageItem::text() const
{
    return QString();
}

void KNPreferenceLanguageItem::setText(const QString &text)
{
    m_text=text;
    update();
}

void KNPreferenceLanguageItem::startMouseInAnime()
{
    //Stop all the animation time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Change the start frame and start the anime.
    m_mouseIn->setStartFrame(m_progress);
    m_mouseIn->start();
}

void KNPreferenceLanguageItem::startMouseOutAnime()
{
    //Stop all the animation time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Change the start frame and start the anime.
    m_mouseOut->setStartFrame(m_progress);
    m_mouseOut->start();
}

void KNPreferenceLanguageItem::startMouseDownAnime()
{
}

void KNPreferenceLanguageItem::startMouseUpAnime()
{
    startMouseInAnime();
    update();
}

void KNPreferenceLanguageItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw the select background with the opacity.
    painter.setOpacity(m_backgroundOpacity);
    painter.setPen(Qt::NoPen);
    painter.fillRect(rect(), palette().window());
    //Draw the icon.
    painter.setOpacity(1.0);
    painter.drawPixmap(m_iconX,
                       (height()-m_iconSize)>>1,
                       m_iconSize,
                       m_iconSize,
                       m_icon);
    //Draw the text.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.setFont(font());
    painter.drawText(m_textX,
                     0,
                     width()-m_textX,
                     height(),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_text);
    //If checked, draw the shadow.
    if(checked())
    {
        //Draw the up shadow.
        m_shadowGradient.setColorAt(0, QColor(0,0,0,shadowOpacity));
        m_shadowGradient.setColorAt(1, QColor(0,0,0,0));
        painter.setPen(Qt::NoPen);
        painter.fillRect(QRect(0,0,width(),m_shadowHeight), m_shadowGradient);
        //Draw the down shadow.
        //Change the coordinate.
        painter.translate(0,height()-m_shadowHeight);
        m_shadowGradient.setColorAt(0, QColor(0,0,0,0));
        m_shadowGradient.setColorAt(1, QColor(0,0,0,shadowOpacity));
        painter.fillRect(QRect(0,0,width(),m_shadowHeight), m_shadowGradient);
    }
}

void KNPreferenceLanguageItem::onActionMouseInOut(const int &frame)
{
    //Save the progress, progress is a number between 1-100.
    m_progress=frame;
    //Update the parameters.
    m_backgroundOpacity=(qreal)frame/200.0;
    //Redraw the widget.
    update();
}

void KNPreferenceLanguageItem::configureTimeLine(QTimeLine *timeLine)
{
    timeLine->setUpdateInterval(16);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNPreferenceLanguageItem::onActionMouseInOut);
}
