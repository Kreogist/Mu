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
#include <QFileDialog>
#include <QFileInfo>

#include "kncircleiconbutton.h"
#include "knlocalemanager.h"
#include "knopacityanimebutton.h"
#include "knglobal.h"

#include "knmusicglobal.h"
#include "knmusicparser.h"

#include "knmusicdetailpanelartwork.h"

#define ArtworkSize 261

#include <QDebug>

KNMusicDetailPanelArtwork::KNMusicDetailPanelArtwork(QWidget *parent) :
    KNMusicDetailDialogPanel(parent),
    m_lastDirectory(QString()),
    m_button(new KNCircleIconButton(this)),
    m_albumArt(new QLabel(this))
{
    //Configure the button.
    m_button->setIcon(
                QIcon(":/plugin/music/detaildialog/tab_icon/albumart.png"));

    //Configure the label.
    m_albumArt->setMaximumSize(ArtworkSize, ArtworkSize);

    //Initial the box layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    //Add the label to the layout.
    mainLayout->addWidget(m_albumArt, 0, Qt::AlignHCenter);
    //Add the button layouts.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            mainLayout->widget());
    buttonLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(buttonLayout, 0);
    buttonLayout->addStretch();
    //Initial the buttons.
    for(int i=0; i<OperationCount; ++i)
    {
        //Generate operation button.
        m_operations[i]=new KNOpacityAnimeButton(this);
        //Set the fixed size of the button.
        m_operations[i]->setFixedSize(26, 26);
        //Add operation button to the layout.
        buttonLayout->addWidget(m_operations[i]);
    }
    buttonLayout->addStretch();

    //Set the icon.
    m_operations[SetAlbumArt]->setIcon(
                QIcon(":/plugin/music/detaildialog/albumart/edit.png"));
    m_operations[ExportAlbumArt]->setIcon(
                QIcon(":/plugin/music/detaildialog/albumart/save.png"));
    //Hide the set album art button, for 0.9.
    m_operations[SetAlbumArt]->hide();

    //Link the operations.
    connect(m_operations[SetAlbumArt], &KNOpacityAnimeButton::clicked,
            this, &KNMusicDetailPanelArtwork::onActionChangeImage);
    connect(m_operations[ExportAlbumArt], &KNOpacityAnimeButton::clicked,
            this, &KNMusicDetailPanelArtwork::onActionSaveImage);

    //Link the locale manager.
    knI18n->link(this, &KNMusicDetailPanelArtwork::retranslate);
    retranslate();
}

QAbstractButton *KNMusicDetailPanelArtwork::tabButton()
{
    return m_button;
}

void KNMusicDetailPanelArtwork::setAnalysisItem(const KNMusicAnalysisItem &item,
                                                KNMusicProxyModel *proxyModel,
                                                const QModelIndex &proxyIndex)
{
    Q_UNUSED(proxyModel)
    Q_UNUSED(proxyIndex)
    //Save the image.
    m_currentItem=item;
    //Check out the track of the detail info.
    m_operations[SetAlbumArt]->setVisible(
                m_currentItem.detailInfo.trackFilePath.isEmpty());
    //Check the cover image is valid or not.
    QPixmap &&scaledPixmap=
            ((m_currentItem.coverImage.isNull())?
                 knMusicGlobal->noAlbumArt():
                 QPixmap::fromImage(m_currentItem.coverImage)).scaled(
                ArtworkSize,
                ArtworkSize,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation);
    m_albumArt->setPixmap(scaledPixmap);
    m_albumArt->resize(scaledPixmap.size());
}

void KNMusicDetailPanelArtwork::retranslate()
{
    //Update the album art.
    m_operations[SetAlbumArt]->setToolTip(tr("Set new album cover"));
    m_operations[ExportAlbumArt]->setToolTip(tr("Save album cover image"));
}

void KNMusicDetailPanelArtwork::onActionChangeImage()
{
    //Get the image.
    QString targetFilePath=QFileDialog::getOpenFileName(
                this,
                tr("Select the new album cover image"),
                m_lastDirectory,
                knGlobal->writeImageFilter());
    //Prepare the new image.
    QImage targetImage;
    //Check out the image file's validation.
    if(targetFilePath.isEmpty() ||
            !targetImage.load(targetFilePath))
    {
        return;
    }
    //Set the new image.
    m_currentItem.coverImage=targetImage;
    //Write the item.
    if(knMusicGlobal->parser()!=nullptr &&
            knMusicGlobal->parser()->writeAlbumArt(m_currentItem))
    {
        //We have to update the file information.
        emit requireUpdateFileInfo();
    }
}

void KNMusicDetailPanelArtwork::onActionSaveImage()
{
    //Check whether the cover image is null or not.
    if(m_currentItem.coverImage.isNull())
    {
        //We cannot save a null image.
        return;
    }
    //Get the save image file path.
    QString targetFilePath=QFileDialog::getSaveFileName(
                this,
                tr("Save album cover"),
                m_lastDirectory,
                knGlobal->writeImageFilter());
    //Check out the target file path.
    if(targetFilePath.isEmpty())
    {
        //Ignore the invalid file path.
        return;
    }
    //Save the image to the target file path.
    m_currentItem.coverImage.save(targetFilePath);
    //Save the last dirctory.
    m_lastDirectory=QFileInfo(targetFilePath).absoluteFilePath();
}

