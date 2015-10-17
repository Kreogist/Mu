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
#include <QLabel>
#include <QBoxLayout>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knglassbutton.h"

#include "knmusicplaylistemptyhint.h"

KNMusicPlaylistEmptyHint::KNMusicPlaylistEmptyHint(QWidget *parent) :
    KNDropProxyContainer(parent),
    m_hintText(new QLabel(this)),
    m_addPlaylist(
        generateButton(":/plugin/music/playlist/playlist_add_empty.png")),
    m_importPlaylist(
        generateButton(":/plugin/music/playlist/playlist_import_empty.png"))
{
    setObjectName("PlaylistEmptyHint");
    //Set properties.
    setAutoFillBackground(true);

    //Configure the hint text.
    m_hintText->setMaximumWidth(600);
    m_hintText->setWordWrap(true);
    m_hintText->setObjectName("PlaylistEmptyHintLabel");
    QFont labelFont=m_hintText->font();
    labelFont.setPixelSize(15);
    m_hintText->setFont(labelFont);
    knTheme->registerWidget(m_hintText);
    //Initial the icon.
    QLabel *icon=new QLabel(this);
    icon->setPixmap(QPixmap(":/plugin/music/playlist/empty.png"));
    //Configure the button.
    connect(m_addPlaylist, &KNGlassButton::released,
            this, &KNMusicPlaylistEmptyHint::requireAddPlaylist);
    connect(m_importPlaylist, &KNGlassButton::released,
            this, &KNMusicPlaylistEmptyHint::requireImportPlaylists);

    //Initial the layout of the widget.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);
    //Initial the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(20);
    //Add content layout to main layout.
    mainLayout->addStretch();
    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();
    //Add icon to content layout.
    contentLayout->addWidget(icon, 0, Qt::AlignRight | Qt::AlignVCenter);
    //Initial the hint text and button layout.
    QBoxLayout *textButtonLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                mainLayout->widget());
    textButtonLayout->setSpacing(3);
    contentLayout->addLayout(textButtonLayout);
    //Add hint label to content layout.
    textButtonLayout->addStretch();
    textButtonLayout->addWidget(m_hintText);
    textButtonLayout->addSpacing(20);
    textButtonLayout->addWidget(m_addPlaylist);
    textButtonLayout->addWidget(m_importPlaylist);
    textButtonLayout->addStretch();

    //Register to palette.
    knTheme->registerWidget(this);
    //Link to retranslate.
    knI18n->link(this, &KNMusicPlaylistEmptyHint::retranslate);
    retranslate();
}

void KNMusicPlaylistEmptyHint::retranslate()
{
    //Update the hint text.
    m_hintText->setText(tr("Songs you add to playlists and the import playlists"
                           " appears in Playlist.\n"
                           "To create a playlist, drag your songs here."));

    //Update the button text.
    m_addPlaylist->setText(tr("New playlist"));
    m_importPlaylist->setText(tr("Import playlists"));
}

KNGlassButton *KNMusicPlaylistEmptyHint::generateButton(
        const QString &iconPath)
{
    //Generate a button.
    KNGlassButton *button=new KNGlassButton(this);
    //Change the size of the button
    button->setFixedSize(300,33);
    //Set the pixmap.
    button->setIcon(QIcon(iconPath));
    //Get the button font.
    QFont buttonFont=button->font();
    //Tweak the font.
    buttonFont.setPixelSize(15);
    //Set the font back.
    button->setFont(buttonFont);
    //Send the button.
    return button;
}

