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

#include "knpreferencetitlebar.h"
#include "knsideshadowwidget.h"

#include "knpreferencesidebar.h"

KNPreferenceSidebar::KNPreferenceSidebar(QWidget *parent) :
    QWidget(parent),
    m_titleBar(new KNPreferenceTitleBar(this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_shadowWidth(15)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add the widget.
    mainLayout->addWidget(m_titleBar);
    //Add the widget.
    mainLayout->addStretch();

    //Link requests.
    connect(m_titleBar, &KNPreferenceTitleBar::requireClosePreference,
            this, &KNPreferenceSidebar::requireClosePreference);
}

void KNPreferenceSidebar::resizeEvent(QResizeEvent *event)
{
    //Do the original resize.
    QWidget::resizeEvent(event);
    //Move the shadow.
    m_rightShadow->setGeometry(width()-m_shadowWidth,
                               0,
                               m_shadowWidth,
                               height());
}

