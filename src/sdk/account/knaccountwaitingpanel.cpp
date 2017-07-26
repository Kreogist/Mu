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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>

#include "knclockwheel.h"
#include "kndpimanager.h"
#include "knlocalemanager.h"

#include "knaccountwaitingpanel.h"

KNAccountWaitingPanel::KNAccountWaitingPanel(QWidget *parent) :
    QWidget(parent),
    m_waitingWheel(new KNClockWheel(this)),
    m_waitingHint(new QLabel(this))
{
    //Set property.
    setFixedHeight(knDpi->height(270));
    //Configure the wheel.
    m_waitingWheel->setFixedSize(knDpi->size(80, 80));
    //Configure the hint text.
    QPalette pal=m_waitingHint->palette();
    pal.setColor(QPalette::WindowText, QColor(157, 157, 157));
    m_waitingHint->setPalette(pal);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addStretch();
    mainLayout->addWidget(m_waitingWheel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(knDpi->height(8));
    mainLayout->addWidget(m_waitingHint, 0, Qt::AlignHCenter);
    mainLayout->addStretch();

    //Link the request.
    knI18n->link(this, &KNAccountWaitingPanel::retranslate);
    retranslate();
}

void KNAccountWaitingPanel::startTicking()
{
    //Start to tick.
    m_waitingWheel->startTick();
}

void KNAccountWaitingPanel::stopTicking()
{
    //Stop tick.
    m_waitingWheel->stopTick();
}

void KNAccountWaitingPanel::retranslate()
{
    //Update the waiting hint.
    m_waitingHint->setText(tr("Please wait"));
}
