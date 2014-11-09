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

#include "knabstractcategorybutton.h"

KNAbstractCategoryButton::KNAbstractCategoryButton(QWidget *parent) :
    KNAnimeCheckedButton(parent)
{
    //Set font.
    QFont captionFont=font();
    captionFont.setPixelSize(20);
    setFont(captionFont);
    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);
}

QSize KNAbstractCategoryButton::sizeHint() const
{
    return QSize(m_iconSize + fontMetrics().width(m_text) + (m_margin<<1) + m_spacing,
                 m_iconSize + (m_margin<<1));
}

QPixmap KNAbstractCategoryButton::icon() const
{
    return m_icon;
}

void KNAbstractCategoryButton::setIcon(const QPixmap &icon)
{
    //Scale the icon.
    m_icon=icon.scaled(m_iconSize,
                       m_iconSize,
                       Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
}

QString KNAbstractCategoryButton::text() const
{
    return m_text;
}

void KNAbstractCategoryButton::setText(const QString &text)
{
    m_text=text;
}

void KNAbstractCategoryButton::paintEvent(QPaintEvent *event)
{
    KNAbstractButton::paintEvent(event);
    //Initial antialiasing painter.
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setPen(palette().color(QPalette::WindowText));
    //Paint the icon.
    painter.drawPixmap(m_margin,
                       m_margin,
                       m_icon);
    //Paint the text.
    painter.drawText(m_margin+m_iconSize+m_spacing,
                     m_margin,
                     width()-(m_margin<<1)-m_iconSize-m_spacing,
                     height()-(m_margin<<1),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_text);
}
