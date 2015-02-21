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
#include <QScrollBar>
#include <QPlainTextEdit>
#include <QStandardItem>

#include "kniconframelineedit.h"
#include "knclockwheel.h"
#include "knemptystatewidget.h"
#include "sao/knsaostyle.h"

#include "knmusiclyricspreviewplayer.h"
#include "knmusiclyricslistview.h"
#include "knmusicglobal.h"

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

    //Initial the lyrics list.
    initialListView();
    //Initial the previews.
    initialPlainTextPreview();
    initialPlayerPreview();

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
                                              searchRequestLayout->widget());
    textEditLayout->setContentsMargins(0,0,0,0);
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

void KNMusicLyricsDownloadWidget::setBackend(KNMusicBackend *backend)
{
    m_playerPreview->setBackend(backend);
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

void KNMusicLyricsDownloadWidget::linkBackend()
{
    m_playerPreview->linkBackend();
}

void KNMusicLyricsDownloadWidget::resetPreviewPlayer()
{
    m_playerPreview->resetPreviewPlayer();
}

QModelIndex KNMusicLyricsDownloadWidget::currentLyricsIndex() const
{
    return m_lyricsListView->currentLyricsIndex();
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

void KNMusicLyricsDownloadWidget::showLyricsItem(QStandardItem *item)
{
    //Check the item.
    if(item==nullptr)
    {
        //Then clear all the data.
        clearPreview();
        return;
    }
    //Display the lyrics item.
    QString lyricsText=item->data(LyricsTextRole).toString();
    m_plainTextPreview->setPlainText(lyricsText);
    m_playerPreview->setLyrics(lyricsText);
}

void KNMusicLyricsDownloadWidget::clearPreview()
{
    //Clear the plain text preview.
    m_plainTextPreview->clear();
    m_playerPreview->clearLyrics();
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
    connect(m_lyricsListView, &KNMusicLyricsListView::lyricsActivate,
            this, &KNMusicLyricsDownloadWidget::lyricsActivate);
    m_lyricsList->setContentWidget(m_lyricsListView);
    //Set default widget to content widget.
    m_lyricsList->showContentWidget();
}

void KNMusicLyricsDownloadWidget::initialPlainTextPreview()
{
    //Generate plain text preview widget.
    m_plainTextPreview=new QPlainTextEdit(this);
    m_plainTextPreview->setReadOnly(true);
    m_plainTextPreview->setFrameStyle(QFrame::NoFrame);
    QPalette pal=m_plainTextPreview->palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    pal.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    pal.setColor(QPalette::HighlightedText, QColor(0,0,0));
    m_plainTextPreview->setPalette(pal);
    //Set scroll bar style sheet.
    KNSAOStyle::setScrollBarStyleSheet(m_plainTextPreview->verticalScrollBar());
    //Add the previewer.
    m_lyricsListView->addPreviewer(QPixmap(":/plugin/music/lyrics/text_preview.png"),
                                   m_plainTextPreview);
}

void KNMusicLyricsDownloadWidget::initialPlayerPreview()
{
    //Generate the preview player.
    m_playerPreview=new KNMusicLyricsPreviewPlayer(this);
    //Add the previewer.
    m_lyricsListView->addPreviewer(QPixmap(":/plugin/music/lyrics/lyrics_preview.png"),
                                   m_playerPreview);
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
