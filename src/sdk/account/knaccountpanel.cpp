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
//
#include "knaccount.h"
#include "knaccountdetails.h"

//Account widget.
#include "knaccountloginpanel.h"
#include "knaccountregisterpanel.h"
#include "knaccountwaitingpanel.h"
#include "knaccountdetailpanel.h"

#include "knhwidgetswitcher.h"

#include "knaccountpanel.h"

KNAccountPanel::KNAccountPanel(QWidget *parent) :
    QWidget(parent),
    m_switcher(new KNHWidgetSwitcher(this)),
    m_loginPanel(new KNAccountLoginPanel(this)),
    m_generatePanel(new KNAccountRegisterPanel(this)),
    m_waitingPanel(new KNAccountWaitingPanel(this)),
    m_detailPanel(new KNAccountDetailPanel(this))
{
    //Set property.
    setFixedHeight(270);
    //Link panel signal.
    connect(m_loginPanel, &KNAccountLoginPanel::requireRegister,
            this, &KNAccountPanel::onActionShowRegister);
    //Add widget to switcher.
    m_switcher->addWidget(m_loginPanel);
    m_switcher->addWidget(m_generatePanel);
    m_switcher->addWidget(m_waitingPanel);
    m_switcher->addWidget(m_detailPanel);
}

void KNAccountPanel::showEvent(QShowEvent *event)
{
    //Check whether account is login.
    if(m_switcher->currentIndex()==RegisterPanel)
    {
        //Reset the switcher to
        m_switcher->setCurrentIndex(LoginPanel);
    }
    //Show the panel.
    QWidget::showEvent(event);
}

void KNAccountPanel::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the switcher,
    m_switcher->resize(size());
}

void KNAccountPanel::onActionShowRegister()
{
    //Switch the widget selector to register panel.
    m_switcher->setCurrentIndex(RegisterPanel);
}

