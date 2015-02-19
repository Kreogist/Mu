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

#include <QWidget>

class QPushButton;
class QAbstractItemModel;
class KNClockWheel;
class KNIconFrameLineEdit;
class KNMusicLyricsListView;
class KNEmptyStateWidget;
class KNMusicLyricsDownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloadWidget(QWidget *parent = 0);
    ~KNMusicLyricsDownloadWidget();
    void setLyricsModel(QAbstractItemModel *model);

    QString title() const;
    QString artist() const;

    void setTitle(const QString &title);
    void setArtist(const QString &artist);

signals:
    void requireSearchLyrics();

public slots:
    void showLoadingWheel();
    void showLyricsList();

private:
    inline void initialListView();
    inline KNIconFrameLineEdit *generateLineEdit(const QPixmap &icon);
    KNClockWheel *m_loadingWheel;
    KNIconFrameLineEdit *m_title, *m_artist;
    KNEmptyStateWidget *m_lyricsList;
    KNMusicLyricsListView *m_lyricsListView;
    QPushButton *m_searchLyrics;
};

#endif // KNMUSICLYRICSDOWNLOADWIDGET_H
