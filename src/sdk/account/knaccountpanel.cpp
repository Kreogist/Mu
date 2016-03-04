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
//Account widget.
#include "knaccountloginpanel.h"
#include "knaccountregisterpanel.h"
#include "knaccountwaitingpanel.h"
#include "knaccountdetailpanel.h"

#include "knhwidgetswitcher.h"

#include "knaccountpanel.h"

KNAccountPanel::KNAccountPanel(QWidget *parent) :
    QWidget(parent),
    m_switcher(new KNHWidgetSwitcher(this))
{
    //Set property.
    setFixedHeight(270);
    //Login the panel.
    KNAccountLoginPanel *loginPanel=new KNAccountLoginPanel(this);
    //Register panel.
    KNAccountRegisterPanel *generatePanel=new KNAccountRegisterPanel(this);
    //Wating panel.
    KNAccountWaitingPanel *waitingPanel=new KNAccountWaitingPanel(this);
    //Detail panel.
    KNAccountDetailPanel *detailPanel=new KNAccountDetailPanel(this);
    //Add widget to switcher.
    m_switcher->addWidget(loginPanel);
    m_switcher->addWidget(generatePanel);
    m_switcher->addWidget(waitingPanel);
    m_switcher->addWidget(detailPanel);
}

void KNAccountPanel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the switcher,
    m_switcher->resize(size());
}

