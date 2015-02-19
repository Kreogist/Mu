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
#ifndef KNMUSICLYRICSDOWNLOADDIALOG_H
#define KNMUSICLYRICSDOWNLOADDIALOG_H

#include "knmusiclyricsdownloaddialogbase.h"

class QStandardItemModel;
class KNMusicLyricsManager;
class KNMusicLyricsDownloadWidget;
class KNMusicLyricsDownloadDialog : public KNMusicLyricsDownloadDialogBase
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloadDialog(QWidget *parent = 0);
    ~KNMusicLyricsDownloadDialog();

    void setDetailInfo(const KNMusicDetailInfo &detailInfo);

signals:

public slots:

private slots:
    void onActionSearchLyrics();

private:
    //UI, Lyrics manager.
    KNMusicLyricsDownloadWidget *m_downloadWidget;
    KNMusicLyricsManager *m_lyricsManager;

    //Current information.
    KNMusicDetailInfo m_detailInfo;

    //Lyrics model.
    QStandardItemModel *m_lyricsModel;
};

#endif // KNMUSICLYRICSDOWNLOADDIALOG_H
