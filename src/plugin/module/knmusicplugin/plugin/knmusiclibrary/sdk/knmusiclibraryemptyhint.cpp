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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include <QLabel>
#include <QBoxLayout>
#include <QFileDialog>

#include "knglobal.h"
#include "knngnlbutton.h"

#include "knmusiclibraryemptyhint.h"

KNMusicLibraryEmptyHint::KNMusicLibraryEmptyHint(QWidget *parent) :
    KNDropProxyContainer(parent)
{
    //Set properties.
    setAutoFillBackground(true);

    //Set palette.
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0x35, 0x35, 0x35));
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    setPalette(pal);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);

    mainLayout->addStretch();

    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(20);
    mainLayout->addLayout(contentLayout);

    //Initial the empty hint icon.
    QLabel *emptyHintIcon=new QLabel(this);
    emptyHintIcon->setPixmap(QPixmap(":/plugin/music/common/emptyicon.png"));
    contentLayout->addWidget(emptyHintIcon,
                          0,
                          Qt::AlignRight | Qt::AlignVCenter);

    //Initial the hint layout.
    QBoxLayout *hintLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          contentLayout->widget());
    hintLayout->setSpacing(5);
    contentLayout->addLayout(hintLayout);
    mainLayout->addStretch();

    hintLayout->addStretch();
    //Initial the hint text lable.
    m_hintText=new QLabel(this);
    //Set palette.
    pal=m_hintText->palette();
    pal.setColor(QPalette::WindowText, QColor(0xA0, 0xA0, 0xA0));
    m_hintText->setPalette(pal);
    //Set the property.
    m_hintText->setMaximumWidth(600);
    m_hintText->setWordWrap(true);
    QFont hintTextFont=m_hintText->font();
    hintTextFont.setPixelSize(15);
    m_hintText->setFont(hintTextFont);
    //Add to layout.
    hintLayout->addWidget(m_hintText);
    hintLayout->addSpacing(20);

    //Initial the operate button.
    m_addMusic=new KNNGNLButton(this);
    m_addMusic->setIcon(QPixmap(":/plugin/music/library/library_add.png"));
    configureNGNLButton(m_addMusic);
    connect(m_addMusic, &KNNGNLButton::clicked,
            this, &KNMusicLibraryEmptyHint::onActionAddToLibrary);
    hintLayout->addWidget(m_addMusic);
    hintLayout->addStretch();

    //Connect retranslate signal.
    connect(KNGlobal::instance(), &KNGlobal::requireRetranslate,
            this, &KNMusicLibraryEmptyHint::retranslate);
    //Retranslate.
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
    QStringList addedFiles=QFileDialog::getOpenFileNames(this, tr("Add To Library"));
    //If there's any file we can get, ask to analysis these files.
    if(!addedFiles.isEmpty())
    {
        emit requireAnalysisFiles(addedFiles);
    }
}

void KNMusicLibraryEmptyHint::configureNGNLButton(KNNGNLButton *button)
{
    button->setIconSize(28, 28);
    button->setMinimumHeight(34);
}
