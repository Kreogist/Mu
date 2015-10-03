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

#ifndef KNMUSICLYRICSDOWNLOADWIDGET_H
#define KNMUSICLYRICSDOWNLOADWIDGET_H

#include "knmusicutil.h"

#include <QWidget>

using namespace MusicUtil;

class QBoxLayout;
class QPushButton;
class KNLabelLineEdit;
class KNMusicLyricsDownloadList;
class KNMusicOnlineLyricsDownloader;
class KNMusicLyricsDownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloadWidget(QWidget *parent = 0);

signals:
    void requireExpand();
    void requireCancel();
    void requireShowOkayButton();
    void requireHideOkayButton();
    void requireDownloadLyrics(KNMusicDetailInfo detailInfo);

public slots:
    void setDetailInfo(const KNMusicDetailInfo &detailInfo);
    void saveSelectLyrics();

private slots:
    void retranslate();
    void onActionSearch();

protected:
    void hideEvent(QHideEvent *event);

private slots:
    void onActionDownloadCancel();
    void onActionDownloadComplete();

private:
    inline void enableSearchEdit();
    inline KNLabelLineEdit *generateLineEdit(const QString &pixmapPath);
    KNMusicDetailInfo m_detailInfo;
    KNLabelLineEdit *m_title, *m_artist;
    QPushButton *m_searchLyrics;
    QBoxLayout *m_searchTextLayout;
    KNMusicLyricsDownloadList *m_downloadedLyrics;
    KNMusicOnlineLyricsDownloader *m_onlineDownloader;
};

#endif // KNMUSICLYRICSDOWNLOADWIDGET_H
