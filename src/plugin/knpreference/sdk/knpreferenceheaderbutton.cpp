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
#include "kndpimanager.h"

#include "knpreferenceheaderbutton.h"

#define IconSize 48

KNPreferenceHeaderButton::KNPreferenceHeaderButton(QWidget *parent) :
    QAbstractButton(parent),
    m_border(QPainterPath(QPoint(0,0))),
    m_icon(QPixmap()),
    m_closeIcon(QPixmap("://public/close.png").scaled(
                    knDpi->size(IconSize, IconSize),
                    Qt::KeepAspectRatio, Qt::SmoothTransformation)),
    m_closeIconOpacity(0.0),
    m_iconPosition(knDpi->posF(11,7)),
    m_mouseInOut(new QTimeLine(200, this))
{
    setObjectName("PreferenceHeaderButton");
    //Set properties.
    setFixedSize(knDpi->size(106, 64));
    //Initial the border.
    m_border.lineTo(knDpi->pos(0, 64));
    m_border.lineTo(knDpi->pos(47, 64));
    m_border.cubicTo(knDpi->posF(75, 64),
                     knDpi->posF(75, 0),
                     knDpi->posF(106, 0));
    m_border.lineTo(0, 0);
    //Initialized the mouse time line.
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseInOut->setUpdateInterval(16);
    //Link the time line.
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KNPreferenceHeaderButton::onMouseInOut);

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
    startAnime(100);
    //Do the default enterEvent.
    QAbstractButton::enterEvent(event);
}

void KNPreferenceHeaderButton::leaveEvent(QEvent *event)
{
    //Start mouse out animation.
    startAnime(0);
    //Do the default leaveEvent.
    QAbstractButton::leaveEvent(event);
}

void KNPreferenceHeaderButton::onMouseInOut(int frame)
{
    //Update the close icon opacity.
    m_closeIconOpacity=(qreal)frame/100;
    //Repaint the button.
    update();
}

inline void KNPreferenceHeaderButton::startAnime(int endFrame)
{
    //Stop all the animations.
    m_mouseInOut->stop();
    //Set the initial frame of the time line.
    m_mouseInOut->setFrameRange(m_closeIconOpacity*100, endFrame);
    //Start animation.
    m_mouseInOut->start();
}

QPixmap KNPreferenceHeaderButton::icon() const
{
    return m_icon;
}

void KNPreferenceHeaderButton::setIcon(const QPixmap &icon)
{
    //Save the icon.
    m_icon=icon.scaled(knDpi->size(IconSize, IconSize),
                       Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //Update the painting.
    update();
}
