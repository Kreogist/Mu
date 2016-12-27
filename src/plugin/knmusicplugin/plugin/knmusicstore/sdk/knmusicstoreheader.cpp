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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>
#include <QTimeLine>
#include <QPainter>
#include <QSignalMapper>

#include "knanimelabelbutton.h"
#include "knlocalemanager.h"
#include "knthememanager.h"

#include "knmusicstoreglobal.h"
#include "knmusicstoreutil.h"

#include "knmusicstoreheader.h"

#include <QDebug>

#define LayoutItemSpacing   7
#define LayoutBorderSpacing 21
#define MaxNavigatorWidth   150

KNMusicStoreHeader::KNMusicStoreHeader(QWidget *parent) :
    KNMouseSenseWidget(parent),
    m_borderGradient(QLinearGradient(0, 0,
                                     0, KNMusicStoreUtil::headerHeight())),
    m_searchText(QString()),
    m_searchKeyword(QString()),
    m_mouseAnime(new QTimeLine(200, this)),
    m_mainLayout(new QBoxLayout(QBoxLayout::LeftToRight, this)),
    m_pluginTray(new QBoxLayout(QBoxLayout::RightToLeft))
{
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);
    setFixedHeight(KNMusicStoreUtil::headerHeight());
    //Initial navigator mapper.
    QSignalMapper *navigatorMapper=new QSignalMapper(this);
    connect(navigatorMapper, SIGNAL(mapped(int)),
            this, SLOT(onNavigatorClick(int)));
    //Initial navigators.
    for(int i=0; i<PagesCount; ++i)
    {
        //Initial the label text.
        m_navigatorItem[i]=new KNAnimeLabelButton(this);
        //Set the properties.
        m_navigatorItem[i]->setCursor(Qt::PointingHandCursor);
        //Hide the navigator item.
        m_navigatorItem[i]->hide();
        //Add the item to mapper.
        connect(m_navigatorItem[i], SIGNAL(clicked()),
                navigatorMapper, SLOT(map()));
        navigatorMapper->setMapping(m_navigatorItem[i], i);
    }
    //Show the home page item.
    m_navigatorItem[PageHome]->show();
    //Configure the tray layout.
    m_pluginTray->setContentsMargins(0, 0, 0, 0);
    m_pluginTray->setSpacing(LayoutItemSpacing);

    //Configure the main layout.
    m_mainLayout->setContentsMargins(LayoutBorderSpacing,
                                     0,
                                     LayoutBorderSpacing,
                                     0);
    m_mainLayout->setSpacing(LayoutItemSpacing);
    setLayout(m_mainLayout);
    m_mainLayout->addSpacing(LayoutBorderSpacing);
    //Add widget to layout.
    for(int i=0; i<PagesCount; ++i)
    {
        //Add the label.
        m_mainLayout->addWidget(m_navigatorItem[i]);
        //Add navigator.
        if(i!=PagesCount-1)
        {
            //Create the indicator of the label.
            m_indicator[i]=new QLabel(this);
            //Hide the indicator.
            m_indicator[i]->hide();
            //Set the pixmap.
            m_indicator[i]->setPixmap(QPixmap("://public/bullet.png"));
            //Add to layout.
            m_mainLayout->addWidget(m_indicator[i]);
        }
    }
    //Add stretch.
    m_mainLayout->addStretch();
    //Add the connection state widget.
//    m_mainLayout->addWidget(knMusicStoreGlobal->connectStateWheel());
    //Add tray layout.
    m_mainLayout->addLayout(m_pluginTray);

    //Link the retranslator.
    knI18n->link(this, &KNMusicStoreHeader::retranslate);
    retranslate();
}

void KNMusicStoreHeader::addHeaderWidget(QWidget *widget,
                                         int stretch,
                                         Qt::Alignment alignment)
{
    //Add widget to the header.
    m_mainLayout->addWidget(widget, stretch, alignment);
}

void KNMusicStoreHeader::addStateWidget(QWidget *widget)
{
    //Add widget to the icon layout.
    m_pluginTray->addWidget(widget);
}

void KNMusicStoreHeader::setNavigatorText(int itemIndex,
                                          const QString &text)
{
    bool elided=false;
    //Show the changed text.
    m_navigatorItem[itemIndex]->show();
    //Update the text for elided text.
    QString elidedText=
            m_navigatorItem[itemIndex]->fontMetrics().elidedText(
                text, Qt::ElideRight, MaxNavigatorWidth);
    //Check elided text length.
    if(elidedText.length()<text.length())
    {
        //Set the tooltip flag to be true.
        elided=true;
    }
    //Check whether the item has indicator.
    if(itemIndex!=0)
    {
        //Show the indicator.
        m_indicator[itemIndex-1]->show();
    }
    //Hide all the labels after the current index.
    for(int i=itemIndex+1; i<PagesCount; ++i)
    {
        //Hide the navigator item.
        m_navigatorItem[i]->hide();
        //Hide the indicator.
        m_indicator[i-1]->hide();
    }
    //Check the item index.
    switch(itemIndex)
    {
    case PageAlbum:
    case PageSingleSong:
        //Check the result.
        if(elided)
        {
            //Set the elided text as the text.
            m_navigatorItem[itemIndex]->setText(elidedText);
            //Set the original text as tooltip.
            m_navigatorItem[itemIndex]->setToolTip(text);
        }
        else
        {
            //Set the original text as the text.
            m_navigatorItem[itemIndex]->setText(text);
            //Clear the elided text.
            m_navigatorItem[itemIndex]->setToolTip("");
        }
        break;
    case PageSearchResult:
        //Update the keyword.
        m_searchKeyword=text;
        //Update the search label text.
        updateSearchLabel();
        break;
    default:
        break;
    }
}

void KNMusicStoreHeader::paintEvent(QPaintEvent *event)
{
    //Paint the raw data.
    QWidget::paintEvent(event);
    //Prepare the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the border line.
    painter.fillRect(0, 0, 1, height(), m_borderGradient);
    painter.fillRect(width()-1, 0, 1, height(), m_borderGradient);
}

void KNMusicStoreHeader::onPaletteChanged()
{
    //Do the original palette changed.
    KNMouseSenseWidget::onPaletteChanged();
    //Save the new color of window.
    QColor borderColor=palette().color(QPalette::Window);
    borderColor.setAlpha(senseRangeStart());
    m_borderGradient.setColorAt(0, borderColor);
    borderColor.setAlpha(senseRangeEnd());
    m_borderGradient.setColorAt(1, borderColor);
    //Update all the navigator item palette.
    for(int i=0; i<PagesCount; ++i)
    {
        m_navigatorItem[i]->updateObjectName("MusicStoreNavigator");
    }
}

void KNMusicStoreHeader::retranslate()
{
    //Update the string text.
    m_searchText=tr("Search Result for '%1'");
    //Update the label.
    m_navigatorItem[PageHome]->setText(tr("Store"));
    //Update the search label text.
    updateSearchLabel();
}

void KNMusicStoreHeader::onNavigatorClick(int index)
{
    //Emit the page display requirement.
    emit requireShowPage(index);
}

inline void KNMusicStoreHeader::updateSearchLabel()
{
    //Update to the latest search text.
    m_navigatorItem[PageSearchResult]->setText(m_searchText.arg(
                                                   m_searchKeyword));
}
