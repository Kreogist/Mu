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
#include <QGraphicsDropShadowEffect>
#include <QPainter>

#include "kndpimanager.h"

#include "knnotificationwidget.h"

#define ImageSize 45
#define Spacing 5
#define ContentY 22
#define TitleY 7
#define TextLeft (ImageSize + (Spacing<<1))

KNNotificationWidget::KNNotificationWidget(QWidget *parent) :
    QWidget(parent),
    m_defaultIcon(QPixmap(":/public/notification_default.png")),
    m_pressed(false)
{
    //Set properties.
    setFixedSize(knDpi->size(270, 57));

    //Initial graphics effect.
    QGraphicsDropShadowEffect *effect=new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(20);
    effect->setOffset(0.0);
    effect->setColor(QColor(0,0,0));
    setGraphicsEffect(effect);
}

void KNNotificationWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set color.
    painter.setOpacity(0.8);
    painter.setBrush(Qt::white);
    //Draw the background.
    painter.drawRoundedRect(rect(), 10, 10);
    //Calculate content width.
    int contentWidth=width()-TextLeft-Spacing;
    //Draw the icon.
    painter.setOpacity(1.0);
    painter.drawPixmap(Spacing,
                       Spacing+1,
                       m_currentIcon.isNull()?m_defaultIcon:m_currentIcon);
    //Draw the content.
    painter.setPen(QColor(0x8a, 0x8a, 0x8a));
    painter.drawText(QRect(TextLeft,
                           ContentY,
                           contentWidth,
                           height() - ContentY - Spacing),
                     Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                     fontMetrics().elidedText(m_content,
                                              Qt::ElideRight,
                                              contentWidth<<1));
    //Draw the title.
    painter.setPen(QColor(0x61, 0x61, 0x61));
    painter.drawText(QRect(TextLeft,
                           TitleY,
                           contentWidth,
                           ContentY - TitleY),
                     Qt::AlignLeft | Qt::AlignTop,
                     fontMetrics().elidedText(m_title,
                                              Qt::ElideRight,
                                              contentWidth));
}

void KNNotificationWidget::mousePressEvent(QMouseEvent *event)
{
    //Do original press event.
    QWidget::mousePressEvent(event);
    //Set flag.
    m_pressed=true;
}

void KNNotificationWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //Check flag.
    if(m_pressed)
    {
        //Reset flag.
        m_pressed=false;
        //Emit signal.
        emit requireHideNotification();
    }
    //Do original event.
    QWidget::mouseReleaseEvent(event);
}

QPixmap KNNotificationWidget::currentIcon() const
{
    return m_currentIcon;
}

void KNNotificationWidget::setCurrentIcon(const QPixmap &currentIcon)
{
    //Check current icon first.
    if(currentIcon.isNull())
    {
        //Reset the current icon.
        m_currentIcon=QPixmap();
        //Complete.
        return;
    }
    //Save the icon.
    m_currentIcon = currentIcon;
}

QString KNNotificationWidget::content() const
{
    return m_content;
}

void KNNotificationWidget::setContent(const QString &content)
{
    m_content = content;
}

void KNNotificationWidget::resetPressedFlag()
{
    //Reset the flag.
    m_pressed=false;
}

QString KNNotificationWidget::title() const
{
    return m_title;
}

void KNNotificationWidget::setTitle(const QString &title)
{
    m_title = title;
}
