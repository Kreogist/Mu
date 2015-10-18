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
#include <QLabel>
#include <QFileDialog>

#include "knglassbutton.h"
#include "knthememanager.h"
#include "knlocalemanager.h"

#include "knmusiclibraryemptyhint.h"

#include <QDebug>

KNMusicLibraryEmptyHint::KNMusicLibraryEmptyHint(QWidget *parent) :
    KNDropProxyContainer(parent),
    m_hintText(new QLabel(this)),
    m_addMusic(generateButton(":/plugin/music/library/library_add.png"))
{
    setObjectName("LibraryEmptyHint");
    //Set properties.
    setAutoFillBackground(true);
    //Configure the hint text label.
    m_hintText->setObjectName("LibraryEmptyHintLabel");
    m_hintText->setMaximumWidth(600);
    m_hintText->setWordWrap(true);
    QFont hintTextFont=m_hintText->font();
    hintTextFont.setPixelSize(15);
    m_hintText->setFont(hintTextFont);
    knTheme->registerWidget(m_hintText);
    //Link the command buttons.
    connect(m_addMusic, &KNGlassButton::clicked,
            this, &KNMusicLibraryEmptyHint::onActionAddToLibrary);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);
    //Add widgets to main layout.
    mainLayout->addStretch();
    //Initial the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(20);
    //Add content to the main layout.
    mainLayout->addLayout(contentLayout);
    //Initial the empty hint icon.
    QLabel *emptyHintIcon=new QLabel(this);
    emptyHintIcon->setPixmap(QPixmap(":/plugin/music/library/empty.png"));
    //Add empty hint icon to content layout.
    contentLayout->addWidget(emptyHintIcon,
                             0,
                             Qt::AlignRight | Qt::AlignVCenter);
    //Initial the hint layout.
    QBoxLayout *hintLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          contentLayout->widget());
    hintLayout->setSpacing(5);
    //Add widgets to hint layout.
    hintLayout->addStretch();
    hintLayout->addWidget(m_hintText);
    hintLayout->addSpacing(20);
    hintLayout->addWidget(m_addMusic);
    hintLayout->addStretch();

    //Add hint layout to the content layout.
    contentLayout->addLayout(hintLayout);
    mainLayout->addStretch();

    //Link to the theme manager.
    knTheme->registerWidget(this);

    //Link the locale manager.
    knI18n->link(this, &KNMusicLibraryEmptyHint::retranslate);
    retranslate();
}

void KNMusicLibraryEmptyHint::retranslate()
{
    //Update the hint text.
    m_hintText->setText(tr("Songs and music you add to Mu appear in your music "
                           "library.\n"
                           "To add songs to library, drag your songs here."));

    //Update the operate button text.
    m_addMusic->setText(tr("Add songs"));
}

void KNMusicLibraryEmptyHint::onActionAddToLibrary()
{
    //Generate the file dialog.
    QFileDialog fileDialog(this,
                           tr("Add To Library"),
                           QString("."));
    //Configure the file dialog.
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    //Launch the file dialog.
    if(fileDialog.exec()==QDialog::Accepted &&
            !fileDialog.selectedUrls().isEmpty())
    {
        emit urlsDropped(fileDialog.selectedUrls());
    }
}

inline KNGlassButton *KNMusicLibraryEmptyHint::generateButton(
        const QString &iconPath)
{
    //Generate a glass button.
    KNGlassButton *button=new KNGlassButton(this);
    //Configure the button.
    button->setIconSize(QSize(28, 28));
    button->setMinimumHeight(34);
    button->setIcon(QIcon(iconPath));
    //Get the button font.
    QFont buttonFont=button->font();
    //Tweak the font.
    buttonFont.setPixelSize(15);
    //Set the font back.
    button->setFont(buttonFont);
    //Give back the button.
    return button;
}

