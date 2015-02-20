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
#include <QPushButton>
#include <QLabel>

#include "kniconframelineedit.h"
#include "knclockwheel.h"
#include "knemptystatewidget.h"

#include "knmusiclyricslistview.h"

#include "knmusiclyricsdownloadwidget.h"

KNMusicLyricsDownloadWidget::KNMusicLyricsDownloadWidget(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setContentsMargins(15,15,15,15);

    //Initial the title caption.
    m_title=generateLineEdit(QPixmap(":/plugin/music/lyrics/title.png"));
    m_artist=generateLineEdit(QPixmap(":/plugin/music/lyrics/artist.png"));

    //Initial the search button.
    m_searchLyrics=new QPushButton(this);
    m_searchLyrics->setDefault(true);
    QPalette buttonPalette=m_searchLyrics->palette();
    buttonPalette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_searchLyrics->setPalette(buttonPalette);
    m_searchLyrics->setText("Search");
    connect(m_searchLyrics, SIGNAL(clicked()),
            this, SIGNAL(requireSearchLyrics()));

    //Initial the server list.
    initialListView();

    //Establish tab order.
    m_title->setDefaultEscFocusTo(m_artist);
    m_artist->setDefaultEscFocusTo(m_searchLyrics);
    setTabOrder(m_searchLyrics, m_title);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    QBoxLayout *searchRequestLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                   mainLayout->widget());
    searchRequestLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(searchRequestLayout);

    QBoxLayout *textEditLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                              mainLayout->widget());
    textEditLayout->addWidget(m_title);
    textEditLayout->addWidget(m_artist);
    searchRequestLayout->addLayout(textEditLayout, 1);

    searchRequestLayout->addWidget(m_searchLyrics);

    mainLayout->addWidget(m_lyricsList);
}

KNMusicLyricsDownloadWidget::~KNMusicLyricsDownloadWidget()
{
    ;
}

void KNMusicLyricsDownloadWidget::setLyricsModel(QAbstractItemModel *model)
{
    m_lyricsListView->setLyricsModel(model);
}

QString KNMusicLyricsDownloadWidget::title() const
{
    return m_title->text();
}

QString KNMusicLyricsDownloadWidget::artist() const
{
    return m_artist->text();
}

void KNMusicLyricsDownloadWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

void KNMusicLyricsDownloadWidget::setArtist(const QString &artist)
{
    m_artist->setText(artist);
}

void KNMusicLyricsDownloadWidget::showLoadingWheel()
{
    //Stop and start ticking the clock wheel.
    m_loadingWheel->startTick();
    //Switch to loading wheel.
    m_lyricsList->showEmptyWidget();
}

void KNMusicLyricsDownloadWidget::showLyricsList()
{
    //Stop loading wheel.
    m_loadingWheel->stopTick();
    //Switch to content wheel.
    m_lyricsList->showContentWidget();
}

inline void KNMusicLyricsDownloadWidget::initialListView()
{
    //Initial lyrics list widget.
    m_lyricsList=new KNEmptyStateWidget(this);

    //Initial the loading wheel and lyrics list.
    QWidget *clockWheelContainer=new QWidget(this);
    QBoxLayout *containerLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                               clockWheelContainer);
    containerLayout->setContentsMargins(0,0,0,0);
    clockWheelContainer->setLayout(containerLayout);
    m_loadingWheel=new KNClockWheel(this);
    containerLayout->addWidget(m_loadingWheel);
    m_lyricsList->setEmptyWidget(clockWheelContainer);
    m_lyricsListView=new KNMusicLyricsListView(this);
    m_lyricsList->setContentWidget(m_lyricsListView);
    //Set default widget to content widget.
    m_lyricsList->showContentWidget();

}

inline KNIconFrameLineEdit *KNMusicLyricsDownloadWidget::generateLineEdit(const QPixmap &icon)
{
    KNIconFrameLineEdit *lineEdit=new KNIconFrameLineEdit(this);
    lineEdit->setMinimumWidth(550);
    lineEdit->disableEscapeKey();
    lineEdit->setMinimumLightness(0xD0);
    lineEdit->setMouseEnterLightness(0xE0);
    lineEdit->setIcon(icon);
    lineEdit->setFrameStyle(QFrame::Box);
    return lineEdit;
}
