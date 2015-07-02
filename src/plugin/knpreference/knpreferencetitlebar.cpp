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
#include <QLabel>

#include "knglobal.h"
#include "knthememanager.h"
#include "knpreferenceheaderbutton.h"

#include "knpreferencetitlebar.h"

KNPreferenceTitleBar::KNPreferenceTitleBar(QWidget *parent) :
    QWidget(parent),
    m_button(new KNPreferenceHeaderButton(this)),
    m_title(new QLabel(this)),
    m_dullPolishBrush(knGlobal->textureBursh(KNGlobal::DullPolish)),
    m_highlightGradient(QLinearGradient(QPointF(0,0), QPointF(0,40)))
{
    setObjectName("PreferenceTitleBar");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(69);
    //Configure the highlight gradient.
    m_highlightGradient.setColorAt(0, QColor(255,255,255,40));
    m_highlightGradient.setColorAt(1, QColor(255,255,255,0));

    //Configure the title label.
    m_title->setObjectName("PreferenceTitleBarLabel");
    m_title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_title->move(93, 0);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(20);
    m_title->setFont(titleFont);

    //Register the widget to theme manager.
    knTheme->registerWidget(this);
    knTheme->registerWidget(m_title);

    //Link the header button.
    connect(m_button, SIGNAL(clicked(bool)),
            this, SIGNAL(requireClosePreference()));
}

void KNPreferenceTitleBar::setIcon(const QPixmap &icon)
{
    //Set the title button icon.
    m_button->setIcon(icon);
}

void KNPreferenceTitleBar::setText(const QString &text)
{
    //Set the title text.
    m_title->setText(text);
}

void KNPreferenceTitleBar::paintEvent(QPaintEvent *event)
{
    //Do orignial paint.
    QWidget::paintEvent(event);
    //Generate the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);

    //Draw the texture.
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_dullPolishBrush);
    painter.drawRect(rect());
    //Draw the high light.
    painter.setBrush(m_highlightGradient);
    painter.drawRect(QRect(0,0,width(),64));
    //Draw the bottom border.
    int lineY=height();
    painter.setPen(QColor(0x2b, 0x2b, 0x2b));
    painter.drawLine(QPointF(0, lineY), QPointF(width(), lineY));
    lineY--;
    painter.setPen(QColor(0x16, 0x16, 0x16));
    painter.drawLine(QPointF(0, lineY), QPointF(width(), lineY));
    lineY--;
    painter.setPen(QColor(0x08, 0x08, 0x08));
    painter.drawLine(QPointF(0, lineY), QPointF(width(), lineY));
}
