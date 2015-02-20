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
#include <QScrollBar>
#include <QLabel>
#include <QListView>

#include "knmusiclyricsitemdelegate.h"
#include "knmusiclyricsmanager.h"
#include "knmusicglobal.h"

#include "knglobal.h"

#include "knmusiclyricslistview.h"

KNMusicLyricsListView::KNMusicLyricsListView(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedHeight(300);
    //Initial the lyrics manager.
    m_lyricsManager=KNMusicGlobal::instance()->lyricsManager();
    //Initial the list widget.
    initialLyricsList();

    //Set the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QBoxLayout *lyricsLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                            mainLayout->widget());
    lyricsLayout->setContentsMargins(0,0,0,0);
    lyricsLayout->setSpacing(0);
    lyricsLayout->addWidget(m_lyricsList);
    mainLayout->addLayout(lyricsLayout);

    //Retranslate.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicLyricsListView::retranslate);
    retranslate();
}

KNMusicLyricsListView::~KNMusicLyricsListView()
{
    ;
}

void KNMusicLyricsListView::setLyricsModel(QAbstractItemModel *model)
{
    m_lyricsList->setModel(model);
}

void KNMusicLyricsListView::retranslate()
{
    ;
}

inline void KNMusicLyricsListView::initialLyricsList()
{
    //Initial the lyrics list.
    m_lyricsList=new QListView(this);
    //Set properties.
    m_lyricsList->setAutoFillBackground(true);
    m_lyricsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    //Hack: for the scroll area lines.
    //A scroll area is worked like this:
    /* +-----+-+
     * |     | |
     * +-----+-+ <- Here is the line, we should hide it.
     * +-----+-+
     */
    //How to hide it? Use FIXED height to force not to draw the last pixel of
    //the list view.
    m_lyricsList->setFixedHeight(301);
    //Set palette.
    QPalette pal=m_lyricsList->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    m_lyricsList->setPalette(pal);
    //Set the item delegate.
    m_lyricsList->setItemDelegate(new KNMusicLyricsItemDelegate);
    //Customized scroll bar.
    QScrollBar *listVScrollBar=new QScrollBar(m_lyricsList);
    listVScrollBar->setStyleSheet(
                "QScrollBar:vertical {"
                "   border: 0px solid grey;"
                "   background: rgba(0, 0, 0, 0);"
                "   width: 8px;"
                "}"
                "QScrollBar::handle:vertical {"
                "   background: rgba(0, 0, 0, 100);"
                "   min-height: 10px;"
                "   border-radius: 4px;"
                "}"
                "QScrollBar::add-line:vertical {"
                "   border: 0px solid grey;"
                "   background: rgba(0, 0, 0, 100);"
                "   height: 0px;"
                "   subcontrol-position: down;"
                "   subcontrol-origin: margin;"
                "}"
                "QScrollBar::sub-line:vertical {"
                "   border: 0px solid grey;"
                "   background: rgba(0, 0, 0, 100);"
                "   height: 0px;"
                "   subcontrol-position: up;"
                "   subcontrol-origin: margin;"
                "}"
                );
    m_lyricsList->setVerticalScrollBar(listVScrollBar);

    //Link the lyrics list.
    connect(m_lyricsList, &QListView::activated,
            this, &KNMusicLyricsListView::lyricsActivate);
}
