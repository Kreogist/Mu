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
#include <QBoxLayout>
#include <QLabel>

#include "knthememanager.h"
#include "kndarkwaitingwheel.h"

#include "knmusicstoreemptywidget.h"

KNMusicStoreEmptyWidget::KNMusicStoreEmptyWidget(QWidget *parent) :
    QWidget(parent),
    m_waitingWheel(new KNDarkWaitingWheel(this)),
    m_title(new QLabel(this))
{
    setObjectName("MusicStoreEmptyWidget");
    //Set properties.
    knTheme->registerWidget(this);
    //Configure the title label.
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(41);
    m_title->setFont(titleFont);
    m_title->setText(QChar(0x03BC)+QString("'sic Store"));

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_waitingWheel, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_title);
    mainLayout->addStretch();
}

void KNMusicStoreEmptyWidget::showEvent(QShowEvent *event)
{
    //Show the widget.
    QWidget::showEvent(event);
    //Show the widget.
    m_waitingWheel->startTick();
}

void KNMusicStoreEmptyWidget::hideEvent(QHideEvent *event)
{
    //Hide the widget.
    QWidget::hideEvent(event);
    //Stop the waiting wheel.
    m_waitingWheel->stopTick();
}
