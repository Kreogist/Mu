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
#include "knmusicdetaildialogpanel.h"
#include "knhtabgroup.h"
#include "knhwidgetswitcher.h"

#include "knmusicdetaildialog.h"

#include <QDebug>

KNMusicDetailDialog::KNMusicDetailDialog(QWidget *parent) :
    KNMessageBox(parent),
    m_panelList(QLinkedList<KNMusicDetailDialogPanel *>()),
    m_panelSwitcher(new KNHTabGroup(this)),
    m_panelContainer(new KNHWidgetSwitcher(this))
{
    //Set properties.
    setTitleText("Information");
    //Configure the panel switcher.
    m_panelSwitcher->setFixedHeight(25);

    //Link the switcher and container.
    connect(m_panelSwitcher, &KNHTabGroup::currentIndexChange,
            m_panelContainer, &KNHWidgetSwitcher::setCurrentIndex);

    //Initial the widget container.
    QWidget *widgetContainer=new QWidget(this);
    widgetContainer->setFixedSize(460, 400);
    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          widgetContainer);
    mainLayout->setContentsMargins(17,12,17,12);
    mainLayout->setSpacing(9);
    widgetContainer->setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_panelSwitcher);
    mainLayout->addWidget(m_panelContainer, 1);
    //Set the detail widget container as the content.
    setContentWidget(widgetContainer);
}

void KNMusicDetailDialog::addPanel(KNMusicDetailDialogPanel *panel)
{
    //Add panel to the panel container.
    m_panelContainer->addWidget(panel);
    //Add the switcher button to switcher.
    m_panelSwitcher->addTab(panel->tabButton());
}

void KNMusicDetailDialog::showDialog(const QString &filePath,
                                     const QString &indexFilePath,
                                     const int &index)
{
    //Ask all the panel to update the information.
    for(QLinkedList<KNMusicDetailDialogPanel *>::iterator i=m_panelList.begin();
        i!=m_panelList.end();
        ++i)
    {
        //Update the panel information.
        (*i)->setFilePath(filePath, indexFilePath, index);
    }
    //Show the dialog.
    exec();
}

