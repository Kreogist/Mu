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

//Dependence
#include "kncategorytab.h"
#include "kncategorytabbar.h"
#include "knhwidgetswitcher.h"
#include "knmousedetectheader.h"

//SDK Dependence.
#include "knmusicdetaildialog.h"
#include "knmusicparser.h"

//Ports
#include "knmusicdetaildialogpanel.h"
#include "knmusictagparser.h"

//Plugins
// Detail Dialog Panels.
#include "plugin/knmusicdetailpaneloverview/knmusicdetailpaneloverview.h"
// Tag Parsers.
#include "plugin/knmusictagid3v1/knmusictagid3v1.h"
#include "plugin/knmusictagid3v2/knmusictagid3v2.h"

//Globals.
#include "knmusicglobal.h"

#include "knmusicplugin.h"

#include <QDebug>
#include "knanimationmenu.h"

KNMusicPlugin::KNMusicPlugin(QWidget *parent) :
    KNAbstractMusicPlugin(parent),
    m_headerWidget(new KNMouseDetectHeader(this)),
    m_headerLeftLayout(nullptr),
    m_headerRightLayout(nullptr),
    m_tabBar(new KNCategoryTabBar(this)),
    m_switcher(new KNHWidgetSwitcher(this))
{
    //Initial the basic infrastructure.
    initialInfrastructure();
    //Initial the detail dialog plugins.
    initialDetailDialogPanel();
    //Initial parser.
    initialParserPlugin();
    //Initial global menus.
    KNAnimationMenu *menu=new KNAnimationMenu(this);
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addSeparator();
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addSeparator();
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->addAction("Test 1");
    menu->setMouseDownPos(QPoint(20,20));
    menu->exec();

    //Initial the .

    //Start working threads.
    knMusicGlobal->startThreads();
}

QWidget *KNMusicPlugin::headerWidget()
{
    return m_headerWidget;
}

QPixmap KNMusicPlugin::icon()
{
    return QPixmap(":/plugin/music/public/icon.png");
}

QString KNMusicPlugin::title()
{
    return tr("Music");
}

void KNMusicPlugin::saveConfigure()
{
    ;
}

void KNMusicPlugin::onArgumentsAvailable(const QStringList &data)
{
    ;
}

void KNMusicPlugin::initialInfrastructure()
{
    //Initial the music global.
    KNMusicGlobal::initial(this);

    //Configure the header widget.
    m_headerWidget->setContentsMargins(0,0,0,0);
    //Set header layout.
    QBoxLayout *headerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_headerWidget);
    headerLayout->setContentsMargins(0,0,10,0);
    headerLayout->setSpacing(0);
    m_headerWidget->setLayout(headerLayout);
    //Initial left layout.
    m_headerLeftLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      headerLayout->widget());
    m_headerLeftLayout->setContentsMargins(0,0,0,0);
    m_headerLeftLayout->setSpacing(0);
    headerLayout->addLayout(m_headerLeftLayout, 1);
    //Initial right layout.
    m_headerRightLayout=new QBoxLayout(QBoxLayout::RightToLeft,
                                       headerLayout->widget());
    m_headerRightLayout->setContentsMargins(10,0,0,0);
    m_headerRightLayout->setSpacing(0);
    headerLayout->addLayout(m_headerRightLayout);

    //Link the tab bar and switcher.
    connect(m_tabBar, &KNCategoryTabBar::currentIndexChange,
            m_switcher, &KNHWidgetSwitcher::setCurrentIndex);

    //Initial layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //Add widget to layout.
    mainLayout->addWidget(m_tabBar);
    mainLayout->addWidget(m_switcher, 1);
}

void KNMusicPlugin::initialDetailDialogPanel()
{
    //Add panels to detail dialog.
    knMusicGlobal->detailDialog()->addPanel(new KNMusicDetailPanelOverview);
}

void KNMusicPlugin::initialParserPlugin()
{
    //Get the music parser.
    KNMusicParser *parser=knMusicGlobal->parser();

    //Add tag parsers.
    parser->installTagParser(new KNMusicTagId3v1);
    parser->installTagParser(new KNMusicTagId3v2);
}
