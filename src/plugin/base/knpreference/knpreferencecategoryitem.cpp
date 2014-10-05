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

#include "knpreferencecategoryitem.h"

KNPreferenceCategoryItem::KNPreferenceCategoryItem(QWidget *parent) :
    KNAnimeCheckedButton(parent)
{
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);
    setMinimumHeight(25);
}

QPixmap KNPreferenceCategoryItem::icon() const
{
    return m_icon;
}

void KNPreferenceCategoryItem::setIcon(const QPixmap &icon)
{
    m_icon=icon;
    update();
}

QString KNPreferenceCategoryItem::text() const
{
    return m_text;
}

void KNPreferenceCategoryItem::setText(const QString &text)
{
    m_text=text;
    update();
}

void KNPreferenceCategoryItem::startMouseInAnime()
{
    ;
}

void KNPreferenceCategoryItem::startMouseOutAnime()
{
    ;
}

void KNPreferenceCategoryItem::startMouseDownAnime()
{
    ;
}

void KNPreferenceCategoryItem::startMouseUpAnime()
{
    ;
}

void KNPreferenceCategoryItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw the select background with the opacity.
    painter.setOpacity(m_backgroundOpacity);
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().window());
    painter.drawRect(rect());
    //Draw the text and the icon to the position.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(0,0,m_text);
    painter.drawText(0,0,"laksdjf;laskdjf;laksdjf");
}
