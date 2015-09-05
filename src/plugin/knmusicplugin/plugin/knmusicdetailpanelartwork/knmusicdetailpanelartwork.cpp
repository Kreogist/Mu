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
#include <QLabel>
#include <QBoxLayout>

#include "kncircleiconbutton.h"

#include "knmusicglobal.h"

#include "knmusicdetailpanelartwork.h"

#define ArtworkSize 225

KNMusicDetailPanelArtwork::KNMusicDetailPanelArtwork(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_button(new KNCircleIconButton(this)),
    m_albumArt(new QLabel(this))
{
    //Configure the button.
    m_button->setIcon(
                QIcon(":/plugin/music/detaildialog/tab_icon/albumart.png"));

    //Configure the label.
    m_albumArt->setFixedSize(ArtworkSize, ArtworkSize);

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);
    //Add the label to the layout.
    mainLayout->addWidget(m_albumArt);
}

QAbstractButton *KNMusicDetailPanelArtwork::tabButton()
{
    return m_button;
}

void KNMusicDetailPanelArtwork::setAnalysisItem(const KNMusicAnalysisItem &item)
{
    //Check the cover image is valid or not.
    if(item.coverImage.isNull())
    {
        //Set the no album image.
        m_albumArt->setPixmap(
                    knMusicGlobal->noAlbumArt().scaled(
                        ArtworkSize,
                        ArtworkSize,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    }
    else
    {
        //Load image from the item.
        m_albumArt->setPixmap(QPixmap::fromImage(item.coverImage).scaled(
                                  ArtworkSize,
                                  ArtworkSize,
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));
    }
}

