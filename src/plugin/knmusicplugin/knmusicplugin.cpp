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

//Debug
#include <QFile>
#include <QDataStream>
#include <QDebug>

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

    //Initial the .
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
    //Debug.
    KNMusicTagParser *parser=new KNMusicTagId3v2(this);
    KNMusicAnalysisItem item;
    QFile fuck("/Users/Saki/Desktop/test.mp3");
    item.detailInfo.filePath="/Users/Saki/Desktop/test.mp3";
//    QFile fuck("/Users/Saki/Music/初音未来 - 夏祭り.mp3");
//    item.detailInfo.filePath="/Users/Saki/Music/初音未来 - 夏祭り.mp3";
    qDebug()<<"Open result: "<<fuck.open(QIODevice::ReadOnly);
    QDataStream datastream(&fuck);
    qDebug()<<parser->parseTag(fuck, datastream, item);
    qDebug()<<parser->parseAlbumArt(item);
    fuck.close();

    KNMusicDetailInfo &di=item.detailInfo;
//    item.coverImage.save("/Users/Saki/Desktop/屏幕快照 2015-07-14 下午11.35.29.png", "PNG");
//    qDebug()<<di.textLists[Name];
//    qDebug()<<di.textLists[Artist];
//    qDebug()<<di.textLists[Album];
//    qDebug()<<di.textLists[TrackNumber];
//    qDebug()<<di.textLists[Genre];
    di.textLists[Name]="Cutie Panther";
    di.textLists[Artist]="Mu's";
//    di.textLists[Album]="Solo Love Live Best Album Collection II - Mu's Best Live CollectionLove Live Best Album Collection II - Mu's Best Live Collection";
    di.textLists[TrackNumber]="12";
    di.textLists[Genre]="Anime";
    qDebug()<<parser->writeTag(item);

    //Add parsers.
    knMusicGlobal->parser()->installTagParser(new KNMusicTagId3v1);
    knMusicGlobal->parser()->installTagParser(new KNMusicTagId3v2);
}
