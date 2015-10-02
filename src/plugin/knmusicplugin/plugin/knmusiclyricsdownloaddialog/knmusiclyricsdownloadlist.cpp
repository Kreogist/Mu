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

#include "knclockwheel.h"

#include "knmusiclyricsdownloadlist.h"

#include <QDebug>

KNMusicLyricsDownloadList::KNMusicLyricsDownloadList(QWidget *parent) :
    QWidget(parent),
    m_container(new QWidget(this)),
    m_clockWheel(new KNClockWheel(this))
{
    //Configure the clock wheel.
    m_clockWheel->setFixedSize(45, 45);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_container);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    m_container->setLayout(mainLayout);
    //Add the widget to main layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_clockWheel);
    mainLayout->addStretch();
}

void KNMusicLyricsDownloadList::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the container.
    m_container->resize(size());
}

