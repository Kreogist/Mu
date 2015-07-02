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
#include <QPainter>
#include <QTimeLine>

#include "knthememanager.h"

#include "knpreferenceheaderbutton.h"

KNPreferenceHeaderButton::KNPreferenceHeaderButton(QWidget *parent) :
    QAbstractButton(parent),
    m_border(QPainterPath(QPoint(0,0))),
    m_icon(QPixmap()),
    m_closeIcon(QPixmap("://public/close.png")),
    m_closeIconOpacity(0.0),
    m_iconPosition(QPointF(11,7)),
    m_mouseIn(generateTimeLine(100)),
    m_mouseOut(generateTimeLine(0))
{
    setObjectName("PreferenceHeaderButton");
    //Set properties.
    setFixedSize(106, 64);
    //Initial the border.
    m_border.lineTo(0,64);
    m_border.lineTo(47,64);
    m_border.cubicTo(75,64,75,0,106,0);
    m_border.lineTo(0,0);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
}

bool KNPreferenceHeaderButton::hitButton(const QPoint &pos) const
{
    //The valid click areas of the header button is the area of painter path.
    return m_border.contains(pos);
}

void KNPreferenceHeaderButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Paint the area.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Window));
    painter.drawPath(m_border);

    //Draw the close icon.
    painter.setOpacity(m_closeIconOpacity);
    painter.drawPixmap(m_iconPosition, m_closeIcon);
    //Draw the content.
    if(!m_icon.isNull())
    {
        painter.setOpacity(1-m_closeIconOpacity);
        painter.drawPixmap(m_iconPosition, m_icon);
    }
}

void KNPreferenceHeaderButton::enterEvent(QEvent *event)
{
    //Start mouse in animation.
    startAnime(m_mouseIn);
    //Do the default enterEvent.
    QAbstractButton::enterEvent(event);
}

void KNPreferenceHeaderButton::leaveEvent(QEvent *event)
{
    //Start mouse out animation.
    startAnime(m_mouseOut);
    //Do the default leaveEvent.
    QAbstractButton::leaveEvent(event);
}

void KNPreferenceHeaderButton::onActionMouseInOut(const int &frame)
{
    //Update the close icon opacity.
    m_closeIconOpacity=(qreal)frame/100;
    //Repaint the button.
    update();
}

inline QTimeLine *KNPreferenceHeaderButton::generateTimeLine(
        const int &endFrame)
{
    //Generate the time line.
    QTimeLine *timeLine=new QTimeLine(200, this);
    //Configure the time line.
    timeLine->setEndFrame(endFrame);
    timeLine->setEasingCurve(QEasingCurve::OutCubic);
    timeLine->setUpdateInterval(10);
    //Link the time line.
    connect(timeLine, &QTimeLine::frameChanged,
            this, &KNPreferenceHeaderButton::onActionMouseInOut);
    return timeLine;
}

inline void KNPreferenceHeaderButton::startAnime(QTimeLine *timeLine)
{
    //Stop all the animations.
    m_mouseIn->stop();
    m_mouseOut->stop();
    //Set the initial frame of the time line.
    timeLine->setStartFrame(m_closeIconOpacity*100);
    //Start animation.
    timeLine->start();
}

QPixmap KNPreferenceHeaderButton::icon() const
{
    return m_icon;
}

void KNPreferenceHeaderButton::setIcon(const QPixmap &icon)
{
    //Save the icon.
    m_icon=icon;
    //Update the painting.
    update();
}
