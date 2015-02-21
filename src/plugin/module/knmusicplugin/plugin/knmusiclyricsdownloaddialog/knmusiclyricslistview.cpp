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

#include "knroundbuttonbar.h"
#include "knhwidgetswitcher.h"
#include "sao/knsaostyle.h"

#include "knmusiclyricsitemdelegate.h"
#include "knmusicglobal.h"
#include "knmusicparser.h"

#include "knglobal.h"

#include "knmusiclyricslistview.h"

KNMusicLyricsListView::KNMusicLyricsListView(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setFixedHeight(300);
    //Initial the list widget.
    initialLyricsList();
    //Initial the lyrics preview widgets.
    initialLyricsPreviewMode();

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

    QBoxLayout *previewLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             mainLayout->widget());
    previewLayout->setContentsMargins(0,0,0,0);
    previewLayout->setSpacing(5);
    previewLayout->addWidget(m_previewModeBar);
    previewLayout->addWidget(m_previewWidgets, 1);
    mainLayout->addLayout(previewLayout);

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
    //Link the lyrics list.
    connect(m_lyricsList->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &KNMusicLyricsListView::currentLyricsChanged);
}

QModelIndex KNMusicLyricsListView::currentLyricsIndex() const
{
    return m_lyricsList->currentIndex();
}

void KNMusicLyricsListView::addPreviewer(const QPixmap &icon,
                                         QWidget *widget)
{
    //Add the preview widget first.
    m_previewWidgets->addWidget(widget);
    //Add the icon button.
    m_previewModeBar->addButton(icon);
}

void KNMusicLyricsListView::retranslate()
{
    ;
}

void KNMusicLyricsListView::currentLyricsChanged(const QModelIndex &current,
                                                 const QModelIndex &previous)
{
    Q_UNUSED(previous)
    emit lyricsActivate(current);
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
     * | XXX | |
     * +-----+=+ <- Here is the line (Marked '='), we should hide it.
     * +-----+-+
     */
    //How to hide it? Use FIXED height to force not to draw the last pixel of
    //the list view.
    //Using the setFixedHeight() to force the list is 1px higher than the total
    //height.
    m_lyricsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    KNSAOStyle::setScrollBarStyleSheet(m_lyricsList->verticalScrollBar());
}

inline void KNMusicLyricsListView::initialLyricsPreviewMode()
{
    //Initial the preview mode bar.
    m_previewModeBar=new KNRoundButtonBar(this);
    //Initial the horizontal widget switcher.
    m_previewWidgets=new KNHWidgetSwitcher(this);
    //Link the preview mode bar with the widget switcher.
    connect(m_previewModeBar, &KNRoundButtonBar::currentIndexChanged,
            m_previewWidgets, &KNHWidgetSwitcher::setCurrentIndex);
}
