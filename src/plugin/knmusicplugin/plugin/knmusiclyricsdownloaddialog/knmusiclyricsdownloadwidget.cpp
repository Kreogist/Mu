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

#include "knlabellineedit.h"
#include "knlocalemanager.h"
#include "knmusiclyricsdownloadlist.h"

#include "knmusiclyricsdownloadwidget.h"

KNMusicLyricsDownloadWidget::KNMusicLyricsDownloadWidget(QWidget *parent) :
    QWidget(parent),
    m_title(generateLineEdit(":/plugin/music/lyrics/title.png")),
    m_artist(generateLineEdit(":/plugin/music/lyrics/artist.png")),
    m_searchLyrics(new QPushButton(this)),
    m_searchTextLayout(nullptr),
    m_downloadLyrics(new KNMusicLyricsDownloadList(this))
{
    //Set properties.
    setContentsMargins(15, 15, 15, 15);

    //Configure the title line edit.
    setTabOrder(m_title, m_artist);
    //Configure the search button.
    setTabOrder(m_artist, m_searchLyrics);
    m_searchLyrics->setDefault(true);
    // Set the palette for the search lyrics button.
    QPalette buttonPalette=m_searchLyrics->palette();
    buttonPalette.setColor(QPalette::Highlight, QColor(0xf7, 0xcf, 0x3d));
    m_searchLyrics->setPalette(buttonPalette);
    // Link the click signal.
    connect(m_searchLyrics, &QPushButton::clicked,
            this, &KNMusicLyricsDownloadWidget::onActionSearch);
    //Configure the download list.
    m_downloadLyrics->setVisible(false);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    //Initial the search text layout.
    m_searchTextLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      mainLayout->widget());
    m_searchTextLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(m_searchTextLayout);
    //Initial the text layout.
    QBoxLayout *textLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          m_searchTextLayout->widget());
    textLayout->setContentsMargins(0,0,0,0);
    m_searchTextLayout->addLayout(textLayout, 1);
    //Add widget to text layout.
    textLayout->addWidget(m_title);
    textLayout->addWidget(m_artist);
    //Add search lyrics button to search text layout.
    m_searchTextLayout->addWidget(m_searchLyrics);
    //Add download lyrics to the main layout.
    mainLayout->addWidget(m_downloadLyrics);

    //Link the retranslate link.
    knI18n->link(this, &KNMusicLyricsDownloadWidget::retranslate);
    retranslate();

    //Set the initial size of the widget.
    resize(400, m_searchTextLayout->sizeHint().height());
}

void KNMusicLyricsDownloadWidget::setParameter(const QString &title,
                                               const QString &artist)
{
    //Set the title and artist text.
    m_title->setText(title);
    m_artist->setText(artist);
    //Once we do this, means we have to show up in a new instance.
    //Hide the download list.
    m_downloadLyrics->setVisible(false);
    //Resize the widget.
    resize(400, m_searchTextLayout->sizeHint().height());
}

void KNMusicLyricsDownloadWidget::retranslate()
{
    //Update search lyrics.
    m_searchLyrics->setText(tr("Search"));
}

void KNMusicLyricsDownloadWidget::onActionSearch()
{
    //Expand the dialog.
    emit requireExpand();
    //Show up the download list.
    m_downloadLyrics->setVisible(true);
}

inline KNLabelLineEdit *KNMusicLyricsDownloadWidget::generateLineEdit(
        const QString &pixmapPath)
{
    //Generate the widget.
    KNLabelLineEdit *lineEdit=new KNLabelLineEdit(this);
    //Set properties.
    lineEdit->setMinimumLightness(0xC0);
    lineEdit->setMediumLightness(0xE0);
    //Set the pixmap.
    lineEdit->setLabelIcon(QPixmap(pixmapPath));
    //Give back the line edit widget.
    return lineEdit;
}
