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

#include "kniconframelineedit.h"

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
}

KNMusicLyricsDownloadWidget::~KNMusicLyricsDownloadWidget()
{
    ;
}

void KNMusicLyricsDownloadWidget::setTitle(const QString &title)
{
    m_title->setText(title);
}

void KNMusicLyricsDownloadWidget::setArtist(const QString &artist)
{
    m_artist->setText(artist);
}

inline KNIconFrameLineEdit *KNMusicLyricsDownloadWidget::generateLineEdit(const QPixmap &icon)
{
    KNIconFrameLineEdit *lineEdit=new KNIconFrameLineEdit(this);
    lineEdit->setMinimumWidth(300);
    lineEdit->disableEscapeKey();
    lineEdit->setMinimumLightness(0xD0);
    lineEdit->setMouseEnterLightness(0xE0);
    lineEdit->setIcon(icon);
    lineEdit->setFrameStyle(QFrame::Box);
    return lineEdit;
}
