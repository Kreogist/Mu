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
#include <QAction>
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

    //Initial the shortcuts.
    initialShortcuts();
}

int KNCategoryTabWidget::addTab(const QPixmap &icon,
                                const QString &caption,
                                QWidget *widget)
{
    //Add widget first, because tab bar will emit signal to switch widget.
    m_widgetSwitcher->addWidget(widget);
    m_tabBar->addCategory(icon, caption);
    //Return the current widget index.
    return m_widgetSwitcher->count()-1;
}

int KNCategoryTabWidget::currentIndex() const
{
    return m_tabBar->currentIndex();
}

int KNCategoryTabWidget::categorySize() const
{
    return m_tabBar->categorySize();
}

void KNCategoryTabWidget::setTabText(const int &index,
                                     const QString &caption)
{
    m_tabBar->setCategoryText(index, caption);
}

void KNCategoryTabWidget::setCurrentIndex(int index)
{
    m_tabBar->setCurrentIndex(index);
}

void KNCategoryTabWidget::moveLeft()
{
    setCurrentIndex(currentIndex()==0?categorySize()-1:currentIndex()-1);
}

void KNCategoryTabWidget::moveRight()
{
    setCurrentIndex(currentIndex()==categorySize()-1?0:currentIndex()+1);
}

void KNCategoryTabWidget::initialShortcuts()
{
    //Move to left tab.
    QAction *moveToLeftTab=new QAction(this);
    moveToLeftTab->setShortcut(Qt::CTRL+Qt::Key_Left);
    moveToLeftTab->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(moveToLeftTab, SIGNAL(triggered()),
            this, SLOT(moveLeft()));
    addAction(moveToLeftTab);

    //Move to right tab.
    QAction *moveToRightTab=new QAction(this);
    moveToRightTab->setShortcut(Qt::CTRL+Qt::Key_Right);
    moveToRightTab->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(moveToRightTab, SIGNAL(triggered()),
            this, SLOT(moveRight()));
    addAction(moveToRightTab);
}
