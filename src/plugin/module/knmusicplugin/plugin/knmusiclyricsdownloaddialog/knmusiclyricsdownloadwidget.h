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

class QPlainTextEdit;
class QPushButton;
class QAbstractItemModel;
class QStandardItem;
class KNClockWheel;
class KNIconFrameLineEdit;
class KNEmptyStateWidget;
class KNMusicBackend;
class KNMusicLyricsPreviewPlayer;
class KNMusicLyricsListView;
class KNMusicLyricsDownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicLyricsDownloadWidget(QWidget *parent = 0);
    ~KNMusicLyricsDownloadWidget();
    void setLyricsModel(QAbstractItemModel *model);
    void setBackend(KNMusicBackend *backend);

    QString title() const;
    QString artist() const;

    void setTitle(const QString &title);
    void setArtist(const QString &artist);

    void linkBackend();
    void resetPreviewPlayer();
    QModelIndex currentLyricsIndex() const;

signals:
    void requireSearchLyrics();
    void lyricsActivate(const QModelIndex &index);

public slots:
    void showLoadingWheel();
    void showLyricsList();
    void showLyricsItem(QStandardItem *item);
    void clearPreview();

private:
    inline void initialListView();
    inline void initialPlainTextPreview();
    inline void initialPlayerPreview();
    inline KNIconFrameLineEdit *generateLineEdit(const QPixmap &icon);
    KNClockWheel *m_loadingWheel;
    KNIconFrameLineEdit *m_title, *m_artist;
    KNEmptyStateWidget *m_lyricsList;
    KNMusicLyricsListView *m_lyricsListView;
    QPushButton *m_searchLyrics;

    QPlainTextEdit *m_plainTextPreview;
    KNMusicLyricsPreviewPlayer *m_playerPreview;
};

#endif // KNMUSICLYRICSDOWNLOADWIDGET_H
