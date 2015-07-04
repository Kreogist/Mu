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
#include <QPainter>

#include "knthememanager.h"

#include "knpreferenceitem.h"

#include <QDebug>

#define itemHeight 40
#define shadowHeight 5
#define textBaseX 15
#define shadowOpacity 95
#define iconSize 30

KNPreferenceItem::KNPreferenceItem(QWidget *parent) :
    QAbstractButton(parent),
    m_shadowGradient(QLinearGradient(QPointF(0,0), QPointF(0, shadowHeight))),
    m_mouseIn(generateTimeLine(100)),
    m_mouseOut(generateTimeLine(0)),
    m_progress(0),
    m_textX(textBaseX),
    m_backgroundOpacity(0.0)
{
    setObjectName("PreferenceItem");
    //Set properties.
    setAutoExclusive(true);
    setCheckable(true);
    setContentsMargins(0,0,0,0);
    setFixedHeight(itemHeight);

    //Set the font.
    QFont itemFont=font();
    itemFont.setPixelSize(15);
    setFont(itemFont);

    //Link toggled signal.
    connect(this, &KNPreferenceItem::toggled,
            this, &KNPreferenceItem::onActionToggled);

    //Add widget to theme manager.
    knTheme->registerWidget(this);
}

void KNPreferenceItem::enterEvent(QEvent *event)
{
    //Start enter anime.
    startAnime(m_mouseIn);
    //Do original enter event.
    QAbstractButton::enterEvent(event);
}

void KNPreferenceItem::leaveEvent(QEvent *event)
{
    if(!isChecked())
    {
        //Start leave anime.
        startAnime(m_mouseOut);
    }
    //Do original leave event.
    QAbstractButton::leaveEvent(event);
}

void KNPreferenceItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Draw the select background with the opacity.
    painter.setOpacity(m_backgroundOpacity);
    painter.fillRect(rect(), palette().button());

    //Draw the icon if icon is not null.
    if(!icon().isNull())
    {
        //Change the opacity.
        painter.setOpacity(m_backgroundOpacity*2);
        //Draw the icon.
        painter.drawPixmap(width()-(m_textX<<1),
                           (height()-iconSize)>>1,
                           iconSize,
                           iconSize,
                           icon().pixmap(iconSize, iconSize));
    }

    //Draw the text.
    painter.setOpacity(1.0);
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.setFont(font());
    painter.drawText(m_textX,
                     0,
                     width()-m_textX,
                     height(),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     text());

    //If checked, draw the shadow.
    if(isChecked())
    {
        //Configure the painter.
        painter.setPen(Qt::NoPen);
        //Configure the up shadow.
        m_shadowGradient.setColorAt(0, QColor(0,0,0,shadowOpacity));
        m_shadowGradient.setColorAt(1, QColor(0,0,0,0));
        //Draw the shadow.
        painter.fillRect(QRect(0,
                               0,
                               width(),
                               shadowHeight), m_shadowGradient);
        //Configure the down shadow.
        m_shadowGradient.setColorAt(0, QColor(0,0,0,0));
        m_shadowGradient.setColorAt(1, QColor(0,0,0,shadowOpacity));
        //Change the coordinate.
        painter.translate(0, height()-shadowHeight);
        //Draw the shadow.
        painter.fillRect(QRect(0,
                               0,
                               width(),
                               shadowHeight), m_shadowGradient);
    }
}

void KNPreferenceItem::mouseReleaseEvent(QMouseEvent *event)
{
    //Keep the checked state.
    setChecked(true);
}

void KNPreferenceItem::onActionMouseInOut(const int &frame)
{
    //Save the progress, progress is a number between 1-100.
    m_progress=frame;
    //Update the parameters.
    m_textX=textBaseX+(m_progress>>3);
    m_backgroundOpacity=(qreal)frame/200.0;
    //Redraw the widget.
    update();
}

void KNPreferenceItem::onActionToggled(bool checked)
{
    //Start the anime according to checked state.
    startAnime(checked?m_mouseIn:m_mouseOut);
}

inline QTimeLine *KNPreferenceItem::generateTimeLine(const int &endFrame)
{
    QTimeLine *timeline=new QTimeLine(200, this);
    timeline->setEndFrame(endFrame);
    timeline->setUpdateInterval(10);
    timeline->setEasingCurve(QEasingCurve::OutCubic);
    connect(timeline, &QTimeLine::frameChanged,
            this, &KNPreferenceItem::onActionMouseInOut);
    return timeline;
}

inline void KNPreferenceItem::startAnime(QTimeLine *timeLine)
{
    //Stop all the animation time line.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Change the start frame of the time line and start the anime.
    timeLine->setStartFrame(m_progress);
    timeLine->start();
}
