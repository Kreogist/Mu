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

#include "knvwidgetswitcher.h"

#include "knpreferencecontents.h"

#include <QDebug>

KNPreferenceContents::KNPreferenceContents(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(30,30,30));
    setPalette(pal);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Initial the panel swithcer.
    m_panelSwitcher=new KNVWidgetSwitcher(this);
    mainLayout->addWidget(m_panelSwitcher);
}

void KNPreferenceContents::addPanelWidget(QWidget *panel)
{
    //Simply add widget to switcher.
    m_panelSwitcher->addWidget(panel);
}

void KNPreferenceContents::setCurrentIndex(const int &index)
{
    //Change the current index.
    m_panelSwitcher->setCurrentIndex(index);
}
