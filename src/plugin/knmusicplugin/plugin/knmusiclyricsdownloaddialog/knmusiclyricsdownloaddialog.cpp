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
#include "knglobal.h"

#include "knmusicglobal.h"
#include "knmusicbackend.h"
#include "knmusiclyricsdownloadwidget.h"

#include "knmusiclyricsdownloaddialog.h"

#include <QDebug>

KNMusicLyricsDownloadDialog::KNMusicLyricsDownloadDialog(QWidget *parent) :
    KNMusicLyricsDownloadDialogBase(knGlobal->mainWindow()),
    m_downloadWidget(new KNMusicLyricsDownloadWidget(this))
{
    Q_UNUSED(parent)
    //Set properties.
    setContentWidget(m_downloadWidget);
    setTitleText("Lyrics");
    setShowCancelButton(true);
    setShowOkayButton(false);

    //Configure the download widget.
    connect(m_downloadWidget, &KNMusicLyricsDownloadWidget::requireExpand,
            this, &KNMusicLyricsDownloadDialog::onActionExpand);
    connect(m_downloadWidget,
            &KNMusicLyricsDownloadWidget::requireShowOkayButton,
            [=]{setButtonVisible(true, true);});
    connect(m_downloadWidget,
            &KNMusicLyricsDownloadWidget::requireHideOkayButton,
            [=]{setButtonVisible(true, false);});
}

void KNMusicLyricsDownloadDialog::setDetailInfo(
        const KNMusicDetailInfo &detailInfo)
{
    //Set the title and artist text data to the panel.
    m_downloadWidget->setDetailInfo(detailInfo);
}

void KNMusicLyricsDownloadDialog::onActionExpand()
{
    //Resize the dialog.
    resizeDialog(QSize(400, 400));
}

bool KNMusicLyricsDownloadDialog::okayPressed()
{
    //Reset the preview thread.
    resetPreviewThread();
    //Save the lyrics.
    m_downloadWidget->saveSelectLyrics();
    //Give back the original settings.
    return KNMusicLyricsDownloadDialogBase::okayPressed();
}

void KNMusicLyricsDownloadDialog::cancelPressed()
{
    //Reset the preview thread.
    resetPreviewThread();
    //Do original cancel.
    KNMusicLyricsDownloadDialogBase::cancelPressed();
}

inline void KNMusicLyricsDownloadDialog::resetPreviewThread()
{
    //Get the backend.
    KNMusicBackend *backend=knMusicGlobal->backend();
    //Check the backend.
    if(backend)
    {
        //Reset the preview thread.
        backend->previewReset();
    }
}
