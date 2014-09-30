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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QBoxLayout>

#include "kncategorytabbar.h"
#include "knhwidgetswitcher.h"

#include "kncategorytabwidget.h"

KNCategoryTabWidget::KNCategoryTabWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Set layout.
    m_tabLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_tabLayout->setContentsMargins(0,0,0,0);
    m_tabLayout->setSpacing(0);
    setLayout(m_tabLayout);

    //Initial tab bar.
    m_tabBar=new KNCategoryTabBar(this);
    m_tabLayout->addWidget(m_tabBar);

    //Initial the widget switcher.
    m_widgetSwitcher=new KNHWidgetSwitcher(this);
    connect(m_tabBar, &KNCategoryTabBar::currentIndexChanged,
            m_widgetSwitcher, &KNHWidgetSwitcher::setCurrentIndex);
    connect(m_widgetSwitcher, &KNHWidgetSwitcher::movedComplete,
            m_tabBar, &KNCategoryTabBar::unlockBar);
    m_tabLayout->addWidget(m_widgetSwitcher, 1);
}

void KNCategoryTabWidget::addTab(const QPixmap &icon,
                                 const QString &caption,
                                 QWidget *widget)
{
    //Add widget first, because tab bar will emit signal to switch widget.
    m_widgetSwitcher->addWidget(widget);
    m_tabBar->addCategory(icon, caption);
}
